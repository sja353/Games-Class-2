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
	GLuint texture;
	Color start_color, end_color, color_deviation;
	ShaderProgram* program;
	std::vector<ParticleEmitter*> active_emitters;
	Matrix *viewMatrix, *projectionMatrix;
	Sheetposition shaded_ball, white_square, white_ball, white_smoke, rock_particle;
public:
	~SpecialEffects(){ delete program; }
	SpecialEffects(){}
	void set_texture(GLuint texture){
		this->texture = texture;
		float sheet_width = 144.0;
		float sheet_height = 216.0;
		/*<TextureAtlas imagePath="sprites.png">
		<SubTexture name="rockparticle.png" x="72" y="72" width="70" height="70"/>
		<SubTexture name="whiteball.png" x="0" y="72" width="70" height="70"/>
		<SubTexture name="whiteballshaded.png" x="0" y="0" width="70" height="70"/>
		<SubTexture name="whitesmokepuff.png" x="0" y="144" width="70" height="70"/>
		<SubTexture name="whitesquare.png" x="72" y="0" width="70" height="70"/>
		</TextureAtlas>*/
		rock_particle = Sheetposition(72, 72, 70, 70, .5, sheet_width, sheet_height);
		white_ball = Sheetposition(0, 72, 70, 70, .5, sheet_width, sheet_height);
		shaded_ball = Sheetposition(0, 0, 70, 70, .5, sheet_width, sheet_height);
		white_smoke = Sheetposition(0, 144, 70, 70, .5, sheet_width, sheet_height);
		white_square = Sheetposition(72, 0, 70, 70, .5, sheet_width, sheet_height);


	}
	SpecialEffects(ShaderProgram* program, Matrix* viewMatrix, Matrix* projectionMatrix){ 
		this->program = program; 
		this->viewMatrix = viewMatrix;
		this->projectionMatrix = projectionMatrix;
	}

	ParticleEmitter* fireball_trail(Vector position, float flip, Color start_color, Color end_color, Color color_deviation){	
		particle_count = 50;
		max_lifetime = .2;
		this->position = position;
		gravity.set_y(0.0f);
		this->velocity.set_x(-1.0*flip);
		this->velocity.set_y(0.0);
		velocity_deviation.set_x(velocity.get_x()*-2);
		velocity_deviation.set_y(1.0f);
		this->start_color = start_color;
		this->end_color = end_color;
		this->color_deviation = color_deviation;
		render_level = 0;
		ParticleEmitter* new_emitter = new  ParticleEmitter(texture, white_ball, particle_count, max_lifetime, max_lifetime, 2.0, this->position, gravity, this->velocity,
			velocity_deviation, start_color, end_color, color_deviation, program, render_level, .025f, .0175f, 6.28);
		clear_all_values();
		return new_emitter;
		//active_emitters.push_back(new_emitter);
	}
	void rock_particles(Vector position){
		particle_count = 20;
		max_lifetime = 2.0;
		this->position = position;
		gravity.set_y(10.0);
		this->velocity.set_x(0.0);
		this->velocity.set_y(2.0);
		velocity_deviation.set_x(1.0);
		velocity_deviation.set_y(1.0);
		this->start_color.r = 1.0;
		this->start_color.g = 1.0;
		this->start_color.b = 1.0;
		this->start_color.a = 1.0;
		this->end_color.r = 1.0;
		this->end_color.g = 1.0;
		this->end_color.b = 1.0;
		this->end_color.a = 1.0;
		this->color_deviation.r = 0.0;
		this->color_deviation.g = 0.0;
		this->color_deviation.b = 0.0;
		this->color_deviation.a = 0.0;
		render_level = 0;
		ParticleEmitter* new_emitter = new  ParticleEmitter(texture, rock_particle, particle_count, max_lifetime, 0, 1.0, this->position, gravity, this->velocity,
			velocity_deviation, start_color, end_color, color_deviation, program, render_level, .04f, .04f, 6.28*3);
		active_emitters.push_back(new_emitter);
		clear_all_values();
	}
	void smoke_puff(Vector position, Color start_color, Color end_color, Color color_deviation){
		particle_count = 50;
		max_lifetime = .4;
		this->position = position;
		gravity.set_y(7.0f);
		this->velocity.set_x(0.0);
		this->velocity.set_y(1.0);
		velocity_deviation.set_x(2.0);
		velocity_deviation.set_y(3.0);
		this->start_color = start_color;
		this->end_color = end_color;
		this->color_deviation = color_deviation;
		render_level = 1;
		ParticleEmitter* new_emitter = new  ParticleEmitter(texture, white_smoke, particle_count, max_lifetime, max_lifetime, .4, this->position, gravity, this->velocity,
			velocity_deviation, start_color, end_color, color_deviation, program, render_level, .1f, .1f, 0.0);
		clear_all_values();
		active_emitters.push_back(new_emitter);
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
	/*void jumpTest(float x, float y){
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
	}*/
	void update(float time_elapsed){
		for (int i = 0; i < active_emitters.size(); i++){
			active_emitters[i]->update(time_elapsed);
			if (active_emitters[i]->get_lifetime() > active_emitters[i]->get_max_lifetime()){
				delete active_emitters[i];
				active_emitters.erase(active_emitters.begin()+i);
			}
		}
	}
	void render(unsigned int render_level){
		program->setViewMatrix(*viewMatrix);
		for (int i = 0; i < active_emitters.size(); i++){
			if (active_emitters[i]->get_render_level() == render_level){active_emitters[i]->render();}
		}
	}

};