#include "WrapperSDLAudio.h"

#include <string>


namespace Corsairs::Client::Audio {
	bool WrapperSDLAudio::Init(std::string_view filename, MIX_Mixer* mixer) {
		Release();
		if (!mixer || filename.empty()) {
			return false;
		}
		//  MIX_LoadAudio требует null-terminated путь.
		const std::string path{filename};
		//  predecode=!_stream: эффекты в памяти, музыка — стримом.
		_audio = MIX_LoadAudio(mixer, path.c_str(), !_stream);
		if (!_audio) {
			return false;
		}
		_track = MIX_CreateTrack(mixer);
		if (!_track) {
			MIX_DestroyAudio(_audio);
			_audio = nullptr;
			return false;
		}
		if (!MIX_SetTrackAudio(_track, _audio)) {
			MIX_DestroyTrack(_track);
			MIX_DestroyAudio(_audio);
			_track = nullptr;
			_audio = nullptr;
			return false;
		}
		return true;
	}

	void WrapperSDLAudio::Release() {
		if (_track) {
			MIX_DestroyTrack(_track);
			_track = nullptr;
		}
		if (_audio) {
			MIX_DestroyAudio(_audio);
			_audio = nullptr;
		}
	}

	bool WrapperSDLAudio::Play(bool loop) {
		if (!_track) {
			return false;
		}
		SDL_PropertiesID props = SDL_CreateProperties();
		if (0 == props) {
			return false;
		}
		SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, loop ? -1 : 0);
		const bool ok = MIX_PlayTrack(_track, props);
		SDL_DestroyProperties(props);
		return ok;
	}

	bool WrapperSDLAudio::Stop() {
		if (!_track) {
			return true;
		}
		MIX_StopTrack(_track, 0);
		return true;
	}

	bool WrapperSDLAudio::IsStopped() const {
		return !_track || (!MIX_TrackPlaying(_track) && !MIX_TrackPaused(_track));
	}

	void WrapperSDLAudio::SetVolume(float vol) {
		if (!_track) {
			return;
		}
		if (vol < 0.0f) {
			vol = 0.0f;
		}
		else if (vol > 1.0f) {
			vol = 1.0f;
		}
		MIX_SetTrackGain(_track, vol);
	}
} // namespace Corsairs::Client::Audio
