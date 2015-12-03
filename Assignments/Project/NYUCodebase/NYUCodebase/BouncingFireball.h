#include "Projectile.h"
#include "LightManager.h"

class BouncingFireball :public Projectile{
public:
	void check_for_shots(Entity* entity)override{
		if (!exploding && entity->is_shot(this)){
			impacts++;
			velocity.set_x(-velocity.get_x());
			velocity.set_y(-velocity.get_y());
			entity->set_hp(entity->get_hp() - damage);
		}
	}
	BouncingFireball(Vector initial_position, Vector initial_velocity, Vector acceleration, 
		ParticleEmitter* emitter, int damage, LightManager* light_manager){
		light = new Light();
		light->position = initial_position;
		light->tint.r = 1.0;
		light->tint.g = 0.0;
		light->tint.b = .66;
		light_manager->accept_light(light);
		position = initial_position;
		velocity = initial_velocity;
		this->emitter = emitter;
		exploding_lifetime = .3f;
		max_impacts = 10;
		this->damage = damage;
		this->acceleration = acceleration;
		max_velocity = 10.0f;
		diameter = .1f;
		max_lifetime = 5.0f;
	}

	void update(float time_elapsed, Level* level) override{
		lifetime += time_elapsed;
		make_adjustments();
		if (!exploding){
			calculate_x_terrain_collisions(level);
			calculate_y_terrain_collisions(level);
			if (velocity.get_hypotenuse() < max_velocity){
				velocity.set_x(velocity.get_x() + acceleration.get_x() * time_elapsed);
				velocity.set_y(velocity.get_y() + (acceleration.get_y() * time_elapsed) - (gravity*time_elapsed));
			}
			position.set_x(position.get_x() + velocity.get_x() * time_elapsed);
			position.set_y(position.get_y() + velocity.get_y() * time_elapsed);
			emitter->update_direction(velocity);
		}
		emitter->update(time_elapsed);
		light->position.set_x(position.get_x());
		light->position.set_y(position.get_y());
	}
private:
	void calculate_y_terrain_collisions(Level* level)override{
		int tile_x = this->get_x_tile_position(level->get_tilesize());
		int tile_y = this->get_y_tile_position(level->get_tilesize());
		TerrainTile lower_tile = level->get_tile(tile_x, tile_y - 1);
		TerrainTile upper_tile = level->get_tile(tile_x, tile_y + 1);
		TerrainTile this_tile = level->get_tile(tile_x, tile_y);
		if (this_tile.is_there() && this_tile.is_shot(this)){
			if (!impact_made){
				velocity.set_y(-velocity.get_y());
				if (this_tile.get_destructible()){
					impacts++;
					this_tile.set_hp(this_tile.get_hp() - damage);
					level->set_tile(tile_x, tile_y, this_tile);
				}
			}
			impact_made = true;
		}
		if (upper_tile.is_there() && upper_tile.bottom_is_solid() && upper_tile.is_shot(this)){
			if (!impact_made){
				velocity.set_y(-velocity.get_y());
				if (upper_tile.get_destructible()){
					impacts++;
					upper_tile.set_hp(upper_tile.get_hp() - damage);
					level->set_tile(tile_x, tile_y + 1, upper_tile);
				}
			}
			impact_made = true;
		}
		if (lower_tile.is_there() && lower_tile.top_is_solid() && lower_tile.is_shot(this)){
			if (!impact_made){
				velocity.set_y(-velocity.get_y());
				if (lower_tile.get_destructible()){
					impacts++;
					lower_tile.set_hp(lower_tile.get_hp() - damage);
					level->set_tile(tile_x, tile_y - 1, lower_tile);
				}
			}
			impact_made = true;
		}
	}

	void calculate_x_terrain_collisions(Level* level) override{
		int tile_x = this->get_x_tile_position(level->get_tilesize());
		int tile_y = this->get_y_tile_position(level->get_tilesize());
		TerrainTile left_tile = level->get_tile(tile_x - 1, tile_y);
		TerrainTile right_tile = level->get_tile(tile_x + 1, tile_y);
		TerrainTile this_tile = level->get_tile(tile_x, tile_y);

		if (this_tile.is_there() && this_tile.is_shot(this)){
			if (!impact_made){
				velocity.set_x(-velocity.get_x());
				if (this_tile.get_destructible()){
					impacts++;
					this_tile.set_hp(this_tile.get_hp() - damage);
					level->set_tile(tile_x, tile_y, this_tile);
				}
			}
			impact_made = true;

		}
		if (left_tile.is_there() && left_tile.right_is_solid() && left_tile.is_shot(this)){
			if (!impact_made){
				velocity.set_x(-velocity.get_x());
				if (left_tile.get_destructible()){
					impacts++;
					left_tile.set_hp(left_tile.get_hp() - damage);
					level->set_tile(tile_x - 1, tile_y, left_tile);
				}
			}
			impact_made = true;
		}
		if (right_tile.is_there() && right_tile.left_is_solid() && right_tile.is_shot(this)){
			if (!impact_made){
				velocity.set_x(-velocity.get_x());
				if (right_tile.get_destructible()){
					impacts++;
					right_tile.set_hp(right_tile.get_hp() - damage);
					level->set_tile(tile_x + 1, tile_y, right_tile);
				}
			}
			impact_made = true;
		}
	}

	float gravity = 10.0f;
	Light* light;
};