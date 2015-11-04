#pragma once 

//#include "Sprite.h"
#include "Player.h"
#include <SDL_mixer.h>

class Enemy : public Sprite{
protected:


public:
	
	void die(){
		Mix_PlayChannel(-1, death_sound, 0);
		current_frame = death_frame;
		dead = true;
		width = 0.0f;
		height = 0.0f;
	}
	void get_behavior(float player_x) {
		if (player_x < x) {
			this->move_left();
			this->mirrored = !this->mirrored;
		}
		else{
			this->move_right();
			this->mirrored = !this->mirrored;
		}
	}

};

