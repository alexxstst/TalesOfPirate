#pragma once

#include "../include/AudioSDL.h"
#include "WrapperSDLAudio.h"

#include <cstdint>
#include <memory>
#include <mutex>
#include <string_view>
#include <unordered_map>


namespace Corsairs::Client::Audio {
	struct AudioInfo {
		AudioType                 Type;
		std::uint32_t             Code;
		std::unique_ptr<WrapperSDLAudio> Audio;
	};


	//  Кэш загруженных аудио-ресурсов по хэшу имени файла.
	//  Не singleton — принадлежит AudioSDL::Impl.
	class AudioCache {
	public:
		AudioCache()  = default;
		~AudioCache() = default;

		AudioCache(const AudioCache&) = delete;
		AudioCache& operator=(const AudioCache&) = delete;

		std::uint32_t GetResourceId(std::string_view resource, AudioType type, MIX_Mixer* mixer);
		AudioInfo*    GetResource(std::uint32_t id);

		void Release();

	private:
		static std::uint32_t Hash(std::string_view resource);

		std::unordered_map<std::uint32_t, std::unique_ptr<AudioInfo>> _queue;
		std::mutex                                                    _mutex;
	};
} // namespace Corsairs::Client::Audio
