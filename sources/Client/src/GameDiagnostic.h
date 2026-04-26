#pragma once

//  Централизованные тоглы диагностических каналов клиентской (gameplay)
//  части проекта. Аналог Engine'овского EngineDiag, но для game-кода:
//  движение персонажа, путь поиска, AI, действия, состояния и т.п.
//
//  Включается клиентом после GlobalAppConfig.Load() из соответствующих
//  ini-флагов в секции [Logging] system.ini. До этого все тоглы false —
//  никаких накладных в hot-path.

namespace Corsairs::Client::Diagnostic {
	class GameDiagnostic {
	public:
		static GameDiagnostic& Instance();

		GameDiagnostic(const GameDiagnostic&) = delete;
		GameDiagnostic& operator=(const GameDiagnostic&) = delete;

		//  [Logging] move — диагностика ходьбы главного героя
		//  (канал "movie"): pos, target, dis, Tick, FPS на каждый setPos.
		//  Срабатывает каждый кадр при движении — может быть очень шумным,
		//  включать только под отладку конкретной механики передвижения.
		void SetMoveEnabled(bool enabled) {
			_move = enabled;
		}

		bool IsMoveEnabled() const {
			return _move;
		}

	private:
		GameDiagnostic() = default;
		~GameDiagnostic() = default;

		bool _move{false};
	};
} // namespace Corsairs::Client::Diagnostic
