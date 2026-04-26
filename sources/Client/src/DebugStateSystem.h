#pragma once

// DebugStateSystem — централизованное хранилище отладочных оверлей-сообщений.
//
// Заменяет legacy-связку из MPRender:
//   MPRender::Print(category, x, y, fmt, ...)   — запись по координате
//   MPRender::EnablePrint(category, bool)        — toggle категории
//   MPRender::ClearPrint(category)               — сброс
//   MPRender::_InfoIdx[type]                     — std::map<int, std::string>
//   MPRender::_szInfo[512]                       — общий vsnprintf-буфер
//   MPRender::_PrintInfoMask[]                   — массив enabled-флагов
//
// Зачем выделили в отдельный класс:
//   - Print жил в Engine, но реальный рендер overlay — обязанность клиента
//     (нужен FontManager / ui::Render). Класс собран в Client/src.
//   - vsnprintf в общий буфер ушёл вместе с variadic-API; сейчас используется
//     std::format (см. SetFmt).
//   - Категории — это enum class (раньше был plain enum INFO_TYPE).
//   - Каллер не должен знать про массивы и индексы; API: Set / Clear / ForEach.
//
// Использование:
//   auto& dbg = DebugStateSystem::Instance();
//   dbg.SetEnabled(DebugStateSystem::Category::Fps, true);
//   dbg.SetFmt(DebugStateSystem::Category::Fps, 5, 5, "FPS: {}", fps);
//
//   // Очистка фрейм-категорий — раз за кадр перед записью:
//   dbg.Clear(DebugStateSystem::Category::Debug);
//
//   // Рендер (в WorldScene/CGameApp render-pass):
//   dbg.ForEachVisible([](const auto& e) {
//       ui::Render(e.text, e.x, e.y, 0xFFFFFF00);
//   });

#include <array>
#include <cstdint>
#include <format>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <utility>

class DebugStateSystem {
public:
	enum class Category : std::uint32_t {
		Debug = 0,        // Произвольная отладка (editor, координаты, углы).
		Performance,      // Метрики: frame time, render time.
		Console,          // Echo команд debug-консоли.
		Fps,              // FPS-counter.
		Game,             // In-game debug (имена объектов, ID и т.п.).
		Count_,
	};

	DebugStateSystem(const DebugStateSystem&) = delete;
	DebugStateSystem& operator=(const DebugStateSystem&) = delete;

	static DebugStateSystem& Instance();

	// Записать сообщение по координатам экрана (x,y) в категорию.
	// Перезаписывает прошлое значение в той же позиции (как legacy _InfoIdx).
	void Set(Category cat, int x, int y, std::string_view text);

	// Удобная обёртка с std::format. Литерал/runtime-format обрабатывается
	// std::format_string<Args...>; ошибки формата ловятся компилятором.
	template <typename... Args>
	void SetFmt(Category cat, int x, int y,
				std::format_string<Args...> fmt, Args&&... args) {
		Set(cat, x, y, std::format(fmt, std::forward<Args>(args)...));
	}

	// Toggle категории. Невидимые категории не накапливают рендер-вызовы
	// (см. ForEachVisible). Set/SetFmt работают независимо от флага — это
	// удобно для записи метрик в фоне, чтобы потом включить overlay.
	void SetEnabled(Category cat, bool enabled);
	bool IsEnabled(Category cat) const;

	// Сброс. -Clear(cat) очищает одну категорию; ClearAll — все.
	// Frame-life категории (Debug, Performance) типично сбрасываются раз за кадр
	// перед записью.
	void Clear(Category cat);
	void ClearAll();

	struct Entry {
		int x{};
		int y{};
		std::string text;
	};

	// Обход всех записей включённых категорий — используется overlay-рендером.
	// visitor вызывается для каждого Entry; порядок — по возрастанию ключа
	// (y * kStride + x), как в legacy _InfoIdx.
	void ForEachVisible(const std::function<void(const Entry&)>& visitor) const;

	// Обход одной категории (включена она или нет).
	void ForEach(Category cat, const std::function<void(const Entry&)>& visitor) const;

private:
	DebugStateSystem();

	// Ключ карты — y * kStride + x. Совпадает с legacy MPRender::Print.
	static constexpr int kStride = 2000;
	static int MakeKey(int x, int y) { return y * kStride + x; }

	struct CategoryState {
		bool enabled = false;
		std::map<int, Entry> entries;
	};

	std::array<CategoryState, static_cast<std::size_t>(Category::Count_)> _categories;
};
