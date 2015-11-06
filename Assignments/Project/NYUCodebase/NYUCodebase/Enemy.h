#pragma once 

//#include "Sprite.h"
#include "Player.h"
#include <SDL_mixer.h>


// Mostly trying to make this into an abstract class 
//(so I can have a vector of enemies and just call for their behaviors, instead of vectors of different types of enemies)
// Having some trouble implementing this
class Enemy : public Sprite{
public:
	
	void update(float time_elapsed, Level* level){
		decide_frame();
		if (dead){ Entity::UpdateY(time_elapsed); }
		else { Sprite::update(time_elapsed, level); }
	}

	void die(){
		audio->dieSound();
		current_frame = death_frame;
		dead = true;
		width = 0.0f;
		height = 0.0f;
	}
	void get_behavior(float player_x) {
		if (!dead){
			if (player_x < x) {
				this->move_left();
				this->mirrored = !this->mirrored;
			}
			else{
				this->move_right();
				this->mirrored = !this->mirrored;
			}
		}
	}
	void decide_frame(){
		if (abs(x_velocity) < minimum_velocity){ current_frame = 0; }
		else if (animation_counter > animation_time){
			animation_counter = 0;
			current_frame++;
			if (current_frame > walk_end){ current_frame = walk_begin; }
		}
		if (dead){ current_frame = death_frame; }
		sprite = frames[current_frame];
	}

	
protected:
	bool dead;
	int death_frame;
	int walk_begin, walk_end;
};

