#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <SDL.h>
#include "ShaderProgram.h"
#include "SheetSprite.h"
#include "Bullet.h"

class Entity {
public:
	void Draw(ShaderProgram* program, Matrix* modelMatrix);
	void UpdateX(float time_elapsed);
	void UpdateY(float time_elapsed);
	void FixXPenetration(Entity* other);
	void FixYPenetration(Entity* other);
	float lerp(float v0, float v1, float t);
	
	float x = 0.0f;
	float y = 0.0f;
	float rotation;

	float width;
	float height;
	
	float x_velocity = 0.0f;
	float y_velocity = 0.0f;

	float x_acceleration = 0.0f;
	float y_acceleration = 0.0f;

	float x_friction = 2.0f;
	float y_friction = 2.0f;
	
	SheetSprite sprite;

	bool is_Alive = true;
	bool Collides(Entity* other);
	bool Is_shot(Bullet* bullet);

};

#endif