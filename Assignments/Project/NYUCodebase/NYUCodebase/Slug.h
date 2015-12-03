#ifndef SLUG_HEADER
#define SLUG_HEADER
#include "Enemy.h"
#include "Player.h"

//Only enemy in the game. I would like to move most of the stuff that is in enemy into this class, but 
//have been having some trouble getting enemy to work as an abstract class
class Slug : public Enemy{
public:
	Slug(){}
	Slug(float size, float x_position, float y_position, int texture, ShaderProgram* program);
	void update(float time_elapsed, Level* level){
		if (hp <= 0 && !dead) { die(); }
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
			if (player_x < position.get_x()) {
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
		if (abs(velocity.get_x()) < minimum_velocity){ current_frame = 0; }
		else if (animation_counter > animation_time){
			animation_counter = 0;
			current_frame++;
			if (current_frame > walk_end){ current_frame = walk_begin; }
		}
		if (dead){ current_frame = death_frame; }
		sprite = frames[current_frame];
	}
private:
};
#endif