#ifndef SLUG_HEADER
#define SLUG_HEADER
#include "Enemy.h"
#include "Player.h"

//Only enemy in the game. I would like to move most of the stuff that is in enemy into this class, but 
//have been having some trouble getting enemy to work as an abstract class
class Slug : public Enemy{
public:
	~Slug(){
		if (light != nullptr){ light->turn_off(); }
		delete[] frames;
	}
	Slug(){}
	void hurt_from_tile(int damage) {
		acceleration.set_y(0);
		hp -= damage;
		if (hurt_from_top){
			hurt_from_top = false;
			velocity.set_y(-hurt_jump);
		}
		else if (hurt_from_bottom){
			hurt_from_bottom = false;
			velocity.set_y(hurt_jump);
		}
		else if (hurt_from_right){
			hurt_from_right = false;
			velocity.set_x(-hurt_jump);
		}
		else if (hurt_from_left){
			hurt_from_left = false;
			velocity.set_x(hurt_jump);
		}
	}
	Slug(float size, float x_position, float y_position, int texture, 
		ShaderProgram* program, float color_r, float color_g, float color_b, LightManager* light_manager);
	void update(float time_elapsed, Level* level){
		if (hp <= 0 && !dead) { die(); }
		decide_frame();
		if (dead){ 
			Entity::UpdateY(time_elapsed); 
			death_counter += time_elapsed;
			if (death_counter >= death_time){
				expired = true;
			}
		}
		else { Sprite::update(time_elapsed, level); }
	}
	void die(){
		light->turn_inactive();
		audio->dieSound();
		current_frame = death_frame;
		dead = true;
		width = 0.0f;
		height = 0.0f;
	}
	void get_behavior(float player_x, float player_y) {
		if (!dead){
			int tile_x = this->get_x_tile_position(level->get_tilesize());
			int tile_y = this->get_y_tile_position(level->get_tilesize());
			if (moving_right){
				if (level->get_tile(tile_x + 1, tile_y - 1).damage_top() || !level->get_tile(tile_x + 1, tile_y - 1).is_there()
					|| level->get_tile(tile_x + 1, tile_y).is_there()){
					moving_right = false;
					moving_left = true;
				}
			}
			else if (moving_left) {
				if (level->get_tile(tile_x - 1, tile_y - 1).damage_top() || !level->get_tile(tile_x - 1, tile_y - 1).is_there()
					|| level->get_tile(tile_x - 1, tile_y).is_there()){
					moving_right = true;
					moving_left = false;
				}
			}
			if (moving_right) {
				this->move_right(); 
				mirrored = true;
			}
			else if (moving_left) { 
				this->move_left();
				mirrored = false;
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
		if (dead){ 
			current_frame = death_frame;
		}
		sprite = frames[current_frame];
	}
	void Draw(){
		light->position.set_x(this->position.get_x());
		light->position.set_y(this->position.get_y());
		Entity::Draw();
	}
private:
	float death_counter = 0.0f;
	float death_time = 5.0f;
	Light* light;
	bool moving_left = false;
	bool moving_right = true;
	float hurt_jump = 1.0;
};
#endif