#pragma once

#include "Vector.h"
#include "Particle.h"
#include <vector>;
#include "ShaderProgram.h"
#include "Color.h"
class ParticleEmitter{
public:
	ParticleEmitter(unsigned int particle_count, float max_lifetime, float emitter_max_lifetime, Vector position, Vector gravity, 
					Vector velocity, Vector velocity_deviation, Color start_color, Color end_color, Color color_deviation, ShaderProgram* program);
	ParticleEmitter(){}
	~ParticleEmitter(){}

	void update(float time_elapsed);
	void render();
	float lerp(float v0, float v1, float t){return (1.0 - t)*v0 + t*v1;}
	float get_lifetime(){ return emitter_lifetime; }
	float get_max_lifetime(){ return emitter_max_lifetime; }
private:
	unsigned int particle_count;
	ShaderProgram* program;
	Vector position;
	Vector gravity;
	Vector velocity;
	Vector velocity_deviation;
	float emitter_lifetime = 0.0f;
	float emitter_max_lifetime;
	float max_lifetime;
	std::vector<Particle> particles;
	Color start_color;
	Color end_color;
	Color color_deviation;
	Matrix modelMatrix;
};