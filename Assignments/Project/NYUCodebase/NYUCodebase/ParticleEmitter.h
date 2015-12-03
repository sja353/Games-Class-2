#pragma once
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

#include "Vector.h"
#include "Particle.h"
#include <vector>
#include "ShaderProgram.h"
#include "Color.h"
class ParticleEmitter{
public:
	ParticleEmitter(unsigned int particle_count, float max_lifetime, float emitter_max_lifetime, Vector position, Vector gravity,
		Vector velocity, Vector velocity_deviation, Color start_color, Color end_color, Color color_deviation, ShaderProgram* program,
		unsigned int render_level, float initial_particle_size = 1.0f, float final_particle_size = 1.0f);
	ParticleEmitter(){}
	~ParticleEmitter(){}

	void update(float time_elapsed);
	void render();
	float lerp(float v0, float v1, float t){return (1.0 - t)*v0 + t*v1;}
	float get_lifetime(){ return emitter_lifetime; }
	float get_max_lifetime(){ return emitter_max_lifetime; }
	unsigned int get_render_level(){ return render_level; }
	void update_position(Vector new_position) { position = new_position; }
	void explode();
	void update_direction(Vector new_direction){
		rotation  = (atan2f(new_direction.get_y(), new_direction.get_x()));
		/*if (new_direction.get_x() < 0){
			velocity.set_x(-velocity.get_x());
			velocity_deviation.set_x(velocity.get_x()*-2);
		}*/
	}
private:
	// This decides where the particle effect will be rendered. The higher the number, the later it will be rendered
	// (will appear on top of everything.)
	unsigned int render_level;
	unsigned int particle_count;
	ShaderProgram* program;
	Vector position;
	Vector gravity;
	Vector velocity;
	Vector velocity_deviation;
	float rotation = 0.0f;
	float emitter_lifetime = 0.0f;
	float emitter_max_lifetime;
	float max_lifetime;
	float initial_particle_size;
	float final_particle_size;
	std::vector<Particle> particles;
	Color start_color;
	Color end_color;
	Color color_deviation;
	Matrix modelMatrix;
	int render_count = 0;
	int update_count = 0;
};