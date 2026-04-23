#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <string_view>


namespace Corsairs::Client::Audio {
	//  Обёртка вокруг пары MIX_Audio + MIX_Track. Один класс для WAV-эффектов и
	//  стриминговой музыки — отличаются только режимом загрузки (predecode).
	class WrapperSDLAudio {
	public:
		//  stream=false — короткий звук (predecode в память, быстрый старт).
		//  stream=true  — большой файл, декодируется на лету.
		explicit WrapperSDLAudio(bool stream)
			: _stream(stream) {}

		~WrapperSDLAudio() {
			Release();
		}

		WrapperSDLAudio(const WrapperSDLAudio&) = delete;
		WrapperSDLAudio& operator=(const WrapperSDLAudio&) = delete;

		bool Init(std::string_view filename, MIX_Mixer* mixer);
		void Release();

		bool Play(bool loop);
		bool Stop();
		bool IsStopped() const;

		//  vol ∈ [0.0f, 1.0f].
		void SetVolume(float vol);

	private:
		MIX_Audio* _audio = nullptr;
		MIX_Track* _track = nullptr;
		bool       _stream = false;
	};
} // namespace Corsairs::Client::Audio
