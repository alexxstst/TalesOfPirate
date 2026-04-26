#pragma once

//  Централизованные тоглы диагностических каналов движка. Сюда стекаются
//  bool-флаги «включить подробный лог такого-то подсистемы», чтобы не плодить
//  по проекту глобальные переменные. Сами каналы (имена logger'ов через
//  ToLogService) — отдельные строки в коде вызова; этот класс лишь хранит
//  состояние «включено/выключено».
//
//  Включение делает клиент после GlobalAppConfig.Load() из соответствующих
//  ini-флагов (см. секцию [Logging] в system.ini). До этого момента все
//  тоглы false — никаких накладных в hot-path.

namespace Corsairs::Engine::Diagnostic {
	class EngineDiag {
	public:
		static EngineDiag& Instance();

		EngineDiag(const EngineDiag&) = delete;
		EngineDiag& operator=(const EngineDiag&) = delete;

		//  [Logging] streampool — диагностика lwDynamicStreamVB/IB
		//  (канал "vbstream"): события Create + параметры кольцевого буфера
		//  на каждом wrap-around (total/free_addr/free_size, branch=A|B).
		void SetStreamPoolEnabled(bool enabled) {
			_streamPool = enabled;
		}

		bool IsStreamPoolEnabled() const {
			return _streamPool;
		}

		//  [Logging] sceneload — диагностика загрузки игровой сцены
		//  (канал "scene"): CreateScene/CreateMemory/_ReadRBO с параметрами
		//  карты и числом загруженных RBO. Срабатывает 1 раз на смену карты,
		//  спама не создаёт; флаг нужен, чтобы в проде не засорять common.
		void SetSceneLoadEnabled(bool enabled) {
			_sceneLoad = enabled;
		}

		bool IsSceneLoadEnabled() const {
			return _sceneLoad;
		}

	private:
		EngineDiag() = default;
		~EngineDiag() = default;

		bool _streamPool{false};
		bool _sceneLoad{false};
	};
} // namespace Corsairs::Engine::Diagnostic
