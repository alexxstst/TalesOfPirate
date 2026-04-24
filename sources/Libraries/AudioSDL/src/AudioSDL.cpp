#include "../include/AudioSDL.h"
#include "AudioCache.h"
#include "WrapperSDLAudio.h"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>


namespace Corsairs::Client::Audio {
	struct AudioSDL::Impl {
		MIX_Mixer* Mixer = nullptr;
		bool IsValid = false;
		AudioCache Cache;
	};

	AudioSDL::AudioSDL()
		: _impl(std::make_unique<Impl>()) {
	}

	AudioSDL::~AudioSDL() = default;

	AudioSDL& AudioSDL::Instance() {
		static AudioSDL instance;
		return instance;
	}

	bool AudioSDL::Init() {
		if (!SDL_Init(SDL_INIT_AUDIO)) {
			return false;
		}
		if (!MIX_Init()) {
			SDL_Quit();
			return false;
		}
		//  nullptr spec — mixer подхватит формат устройства по умолчанию.
		_impl->Mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
		if (!_impl->Mixer) {
			MIX_Quit();
			SDL_Quit();
			return false;
		}
		_impl->IsValid = true;
		return true;
	}

	void AudioSDL::Release() {
		_impl->Cache.Release();
		if (_impl->Mixer) {
			MIX_DestroyMixer(_impl->Mixer);
			_impl->Mixer = nullptr;
		}
		MIX_Quit();
		SDL_Quit();
		_impl->IsValid = false;
	}

	bool AudioSDL::IsValid() const {
		return _impl->IsValid;
	}

	std::uint32_t AudioSDL::GetResourceId(std::string_view resource, AudioType type) {
		if (!_impl->IsValid) {
			return 0;
		}
		return _impl->Cache.GetResourceId(resource, type, _impl->Mixer);
	}

	bool AudioSDL::Play(std::uint32_t id, bool loop) {
		AudioInfo* info = _impl->Cache.GetResource(id);
		if (!info) {
			return false;
		}
		return info->Audio->Play(loop);
	}

	bool AudioSDL::Stop(std::uint32_t id) {
		AudioInfo* info = _impl->Cache.GetResource(id);
		if (!info) {
			return false;
		}
		return info->Audio->Stop();
	}

	bool AudioSDL::IsStopped(std::uint32_t id) {
		AudioInfo* info = _impl->Cache.GetResource(id);
		if (!info) {
			return false;
		}
		return info->Audio->IsStopped();
	}

	bool AudioSDL::SetVolume(std::uint32_t id, float vol) {
		AudioInfo* info = _impl->Cache.GetResource(id);
		if (!info) {
			return false;
		}
		info->Audio->SetVolume(vol);
		return true;
	}
} // namespace Corsairs::Client::Audio
