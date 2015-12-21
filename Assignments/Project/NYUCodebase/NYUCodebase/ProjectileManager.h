#pragma once 
#include "Fireball.h"
#include "BouncingFireball.h"
#include <vector>
#include "SheetSprite.h"
#include "SpecialEffects.h"
#include "LightManager.h"
//class Level;
#include "Level.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

class ProjectileManager{
private:
	SheetSprite spritesheet;
	SpecialEffects* special_effects;
	ParticleEmitter* emitter;
	std::vector<Projectile*> player_projectiles;
	float effects_counter = 0.0f;
	float effects_time = .3f;
	LightManager* light_manager;
public:
	~ProjectileManager(){
		for (int i = 0; i < player_projectiles.size(); i++){
			delete player_projectiles[i];
			player_projectiles.erase(player_projectiles.begin() + i);
		}
	}
	void set_light_manager(LightManager* light_manager){ this->light_manager = light_manager; }
	ProjectileManager(SpecialEffects* special_effects){
		this->special_effects = special_effects;
	}
	enum type {FIREBALL, BLOB_SPIT};
	void shoot_fireball(Vector position, bool facing_left){
		Vector velocity, acceleration;
		float flip = 1.0;
		if (facing_left == true){ flip = -1.0; }
		velocity.set_x(5.0 * flip);
		velocity.set_y(0.0f);
		acceleration.set_y(0.0f);
		acceleration.set_x(.5*flip);
		Color start_color, end_color, color_deviation;
		start_color.r = 1.0;
		start_color.g = 1.0;
		end_color.r = 1.0;
		end_color.g = .65;
		color_deviation.r = .3;
		color_deviation.g = .3;
		color_deviation.a = 0.0;
		emitter = special_effects->fireball_trail(position, flip, start_color, end_color, color_deviation);
		
		player_projectiles.push_back(new Fireball(position, velocity, acceleration, emitter, 5, light_manager));
	}

	void shoot_bouncing_fireball(Vector position, bool facing_left){
		Vector velocity, acceleration;
		float flip = 1.0;
		if (facing_left == true){ flip = -1.0; }
		velocity.set_x(2.0 * flip);
		velocity.set_y(3.0f);
		acceleration.set_y(0.0f);
		acceleration.set_x(.5*flip);
		Color start_color, end_color, color_deviation;
		start_color.r = 1.0;
		start_color.b = 1.0;
		end_color.r = 0.0;
		end_color.b = .65;
		color_deviation.r = .3;
		color_deviation.b = .3;
		color_deviation.a = 0.0;
		flip = 1.0;
		emitter = special_effects->fireball_trail(position, flip, start_color, end_color, color_deviation);
	
		player_projectiles.push_back(new BouncingFireball(position, velocity, acceleration, emitter, 5, light_manager));

	}

	void update(float time_elapsed, Level* level){
		effects_counter += time_elapsed;
		for (int i = 0; i < player_projectiles.size(); i++){
			player_projectiles[i]->update(time_elapsed, level);
			if (player_projectiles[i]->expired()){
				delete player_projectiles[i];
				player_projectiles.erase(player_projectiles.begin() + i);
			}
		}
	}

	void check_shot_enemies(Entity* entity){
		for (int i = 0; i < player_projectiles.size(); i++){
			player_projectiles[i]->check_for_shots(entity);
		}
	}

	void render() {
		for (int i = 0; i < player_projectiles.size(); i++){
			player_projectiles[i]->render();
		}
	}
	
	void clear_active_projectiles(){
		for (int i = 0; i < player_projectiles.size(); i++){
			delete player_projectiles[i];
			player_projectiles.erase(player_projectiles.begin() + i);
		}
	}
};