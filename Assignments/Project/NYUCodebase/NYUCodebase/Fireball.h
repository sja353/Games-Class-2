#include "Projectile.h"

class Fireball :public Projectile{
public:
	~Fireball(){
		light->turn_off();
	}
	Fireball(Vector initial_position, Vector initial_velocity, Vector acceleration, ParticleEmitter* emitter, int damage, LightManager* light_manager){
		position = initial_position;
		velocity = initial_velocity;
		this->emitter = emitter;
		exploding_lifetime = .3f;
		max_impacts = 1;
		light = new Light();
		light->position = initial_position;
		light->tint.r = 1.0;
		light->tint.g = 1.0;
		light->tint.b = 0.0;
		light->a = 1.0;
		light->b = 2.0;
		light->is_off = false;
		light_manager->accept_light(light);
		this->damage = damage;
		this->acceleration = acceleration;
		max_velocity = 20.0f;
		diameter = .1f;
		max_lifetime = 5.0f;
	}

	void update(float time_elapsed, Level* level) override{
		light->position = this->position;
		lifetime += time_elapsed;
		make_adjustments();
		if (!exploding){
			calculate_x_terrain_collisions(level);
			calculate_y_terrain_collisions(level);
			if (velocity.get_hypotenuse() < max_velocity){
				velocity.set_x(velocity.get_x() + acceleration.get_x() * time_elapsed);
				velocity.set_y(velocity.get_y() + acceleration.get_y() * time_elapsed);
			}
			position.set_x(position.get_x() + velocity.get_x() * time_elapsed);
			position.set_y(position.get_y() + velocity.get_y() * time_elapsed);
		}
		emitter->update(time_elapsed);
	}
private:
	Light* light;
};