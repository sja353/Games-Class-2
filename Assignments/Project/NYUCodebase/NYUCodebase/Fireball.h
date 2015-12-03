#include "Projectile.h"

class Fireball :public Projectile{
public:
	Fireball(Vector initial_position, Vector initial_velocity, Vector acceleration, ParticleEmitter* emitter, int damage){
		position = initial_position;
		velocity = initial_velocity;
		this->emitter = emitter;
		exploding_lifetime = .3f;
		max_impacts = 1;
		this->damage = damage;
		this->acceleration = acceleration;
		max_velocity = 20.0f;
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
				velocity.set_y(velocity.get_y() + acceleration.get_y() * time_elapsed);
			}
			position.set_x(position.get_x() + velocity.get_x() * time_elapsed);
			position.set_y(position.get_y() + velocity.get_y() * time_elapsed);
		}
		emitter->update(time_elapsed);
	}
};