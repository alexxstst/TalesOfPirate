#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <stop_token>
#include <thread>

namespace Corsairs::Client::Frame {

//  Frame pacer клиента: фоновый поток-метроном раз в _sleepIntervalMs мс
//  поднимает счётчик _pendingTicks; главный поток в каждой итерации
//  message-loop'а проверяет Run(), и если флаг есть — отрабатывает FrameMove + Render.
//  После рендера End() копит длительность кадра, фоновый поток раз в секунду
//  адаптивно пересчитывает целевой FPS (_refreshFps) с клампом 8..30 или 8..60.
//  Применение _refreshFps происходит в Run() — каждый кадр проверяется,
//  изменился ли target, и если да — _sleepIntervalMs пересчитывается.
class SteadyFrameSync {
public:
	static SteadyFrameSync& Instance();

	SteadyFrameSync(const SteadyFrameSync&) = delete;
	SteadyFrameSync& operator=(const SteadyFrameSync&) = delete;

	bool Init();
	void Exit();

	bool Run();
	void End();
	std::uint32_t GetTick() const;

	std::uint32_t GetFps() const;

	//  Целевой FPS пользователя (из ini/UI). Адаптивная логика не поднимает
	//  GetFps() выше этого значения и не опускает ниже 30.
	std::uint32_t GetTargetFps() const;

	//  Установить целевой FPS пользователя (через ini/UI/консольную команду).
	//  Сохраняется как потолок для адаптивной логики; реальный _fps может временно
	//  падать ниже при просадке производительности, но не ниже 30.
	void SetFps(std::uint32_t fps);

	//  Множитель «во сколько раз быстрее текущий FPS относительно референсного 30».
	//  Изначально весь движок (анимации, скорости движения, IsGlitched-логика)
	//  настраивался под 30 FPS — этот множитель компенсирует частоту вызовов
	//  FrameMove/Render для произвольного FPS:
	//    velocity *= mult       — нормализация анимации к 30-FPS-видимой скорости;
	//    speed    *= mult       — компенсация скорости движения (60 FPS зовёт frame в 2 раза чаще,
	//                             значит за тот же game-time нужно сделать в 2 раза большие шаги).
	float GetAnimMultiplier() const;

	//  Применить отложенный _refreshFps если адаптивная логика его изменила.
	//  Вызывается автоматически из Run() — внешние колсайты не нужны, оставлен
	//  публичным для совместимости со старыми точками вызова.
	void RefreshFps();

	bool IsFramerate60() const;
	void SetFramerate60(bool v);

private:
	SteadyFrameSync() = default;
	~SteadyFrameSync() = default;

	void _SleepLoop(std::stop_token stop);

	//  Внутренний апдейт _fps/_sleepIntervalMs без изменения _targetFps —
	//  используется RefreshFps() для применения адаптивной коррекции.
	void _UpdateCurrentFps(std::uint32_t fps);

	//  Целевой FPS — то значение, которое выставил пользователь (через ini/UI).
	//  Адаптивная логика временно снижает реальный _fps при перегрузке, но
	//  не выше _targetFps как потолка и не ниже 30 как пола.
	std::atomic<std::uint32_t> _targetFps{30};

	//  Текущий FPS и производный sleep-интервал. Пишутся главным потоком
	//  (SetFps / RefreshFps), читаются фоновым (Sleep). std::atomic — корректная
	//  синхронизация без UB; стоимость на x86/x64 фактически нулевая для uint32.
	std::atomic<std::uint32_t> _fps{30};
	std::atomic<std::uint32_t> _sleepIntervalMs{33};
	std::atomic<std::uint32_t> _refreshFps{30};

	//  Момент тика для внешнего API (DWORD-timestamp от GetTickCount):
	//  передаётся в FrameMove(DWORD), менять контракт нельзя.
	std::atomic<std::uint32_t> _currentTickMs{0};

	//  Высокоточный момент начала кадра (steady_clock в наносекундах).
	//  Используется внутренне для замера длительности — раньше делали через
	//  GetTickCount, у которого резолюция 15.6 мс, и кадры 3-5 мс
	//  вообще не учитывались (rate выходил 0, адаптация ломалась).
	std::atomic<std::int64_t> _frameStartNs{0};
	std::atomic<std::int64_t> _totalRenderNs{0};

	//  Сигнал «пора рендерить»: фоновый поток инкрементит, главный сбрасывает в 0.
	std::atomic<std::int32_t> _pendingTicks{0};

	bool _isTimerPeriodSet = false;

	//  std::jthread сам отдаёт stop_token в callable и автоматически делает
	//  request_stop + join в деструкторе — заменяет связку HANDLE + _isRunning + WaitForSingleObject + CloseHandle.
	std::jthread _thread;
};

} // namespace Corsairs::Client::Frame
