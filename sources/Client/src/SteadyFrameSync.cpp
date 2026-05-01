#include "stdafx.h"
#include "SteadyFrameSync.h"
#include "CrushSystem.h"

namespace Corsairs::Client::Frame {

namespace {
//  Окно адаптивной оценки FPS — сколько тиков фонового потока копим перед
//  пересчётом fRate.
constexpr std::uint32_t kAdaptiveWindowTicks = 60;

//  Текущий момент в наносекундах от epoch'и steady_clock — атомарный snapshot,
//  пригодный для хранения в std::atomic<int64>.
std::int64_t NowNs() {
	return std::chrono::steady_clock::now().time_since_epoch().count();
}
} // namespace

SteadyFrameSync& SteadyFrameSync::Instance() {
	static SteadyFrameSync instance;
	return instance;
}

bool SteadyFrameSync::IsFramerate60() const {
	//  Backward-compat для UI чекбокса «60 FPS вкл/выкл». Произвольный target
	//  >= 60 (например 144) тоже считаем «вкл» — чекбокс отображает грубо.
	return GetFps() >= 60u;
}

void SteadyFrameSync::SetFramerate60(bool v) {
	//  Backward-compat обёртка для UI чекбокса. Новый код — SetFps(N) напрямую.
	SetFps(v ? 60 : 30);
}

std::uint32_t SteadyFrameSync::GetFps() const {
	return _fps.load(std::memory_order_relaxed);
}

std::uint32_t SteadyFrameSync::GetTargetFps() const {
	return _targetFps.load(std::memory_order_relaxed);
}

void SteadyFrameSync::SetFps(std::uint32_t fps) {
	//  Запомнить как user-target — это потолок адаптивной коррекции.
	_targetFps.store(fps, std::memory_order_relaxed);
	_UpdateCurrentFps(fps);
}

void SteadyFrameSync::_UpdateCurrentFps(std::uint32_t fps) {
	_fps.store(fps, std::memory_order_relaxed);
	const auto interval = static_cast<std::uint32_t>(1000.0f / static_cast<float>(fps));
	_sleepIntervalMs.store(interval, std::memory_order_relaxed);
}

float SteadyFrameSync::GetAnimMultiplier() const {
	return static_cast<float>(GetFps()) / 30.0f;
}

void SteadyFrameSync::RefreshFps() {
	//  Адаптация снижает _fps под нагрузкой — _targetFps при этом не трогается,
	//  чтобы при спаде нагрузки можно было вернуться к user-target.
	const auto current = _fps.load(std::memory_order_relaxed);
	const auto pending = _refreshFps.load(std::memory_order_relaxed);
	if (current != pending) {
		_UpdateCurrentFps(pending);
	}
}

bool SteadyFrameSync::Run() {
	//  Атомарный exchange — забираем все накопленные тики и сбрасываем счётчик
	//  одной операцией, чтобы фоновый поток не потерял инкремент между чтением и записью.
	const auto pending = _pendingTicks.exchange(0, std::memory_order_acquire);
	if (pending <= 0) {
		return false;
	}

	//  Адаптивная коррекция применяется здесь, на каждом кадре. Раньше требовала
	//  явного RefreshFps() извне (Character.cpp и т.п.) — фактически срабатывало
	//  только при движении ГГ; теперь автоматически.
	RefreshFps();

	//  GetTickCount — для внешнего FrameMove(DWORD) контракта,
	//  steady_clock — для внутреннего точного замера длительности кадра в End().
	_currentTickMs.store(static_cast<std::uint32_t>(GetTickCount()), std::memory_order_relaxed);
	_frameStartNs.store(NowNs(), std::memory_order_relaxed);
	return true;
}

std::uint32_t SteadyFrameSync::GetTick() const {
	return _currentTickMs.load(std::memory_order_relaxed);
}

void SteadyFrameSync::End() {
	//  steady_clock даёт ~100 нс резолюцию (под Windows = QueryPerformanceCounter).
	//  Раньше тут было GetTickCount — резолюция 15.6 мс, и кадры быстрее 15 мс
	//  вообще не учитывались, rate в адаптивной логике выходил 0.
	const auto elapsedNs = NowNs() - _frameStartNs.load(std::memory_order_relaxed);
	if (elapsedNs > 0) {
		_totalRenderNs.fetch_add(elapsedNs, std::memory_order_relaxed);
	}
}

bool SteadyFrameSync::Init() {
	//  Подстраховка: к моменту Init() LoadCustomProp уже должен был вызвать
	//  SetFps(N) с user-target. Если по какой-то причине _targetFps остался 0
	//  (порядок инициализации поломан) — поднимаем до дефолтных 30.
	if (_targetFps.load(std::memory_order_relaxed) < 30u) {
		SetFps(30);
	}
	_refreshFps.store(_fps.load(std::memory_order_relaxed), std::memory_order_relaxed);

	_totalRenderNs.store(0, std::memory_order_relaxed);
	_pendingTicks.store(0, std::memory_order_relaxed);

	//  Поднимаем резолюцию системного таймера до 1 мс — иначе sleep_for(16ms)
	//  округляется до следующего scheduler-тика (~31 мс) и FPS режется до ~32.
	if (::timeBeginPeriod(1) == TIMERR_NOERROR) {
		_isTimerPeriodSet = true;
	}

	//  jthread получает stop_token первым параметром и сам делает request_stop+join
	//  в деструкторе. SetThreadName/CRT-крэш-обработчик ставим в начале лямбды,
	//  раньше это делал static _SleepThreadProc.
	_thread = std::jthread([this](std::stop_token stop) {
		::SetThreadName("steady-frame");
		TalesOfPirate::Utils::Crush::SetPerThreadCRTExceptionBehavior();
		_SleepLoop(stop);
	});

	if (_thread.joinable()) {
		ToLogService("common", "_SleepThreadProc started");
		return true;
	}
	return false;
}

void SteadyFrameSync::Exit() {
	if (_thread.joinable()) {
		//  request_stop поставит флаг в stop_token. Поток выйдет после очередного
		//  sleep_for, увидев stop_requested(). join блокирует до его выхода (≤ один
		//  sleep-интервал ~125 мс при минимальном 8 FPS).
		_thread.request_stop();
		_thread.join();
		ToLogService("common", "_SleepThreadProc end");
	}

	if (_isTimerPeriodSet) {
		::timeEndPeriod(1);
		_isTimerPeriodSet = false;
	}
}

void SteadyFrameSync::_SleepLoop(std::stop_token stop) {
	std::uint32_t count = 0;
	auto windowStart = std::chrono::steady_clock::now();

	while (!stop.stop_requested()) {
		const auto interval = _sleepIntervalMs.load(std::memory_order_relaxed);
		std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		_pendingTicks.fetch_add(1, std::memory_order_release);

		count++;
		if (count < kAdaptiveWindowTicks) {
			continue;
		}

		const auto windowEnd = std::chrono::steady_clock::now();
		const auto windowNs = std::chrono::duration_cast<std::chrono::nanoseconds>(windowEnd - windowStart).count();
		if (windowNs <= 0) {
			//  Часы не сдвинулись — пропускаем оценку, иначе деление на ноль.
			count = 0;
			windowStart = windowEnd;
			continue;
		}

		const auto totalRender = _totalRenderNs.load(std::memory_order_relaxed);
		const float rate = static_cast<float>(totalRender) / static_cast<float>(windowNs);
		const auto current = _fps.load(std::memory_order_relaxed);

		std::uint32_t newTarget = current;
		if (rate < 0.5f) {
			newTarget = current + 3;
		}
		else if (rate < 0.6f) {
			newTarget = current + 2;
		}
		else if (rate < 0.7f) {
			newTarget = current + 1;
		}
		else if (rate > 0.98f) {
			newTarget = (current >= 33u) ? current - 3 : 30u;
		}
		else if (rate > 0.95f) {
			newTarget = (current >= 32u) ? current - 2 : 30u;
		}
		else if (rate > 0.9f) {
			newTarget = (current >= 31u) ? current - 1 : 30u;
		}

		//  Cap = user-target (то, что выставил пользователь через ini/UI).
		//  Адаптация может снижать _fps под нагрузкой, но не выше потолка
		//  и не ниже исторического минимума 30 (под него настроены анимации/физика).
		const std::uint32_t cap = _targetFps.load(std::memory_order_relaxed);
		if (newTarget > cap) {
			newTarget = cap;
		}
		else if (newTarget < 30u) {
			newTarget = 30u;
		}

		_refreshFps.store(newTarget, std::memory_order_relaxed);

		count = 0;
		windowStart = windowEnd;
		_totalRenderNs.store(0, std::memory_order_relaxed);
	}
}

} // namespace Corsairs::Client::Frame
