
#pragma once 
#include "Entity.h"
#include "Sheetposition.h"
#include "Level.h"

// Base class for all movable objects in the game. Movements common to all sprites handled here
class Sprite : public Entity{	
public:
	
	//movement
	void move_right();
	void move_left();
	void jump();
	void idle(){};
	//Update and collisions
	void update(float time_elapsed, Level* level);
	void calculate_x_terrain_collisions(Level* level);
	void calculate_y_terrain_collisions(Level* level);
	
	//Getters/setters
	int get_x_tile_position(float tilesize);
	int get_y_tile_position(float tilesize);
	int get_current_frame();
	void set_y_velocity(float new_velocity){ y_velocity = new_velocity; }
	void set_x_velocity(float new_velocity){ x_velocity = new_velocity; }
	float get_y_velocity(){ return y_velocity; }
	float get_x_velocity() { return x_velocity; }
	
	//bool dead = false;

protected:
	// Collision flags
	bool top_flag, bottom_flag, left_flag, right_flag;
	
	//Animation stuff-maybe move to child classes?
	float animation_time=0; //common to all derived classes
	float animation_counter = 0; // common
	int current_frame=0; //  common
	Sheetposition* frames; // common to all derived classes
	
	float jump_velocity;
	
};

