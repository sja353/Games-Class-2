
#pragma once 
#include "Entity.h"
#include "Sheetposition.h"
#include "Level.h"
#include <SDL_mixer.h>

class Sprite : public Entity{	

public:
	
	Mix_Chunk *jump_sound, *hurt_sound, *death_sound;
	void move_right();
	void move_left();
	void jump();
	void idle();
	void update(float time_elapsed, Level* level);
	void calculate_terrain_collisions(Level* level);
	int get_x_tile_position(float tilesize){
		int x_tile =( x)/ tilesize;
		return x_tile;
	}
	int get_y_tile_position(float tilesize){
		int y_tile = (y ) / tilesize;
		return y_tile;
	}
	bool dead = false;
	int get_current_frame(){ return current_frame; }

protected:
	bool top_flag, bottom_flag, left_flag, right_flag;
	float animation_time;
	float animation_counter = 0;
	float jump_time = .3;
	float jump_counter = 0;
	int current_frame=0;
	int first_animation_frame, last_animation_frame;
	int animation_pointer = 0;
	Sheetposition* frames;
	//float y_gravity_store;
	int death_frame;
	bool hurt = false;
	float hurt_counter =0;
	float hurt_time = .5f;
	int hurt_frame;
	bool jumping = false;
	float jump_velocity;
	bool jump_ended = false;
};

