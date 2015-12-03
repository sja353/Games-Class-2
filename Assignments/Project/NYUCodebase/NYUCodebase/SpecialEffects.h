#pragma once
#include "ParticleEmitter.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}
// class to have some pre-defined particle emitter effects and sprite distortion effects
class SpecialEffects{
private:
	unsigned int particle_count, render_level;
	float max_lifetime;
	Vector position, gravity, velocity, velocity_deviation;
	
	Color start_color, end_color, color_deviation;
	ShaderProgram* program;
	std::vector<ParticleEmitter> active_emitters;
	Matrix *viewMatrix, *projectionMatrix;
public:
	//~SpecialEffects(){ delete program; }
	SpecialEffects(){}
	
	SpecialEffects(ShaderProgram* program, Matrix* viewMatrix, Matrix* projectionMatrix){ 
		this->program = program; 
		this->viewMatrix = viewMatrix;
		this->projectionMatrix = projectionMatrix;
	}

	ParticleEmitter* fireball_trail(Vector position, float flip, Color start_color, Color end_color, Color color_deviation){	
		particle_count = 25;
		max_lifetime = .4;
		this->position = position;
		gravity.set_y(0.0f);
		this->velocity.set_x(-1.0*flip);
		this->velocity.set_y(0.0);
		velocity_deviation.set_x(velocity.get_x()*-2);
		velocity_deviation.set_y(.5f);
		this->start_color = start_color;
		this->end_color = end_color;
		this->color_deviation = color_deviation;
		render_level = 0;
		ParticleEmitter* new_emitter = new  ParticleEmitter(particle_count, max_lifetime, 2.0, this->position, gravity, this->velocity,
			velocity_deviation, start_color, end_color, color_deviation, program, render_level, .03f, .03f);
		clear_all_values();
		return new_emitter;
		//active_emitters.push_back(new_emitter);
	}

	void clear_all_values(){
		particle_count = 0;
		render_level= 0;
		float max_lifetime = 0;
		position.clear();
		gravity.clear();
		velocity.clear();
		velocity_deviation.clear();
		start_color.clear();
		end_color.clear();
		color_deviation.clear();
		color_deviation.a = 0.0;
	}
	void jumpTest(float x, float y){
		particle_count = 50;
		max_lifetime = .5;
		position.set_x(x);
		position.set_y(y);
		gravity.set_x(0.0f);
		gravity.set_y(.1f);
		velocity.set_x(0.0f);
		velocity.set_y(0.0f);
		velocity_deviation.set_x(1.0f);
		velocity_deviation.set_y(1.0f);
		start_color.r = 0.0;
		start_color.g = 1.0;
		start_color.b = 0.0;
		start_color.a = 1.0;
		end_color.r = 0.0;
		end_color.g = 0.0;
		end_color.b = 1.0;
		end_color.a = 1.0;
		color_deviation.r = 0.0;
		color_deviation.g = 0.0;
		color_deviation.b = 0.0;
		color_deviation.a = 0.0;
		render_level = 0;
		ParticleEmitter new_emitter = ParticleEmitter(particle_count, max_lifetime, 2.0, position, gravity, velocity, 
										velocity_deviation, start_color, end_color, color_deviation, program, render_level);
		active_emitters.push_back(new_emitter);
	}
	void update(float time_elapsed){
		for (int i = 0; i < active_emitters.size(); i++){
			active_emitters[i].update(time_elapsed);
			if (active_emitters[i].get_lifetime() > active_emitters[i].get_max_lifetime()){
				active_emitters.erase(active_emitters.begin()+i);
			}
		}
	}
	void render(unsigned int render_level){
		program->setViewMatrix(*viewMatrix);
		for (int i = 0; i < active_emitters.size(); i++){
			if (active_emitters[i].get_render_level() == render_level){active_emitters[i].render();}
		}
	}

};