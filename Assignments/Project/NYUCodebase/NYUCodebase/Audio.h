#pragma once
#include <SDL_mixer.h>

// class to handle all audio
class Audio{
public:
	/*~Audio(){
		Mix_FreeMusic(music);
		Mix_FreeChunk(death_sound);
		Mix_FreeChunk(jump_sound);
		Mix_FreeChunk(hurt_sound);
		SDL_AudioQuit();
	}*/
	
	Audio(){
		music = Mix_LoadMUS("../audio/BDDStage1GM.mid");
		jump_sound = Mix_LoadWAV("../audio/jump.wav");
		hurt_sound = Mix_LoadWAV("../audio/hurt.wav");
		death_sound = Mix_LoadWAV("../audio/enemy_death.wav");
	}

	void hurtSound(){ Mix_PlayChannel(-1, hurt_sound, 0); }
	void dieSound(){ Mix_PlayChannel(-1, death_sound, 0); }
	void jumpSound(){ Mix_PlayChannel(-1, jump_sound, 0); }
	void playMusic(){ Mix_PlayMusic(music, -1); }

private:
	Mix_Chunk *jump_sound, *hurt_sound, *death_sound;
	Mix_Music *music;
};