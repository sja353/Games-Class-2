#include "ParticleEmitter.h"


void ParticleEmitter::explode(){
	//this->particle_count = 500;
	this->velocity.set_x(0);
	this->velocity.set_y(0);
	this->velocity_deviation.set_x(5.0);
	this->velocity_deviation.set_y(5.0);
	this->max_lifetime = .1f;
}

ParticleEmitter::ParticleEmitter(GLuint texture, Sheetposition sheet_position, unsigned int particle_count, float max_lifetime, float randomization,
	float emitter_max_lifetime, Vector position, Vector gravity,
	Vector velocity, Vector velocity_deviation, Color start_color, Color end_color, 
	Color color_deviation, ShaderProgram* program, unsigned int render_level, float initial_particle_size, 
	float final_particle_size, float rotation_rate){
	//DBOUT("created");
	this->texture = texture;
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
	colorAttribute = glGetAttribLocation(program->programID, "color");
	this->render_level = render_level;
	this->rotation_rate = rotation_rate;
	this->initial_particle_size = initial_particle_size;
	this->final_particle_size = final_particle_size;
	this->sheet_position = sheet_position;
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
		particle.position.set_x(position.get_x());
		particle.position.set_y(position.get_y());
		particle.lifetime = (((float)rand()) / ((float)RAND_MAX))*randomization;
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
			particles[i].position.set_x(position.get_x());
			particles[i].position.set_y(position.get_y());

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
		particles[i].rotation += rotation_rate * time_elapsed;
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
	std::vector<float> texCoords;
	std::vector<float> particle_colors;
	float particle_size;
	float u = sheet_position.u;
	float u_plus = sheet_position.u + sheet_position.width;
	float v = sheet_position.v;
	float v_plus = sheet_position.v + sheet_position.height;
	for (int i = 0; i < particles.size(); i++){
		float m = particles[i].lifetime / max_lifetime;
		particle_size = (lerp(initial_particle_size, final_particle_size, m));
		//particle_size = .5f;
		float cosTheta = cosf(particles[i].rotation);
		float sinTheta = sinf(particles[i].rotation);

		float TL_x = cosTheta * -particle_size - sinTheta* particle_size;
		float TL_y = sinTheta * -particle_size + cosTheta * particle_size;
		
		float BL_x = cosTheta * -particle_size - sinTheta * -particle_size;
		float BL_y = sinTheta * -particle_size + cosTheta *-particle_size;

		float BR_x = cosTheta * particle_size - sinTheta * -particle_size;
		float BR_y = sinTheta * particle_size + cosTheta * -particle_size;

		float TR_x = cosTheta *particle_size - sinTheta * particle_size;
		float TR_y = sinTheta * particle_size + cosTheta *particle_size;

		particle_vertices.insert(particle_vertices.end(), {
			particles[i].position.get_x() + BL_x, particles[i].position.get_y() + BL_y,
			particles[i].position.get_x() + TR_x, particles[i].position.get_y() + TR_y,
			particles[i].position.get_x() + TL_x, particles[i].position.get_y() + TL_y,

			particles[i].position.get_x() + TR_x, particles[i].position.get_y() + TR_y,
			particles[i].position.get_x() + BL_x, particles[i].position.get_y() + BL_y,
			particles[i].position.get_x() + BR_x, particles[i].position.get_y() + BR_y,
		});
		texCoords.insert(texCoords.end(),
		{ u, v_plus,
		u_plus, v,
		u, v,

		u_plus, v,
		u, v_plus,
		u_plus, v_plus});
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

	modelMatrix.identity();
	//modelMatrix.Translate(position.get_x(), position.get_y(), 0);
	//modelMatrix.Rotate(rotation);
	program->setModelMatrix(modelMatrix);
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, particle_vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);
	
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, 0, particle_colors.data());
	glEnableVertexAttribArray(colorAttribute);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
	glBindTexture(GL_TEXTURE_2D, texture);
	//DBOUT(texture);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDrawArrays(GL_TRIANGLES, 0, particle_vertices.size()/ 2);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	modelMatrix.identity();
}