#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <SDL.h>
#include "ShaderProgram.h"

class Entity {
public:
	void Draw(float elapsed, ShaderProgram* program, Matrix* modelMatrix);
	float x;
	float y;
	float rotation;

	float width;
	float height;
	
	float speed;
	float x_direction;
	float y_direction;

	GLuint textureID;

	bool Collides(Entity* other);

};

#endif