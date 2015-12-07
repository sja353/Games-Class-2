#include "ParticleEmitter.h"


void ParticleEmitter::explode(){
	//this->particle_count = 500;
	this->velocity.set_x(0);
	this->velocity.set_y(0);
	this->velocity_deviation.set_x(5.0);
	this->velocity_deviation.set_y(5.0);
	this->max_lifetime = .1f;
}

ParticleEmitter::ParticleEmitter(unsigned int particle_count, float max_lifetime, float emitter_max_lifetime, Vector position, Vector gravity,
	Vector velocity, Vector velocity_deviation, Color start_color, Color end_color, 
	Color color_deviation, ShaderProgram* program, unsigned int render_level, float initial_particle_size, float final_particle_size){
	//DBOUT("created");
	this->particle_count = particle_count;
	this->max_lifetime = max_lifetime;
	this->emitter_max_lifetime = emitter_max_lifetime;
	this->position = position;
	this->gravity = gravity;
	this->velocity = velocity;
	this->velocity_deviation = velocity_deviation;
	this->start_color = start_color;
	this->end_color = end_color;
	this->color_deviation = color_deviation;
	this->program = program;
	this->render_level = render_level;
	this->initial_particle_size = initial_particle_size;
	this->final_particle_size = final_particle_size;
	for (int i = 0; i < particle_count; i++){
		Particle particle;
		particle.color_deviation.r = color_deviation.r * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.r;
		particle.color_deviation.g = color_deviation.g * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.g;
		particle.color_deviation.b = color_deviation.b * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.b;
		particle.color_deviation.a = color_deviation.a * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.a;
		particle.velocity.set_x((velocity.get_x() - velocity_deviation.get_x() / 2) +
			(((float)rand()) / (float)RAND_MAX)*velocity_deviation.get_x());
		particle.velocity.set_y((velocity.get_y() - velocity_deviation.get_y() / 2) +
			(((float)rand()) / (float)RAND_MAX)*velocity_deviation.get_y());
		//particle.position = position;
		particle.position.set_x(0.0f);
		particle.position.set_y(0.0f);
		particle.lifetime = (((float)rand()) / ((float)RAND_MAX))*max_lifetime;
		particles.push_back(particle);
	}
}
void ParticleEmitter::update(float time_elapsed){
	update_count++;
	emitter_lifetime += time_elapsed;
	for (int i = 0; i < particles.size(); i++){
		particles[i].lifetime += time_elapsed;
		if (particles[i].lifetime > max_lifetime){
			particles[i].lifetime = 0.0f;
			particles[i].position.set_x(0.0f);
			particles[i].position.set_y(0.0f);

			//Currently does not keep total velocity consistent. Add option to do that?
			particles[i].velocity.set_x((velocity.get_x() - velocity_deviation.get_x() / 2) +
				(((float)rand()) / (float)RAND_MAX)*velocity_deviation.get_x());
			particles[i].velocity.set_y((velocity.get_y() - velocity_deviation.get_y() / 2) +
				(((float)rand()) / (float)RAND_MAX)*velocity_deviation.get_y());
			particles[i].color_deviation.r = color_deviation.r * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.r;
			particles[i].color_deviation.g = color_deviation.g * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.g;
			particles[i].color_deviation.b = color_deviation.b * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.b;
			particles[i].color_deviation.a = color_deviation.a * -.5 + (((float)rand()) / (float)RAND_MAX)*color_deviation.a;
		}
		particles[i].position.set_x(particles[i].position.get_x() + particles[i].velocity.get_x()*time_elapsed);
		particles[i].position.set_y(particles[i].position.get_y() + particles[i].velocity.get_y()*time_elapsed);
		particles[i].velocity.set_x(particles[i].velocity.get_x() - gravity.get_x()*time_elapsed);
		particles[i].velocity.set_y(particles[i].velocity.get_y() - gravity.get_y()* time_elapsed);
	}

}

void ParticleEmitter::render(){
	render_count++;
	/*if (render_count == 100){
		DBOUT(update_count);
		render_count = 0;
		update_count = 0;
	}*/
	/*
	glPointSize(5.0);
	std::vector<float> particle_vertices;
	std::vector<float> particle_colors;
	for (int i = 0; i < particles.size(); i++){
		float m = particles[i].lifetime / max_lifetime;
		particle_vertices.push_back(particles[i].position.get_x());
		particle_vertices.push_back(particles[i].position.get_y());
		particle_colors.push_back(lerp(start_color.r, end_color.r, m) + particles[i].color_deviation.r);
		particle_colors.push_back(lerp(start_color.g, end_color.g, m) + particles[i].color_deviation.g);
		particle_colors.push_back(lerp(start_color.b, end_color.b, m) + particles[i].color_deviation.b);
		particle_colors.push_back(lerp(start_color.a, end_color.a, m) + particles[i].color_deviation.a);
	}
	//modelMatrix.identity();
	modelMatrix.Translate(position.get_x(), position.get_y(), 0);
	modelMatrix.Rotate(rotation);
	program->setModelMatrix(modelMatrix);
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, particle_vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);
	GLuint colorAttribute = glGetAttribLocation(program->programID, "color");
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, 0, particle_colors.data());
	glEnableVertexAttribArray(colorAttribute);
	glDrawArrays(GL_POINTS, 0, particle_vertices.size() / 2);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	modelMatrix.identity();
	*/

	//Problems: Causes a lot of slowdown - more than it should?
	// Less particles rendered than there should be?

	
	std::vector<float> particle_vertices;
	std::vector<float> particle_colors;
	float particle_size;
	initial_particle_size = .1;
	final_particle_size = .035;
	for (int i = 0; i < particles.size(); i++){
		float m = particles[i].lifetime / max_lifetime;
		particle_size = (lerp(initial_particle_size, final_particle_size, m));
		//DBOUT(initial_particle_size);
		particle_vertices.push_back(particles[i].position.get_x() - particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_y() - particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_x() + particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_y() + particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_x() - particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_y() + particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_x() + particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_y() + particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_x() - particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_y() - particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_x() + particle_size / 2);
		particle_vertices.push_back(particles[i].position.get_y() - particle_size / 2);
		float r = lerp(start_color.r, end_color.r, m) + particles[i].color_deviation.r;
		float g = lerp(start_color.g, end_color.g, m) + particles[i].color_deviation.g;
		float b = lerp(start_color.b, end_color.b, m) + particles[i].color_deviation.b;
		float a = lerp(start_color.a, end_color.a, m) + particles[i].color_deviation.a;
		for (int i = 0; i < 6; i++){
			particle_colors.push_back(r);
			particle_colors.push_back(g);
			particle_colors.push_back(b);
			particle_colors.push_back(a);
		}
	}

	//modelMatrix.identity();
	modelMatrix.Translate(position.get_x(), position.get_y(), 0);
	modelMatrix.Rotate(rotation);
	program->setModelMatrix(modelMatrix);
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, particle_vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);
	GLuint colorAttribute = glGetAttribLocation(program->programID, "color");
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, 0, particle_colors.data());
	glEnableVertexAttribArray(colorAttribute);
	glDrawArrays(GL_TRIANGLES, 0, particle_vertices.size()/ 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	modelMatrix.identity();
}