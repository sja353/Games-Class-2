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
		break_sound = Mix_LoadWAV("../audio/block_break.wav");
		shoot_sound = Mix_LoadWAV("../audio/shoot.wav");
		negative_sound = Mix_LoadWAV("../audio/negative.wav");
		transform_sound = Mix_LoadWAV("../audio/transform.wav");
	}
	void transformSound(){ 
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, transform_sound, 0);
		}
	}
	void breakSound(){
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, break_sound, 0);
		}
	}
	void negativeSound(){
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, negative_sound, 0);
		}
	}
	void shootSound(){
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, shoot_sound, 0);
		}
	}
	void hurtSound(){
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, hurt_sound, 0);
		}
	}
	void dieSound(){
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, death_sound, 0);
		}
	}
	void jumpSound(){
		if (breaktime_counter > min_breaktime){
			breaktime_counter = 0;
			Mix_PlayChannel(-1, jump_sound, 0);
		}
	}
	void playMusic(){ Mix_PlayMusic(music, -1); }
	void update(float time_elapsed){
		breaktime_counter += time_elapsed;
	}

private:
	Mix_Chunk *jump_sound, *hurt_sound, *death_sound, *break_sound, *shoot_sound, *negative_sound, *transform_sound;
	Mix_Music *music;
	float min_breaktime = .1f;
	float breaktime_counter = 0.0f;
};