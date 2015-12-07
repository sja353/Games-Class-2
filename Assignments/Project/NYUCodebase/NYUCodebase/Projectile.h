
#pragma once
#include "Vector.h"
#include "Level.h"
//#include "Entity.h"
//class Level;
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

class Projectile{
protected:
	
	Vector position;
	Vector velocity;
	Vector acceleration;
	float rotation;
	float lifetime = 0.0f;
	float exploding_lifetime;
	float max_lifetime;
	float max_velocity;
	float diameter;
	int type;
	ParticleEmitter* emitter;
	unsigned int impacts = 0;
	unsigned int max_impacts;
	bool impact_made = false;
	bool exploding = false;
	int damage;
public:
	virtual ~Projectile(){
		delete emitter;
	}
	Projectile(){}
	Projectile(Vector initial_position, Vector initial_velocity, Vector acceleration,
		float max_velocity, float diameter, float max_lifetime, int type, ParticleEmitter* emitter, int damage){
		position = initial_position;
		velocity = initial_velocity;
		this->acceleration = acceleration;
		this->max_velocity = max_velocity;
		this->diameter = diameter;
		this->type = type;
		this->max_lifetime = max_lifetime;
		this->emitter = emitter;
		this->damage = damage;
	}
	bool expired(){ return (exploding && lifetime > exploding_lifetime); }

	virtual void update(float time_elapsed, Level* level){
		DBOUT("Projectile update called");
	}

	virtual void check_for_shots(Entity* entity){
		if (!exploding && entity->is_shot(this)){
			initiate_explosion();
			entity->set_hp(entity->get_hp() - damage);
		}
	}

	void render(){
		if (impact_made){ impacts++; }
		impact_made = false;
		emitter->update_position(position);
		emitter->render();
	}

	int get_x_tile_position(float tilesize){
		int x_tile = (position.get_x()) / tilesize;
		return x_tile;
	}

	int get_y_tile_position(float tilesize){
		int y_tile = (position.get_y()) / tilesize;
		return y_tile;
	}

	virtual void calculate_y_terrain_collisions(Level* level){
		int tile_x = this->get_x_tile_position(level->get_tilesize());
		int tile_y = this->get_y_tile_position(level->get_tilesize());
		TerrainTile lower_tile = level->get_tile(tile_x, tile_y - 1);
		TerrainTile upper_tile = level->get_tile(tile_x, tile_y + 1);
		TerrainTile this_tile = level->get_tile(tile_x, tile_y);
		if (this_tile.is_there() && this_tile.is_shot(this)){
			if (this_tile.get_destructible() && !impact_made){
				impacts++;
				this_tile.set_hp(this_tile.get_hp() - damage);
				level->set_tile(tile_x, tile_y, this_tile);
			}
			impact_made = true;
		}
		if (upper_tile.is_there() && upper_tile.bottom_is_solid() && upper_tile.is_shot(this)){
			if (upper_tile.get_destructible() && !impact_made){
				impacts++;
				upper_tile.set_hp(upper_tile.get_hp() - damage);
				level->set_tile(tile_x, tile_y + 1, upper_tile);
			}
			impact_made = true;
		}
		if (lower_tile.is_there() && lower_tile.top_is_solid() && lower_tile.is_shot(this)){
			if (lower_tile.get_destructible() && !impact_made){
				impacts++;
				lower_tile.set_hp(lower_tile.get_hp() - damage);
				level->set_tile(tile_x, tile_y - 1, lower_tile);
			}
			impact_made = true;
		}
	}
	virtual void calculate_x_terrain_collisions(Level* level){
		int tile_x = this->get_x_tile_position(level->get_tilesize());
		int tile_y = this->get_y_tile_position(level->get_tilesize());
		TerrainTile left_tile = level->get_tile(tile_x - 1, tile_y);
		TerrainTile right_tile = level->get_tile(tile_x + 1, tile_y);
		TerrainTile this_tile = level->get_tile(tile_x, tile_y);

		if (this_tile.is_there() && this_tile.is_shot(this)){
			if (this_tile.get_destructible() && !impact_made){
				impacts++;
				this_tile.set_hp(this_tile.get_hp() - damage);
				level->set_tile(tile_x, tile_y, this_tile);
			}
			impact_made = true;

		}
		if (left_tile.is_there() && left_tile.right_is_solid() && left_tile.is_shot(this)){
			if (left_tile.get_destructible() && !impact_made){
				impacts++;
				left_tile.set_hp(left_tile.get_hp() - damage);
				level->set_tile(tile_x - 1, tile_y, left_tile);
			}
			impact_made = true;
		}
		if (right_tile.is_there() && right_tile.left_is_solid() && right_tile.is_shot(this)){
			if (right_tile.get_destructible() && !impact_made){
				impacts++;
				right_tile.set_hp(right_tile.get_hp() - damage);
				level->set_tile(tile_x + 1, tile_y, right_tile);
			}
			impact_made = true;
		}
	}
	void initiate_explosion(){
		exploding = true;
		lifetime = 0.0f;
		emitter->explode();
	}
	void make_adjustments(){
		if (!exploding && (impacts > max_impacts || lifetime > max_lifetime)){
			initiate_explosion();
		}
	}
	void record_impact(){ impacts++; }
	float get_x(){ return position.get_x(); }
	float get_y(){ return position.get_y(); }
	int get_damage(){ return damage; }
	float get_diameter(){ return diameter; }
	Vector get_position(){ return position; }
	Vector get_velocity(){ return velocity; }
	int get_type(){ return type; }
};