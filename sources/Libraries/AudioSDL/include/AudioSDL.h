#pragma once

#include <cstdint>
#include <memory>
#include <string_view>


namespace Corsairs::Client::Audio {
	enum class AudioType : std::uint8_t {
		Sfx = 1,    //  Короткий эффект, предекодированный в память (WAV).
		Stream = 2, //  Стриминговая музыка (OGG/MP3).
	};


	class AudioSDL {
	public:
		static AudioSDL& Instance();

		bool Init();
		void Release();
		bool IsValid() const;

		//  Возвращает id ресурса (0 при ошибке).
		std::uint32_t GetResourceId(std::string_view resource, AudioType type);

		bool Play(std::uint32_t id, bool loop = false);
		bool Stop(std::uint32_t id);
		bool IsStopped(std::uint32_t id);

		//  Громкость в [0.0f, 1.0f].
		bool SetVolume(std::uint32_t id, float vol);

	private:
		AudioSDL();
		~AudioSDL();
		AudioSDL(const AudioSDL&) = delete;
		AudioSDL& operator=(const AudioSDL&) = delete;

		struct Impl;
		std::unique_ptr<Impl> _impl;
	};
} // namespace Corsairs::Client::Audio
