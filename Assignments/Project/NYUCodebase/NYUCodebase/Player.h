#pragma once
#include "Sprite.h"
#include "Enemy.h"
#include "ProjectileManager.h"
#include "LevelModifier.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

// Purpose: Handle player-specific movements and animation decisions

class Player : public Sprite{
public:

	Player(){};
	Player(float size, float x_position, float y_position, int texture, ShaderProgram* program);
	void calculate_enemy_collision(Enemy* enemy);
	void jump(){
		if (transformed){ acceleration.set_y(bat_acceleration); }
		else if (bottom_flag){
			//special_effects->jumpTest(x, y);
			Sprite::jump();
		}
	}
	void down_key() {
		if (transformed){ acceleration.set_y(-2 * bat_acceleration); }
	}
	void shoot(){
		if (!transformed && current_mana >= 2){
			if (shoot_timer > shoot_spacing){
				current_mana-=2;
				audio->shootSound();
				shoot_timer = 0.0f;
				projectile_manager->shoot_fireball(position, mirrored);
				//projectile_manager->shoot_bouncing_fireball(position, mirrored);
			}
		}
	}
	void shoot_bouncy(){
		if (!transformed && current_mana >= 4){
			if (shoot_timer > shoot_spacing){
				current_mana-=4;
				audio->shootSound();
				shoot_timer = 0.0f;
				//projectile_manager->shoot_fireball(position, mirrored);
				projectile_manager->shoot_bouncing_fireball(position, mirrored);
			}
		}
	}
	void create_tile(){
		if (!transformed && current_mana >= 2){
			bool success;
			if (mirrored){
				success=level_modifier.add_tile(this->get_x_tile_position(level_modifier.get_tilesize()) - 1,
					this->get_y_tile_position(level_modifier.get_tilesize()));
			}
			else{
				success=level_modifier.add_tile(this->get_x_tile_position(level_modifier.get_tilesize()) + 1,
					this->get_y_tile_position(level_modifier.get_tilesize()));
			}
			if (success){ current_mana -= 2; }
		}
	}

	void hurt_from_tile(int damage) {
		acceleration.set_y(0);
		current_health -= damage;
		audio->hurtSound();
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
	// getters+setters
	Sheetposition return_sheet_position();
	float get_x();
	float get_y();
	Matrix get_matrix();
	void decide_frame();
	void update(float time_elapsed, Level* level);
	void set_level_modifier(Level* level){ level_modifier = LevelModifier(level); }
	int get_current_health(){ return current_health; }
	int get_max_health(){ return max_health; }
	int get_current_mana(){ return current_mana; }
	int get_max_mana() { return max_mana; }
	void Draw(){
		render_count++;
		if (render_count == 1000){
			DBOUT(update_count);
			render_count = 0;
			update_count = 0;
		}
		Entity::Draw();
	}

	void Transform();
	void touched_win_tile() { win = true; }
	bool win_status() { return win; }
	void reset_win() { 
		mana_refill_rate = 1.0;
		win = false; 
		current_health = max_health;
	}
protected:
	int max_health, current_health, max_mana, current_mana;
	float size_storage, width_storage, height_storage;
	float hurt_jump = 5.0;
	float bat_acceleration = 2.0f;
	int s = 1;
	bool transformed = false;
	float time_since_transformed = 0.0f;
	float min_transform_wait = 3.0f;
	float transform_timer = 0.0f;
	float transform_time_limit = 10.0f;
	int update_count = 0;
	int render_count = 0;
	int walk_begin, walk_end, hurt_frame;
	bool hurt;
	float hurt_counter = 0;
	float hurt_time;
	float shoot_timer = 0.0f;
	float shoot_spacing = .25f;
	LevelModifier level_modifier;
	bool win = false;
	float health_drain_rate = 1.0, health_drain_counter = 0.0;
	float mana_refill_rate = 1.0, mana_refill_counter = 0.0;
};