#include "ParticleEmitter.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

ParticleEmitter::ParticleEmitter(unsigned int particle_count, float max_lifetime, float emitter_max_lifetime, Vector position, Vector gravity,
	Vector velocity, Vector velocity_deviation, Color start_color, Color end_color, Color color_deviation, ShaderProgram* program){
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
	glPointSize(5.0f);
	std::vector<float> particle_vertices;
	std::vector<float> particle_colors;
	for (int i = 0; i < particles.size(); i++){
		particle_vertices.push_back(particles[i].position.get_x());
		particle_vertices.push_back(particles[i].position.get_y());
		float m = particles[i].lifetime / max_lifetime;
		particle_colors.push_back(lerp(start_color.r, end_color.r, m) + particles[i].color_deviation.r);
		particle_colors.push_back(lerp(start_color.g, end_color.g, m) + particles[i].color_deviation.g);
		particle_colors.push_back(lerp(start_color.b, end_color.b, m) + particles[i].color_deviation.b);
		particle_colors.push_back(lerp(start_color.a, end_color.a, m) + particles[i].color_deviation.a);
	}
	modelMatrix.identity();
	modelMatrix.Translate(position.get_x(), position.get_y(), 0);
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
	//modelMatrix.identity();
}