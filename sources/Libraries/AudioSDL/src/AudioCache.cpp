#include "AudioCache.h"


namespace Corsairs::Client::Audio {
	std::uint32_t AudioCache::Hash(std::string_view resource) {
		//  std::hash возвращает size_t — достаточно 32 бит для наших ~сотен семплов.
		//  Схлопываем 64-битный hash в 32 бита XOR'ом half-ов.
		const auto h = std::hash<std::string_view>{}(resource);
		if constexpr (sizeof(h) == 8) {
			return static_cast<std::uint32_t>(h) ^ static_cast<std::uint32_t>(h >> 32);
		} else {
			return static_cast<std::uint32_t>(h);
		}
	}

	std::uint32_t AudioCache::GetResourceId(std::string_view resource, AudioType type, MIX_Mixer* mixer) {
		if (!mixer || resource.empty()) {
			return 0;
		}

		const std::uint32_t code = Hash(resource);

		std::scoped_lock lock(_mutex);

		//  Уже в кэше — просто возвращаем id.
		if (auto it = _queue.find(code); it != _queue.end()) {
			return it->second->Code;
		}

		//  Подгружаем.
		auto info   = std::make_unique<AudioInfo>();
		info->Type  = type;
		info->Code  = code;
		info->Audio = std::make_unique<WrapperSDLAudio>(type == AudioType::Stream);

		if (!info->Audio->Init(resource, mixer)) {
			return 0;
		}

		_queue.emplace(code, std::move(info));
		return code;
	}

	AudioInfo* AudioCache::GetResource(std::uint32_t id) {
		std::scoped_lock lock(_mutex);

		if (auto it = _queue.find(id); it != _queue.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	void AudioCache::Release() {
		std::scoped_lock lock(_mutex);
		_queue.clear();
	}
} // namespace Corsairs::Client::Audio
