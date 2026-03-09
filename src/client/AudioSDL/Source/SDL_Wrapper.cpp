#include "SDL_Wrapper.h"


bool SDL_Sound::init(const char* filename) {
	release();
	m_device = Mix_LoadWAV(filename);;
	return 0 != m_device;
}

bool SDL_Sound::release() {
	if (m_device) {
		Mix_FreeChunk(m_device);
		m_device = 0;
	}
	return true;
}

bool SDL_Sound::play() {
	m_channel = Mix_PlayChannel(-1, m_device, m_loop);
	return (-1 != m_channel);
}

bool SDL_Sound::fadeIn(int ms) {
	m_channel = Mix_FadeInChannel(-1, m_device, m_loop, ms);
	return (-1 != m_channel);
}

bool SDL_Sound::fadeOut(int ms) {
	return -1 != Mix_FadeOutChannel(m_channel, ms);
}

bool SDL_Sound::stop() {
	Mix_HaltChannel(m_channel);
	return true;
}

bool SDL_Sound::pause() {
	Mix_Pause(m_channel);
	return true;
}

bool SDL_Sound::resume() {
	Mix_Resume(m_channel);
	return true;
}

bool SDL_Sound::rewind() {
	//  unsupported
	return false;
}

bool SDL_Sound::is_playing() {
	return 1 == Mix_Playing(m_channel);
}

bool SDL_Sound::is_paused() {
	return 1 == Mix_Paused(m_channel);
}

void SDL_Sound::set_volume(int vol) {
	SDL_Audio_Base::set_volume(vol);

	Mix_VolumeChunk(m_device, m_volume);
	if (!is_stopped()) {
		Mix_Volume(m_channel, m_volume);
	}
}


bool SDL_Music::init(const char* filename) {
	release();
	m_device = Mix_LoadMUS(filename);;
	return 0 != m_device;
}

bool SDL_Music::release() {
	if (m_device) {
		Mix_FreeMusic(m_device);
		m_device = 0;
	}
	return true;
}

bool SDL_Music::play() {
	return 0 == Mix_PlayMusic(m_device, m_loop);
}

bool SDL_Music::fadeIn(int ms) {
	return 0 == Mix_FadeInMusic(m_device, m_loop, ms);
}

bool SDL_Music::fadeOut(int ms) {
	return 1 == Mix_FadeOutMusic(ms);
}

bool SDL_Music::stop() {
	Mix_HaltMusic();
	return true;
}

bool SDL_Music::pause() {
	Mix_PauseMusic();
	return true;
}

bool SDL_Music::resume() {
	Mix_ResumeMusic();
	return true;
}

bool SDL_Music::rewind() {
	Mix_RewindMusic();
	return true;
}

bool SDL_Music::is_playing() {
	return 1 == Mix_PlayingMusic();
}

bool SDL_Music::is_paused() {
	return 1 == Mix_PausedMusic();
}

void SDL_Music::set_volume(int vol) {
	SDL_Audio_Base::set_volume(vol);

	Mix_VolumeMusic(m_volume);
}
