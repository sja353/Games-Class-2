#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <SDL.h>
#include "ShaderProgram.h"
#include "SheetSprite.h"
#include "Bullet.h"

class Entity {
public:
	void Draw();
	void UpdateX(float time_elapsed);
	void UpdateY(float time_elapsed);
	void FixXPenetration(Entity* other);
	void FixYPenetration(Entity* other);
	float lerp(float v0, float v1, float t);
	virtual void set_hitbox(float width, float height);
	float get_x(){ return x;}
	float get_y(){ return y; }
	float get_width(){ return width; }
	float get_height(){ return height; }
protected:
	
	ShaderProgram* program;
	Matrix modelMatrix;
	bool mirrored;
	float x = 0.0f;
	float y = 0.0f;
	float rotation =0.0f;
	float size;

	float width = 0.0f;
	float height= 0.0f;
	float x_velocity = 0.0f;
	float y_velocity = 0.0f;
	float y_gravity = 1.0f;
	float x_acceleration = 0.0f;
	float y_acceleration = 0.0f;
	float x_friction = 25.0f;
	float y_friction = 0.0f;
	
	SheetSprite spritesheet;
	Sheetposition sprite;
	
	float acceleration_rate = 4.0;
	float minimum_velocity = .05;
	float maximum_acceleration = 80.0;
	float acceleration_decay = .95;
	bool Collides(Entity* other);

};

#endif