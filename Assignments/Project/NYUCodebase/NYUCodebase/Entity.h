#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <SDL.h>
#include "ShaderProgram.h"
#include "SheetSprite.h"
#include "Audio.h"

//Purpose: Base class for all objects in the game. Movement and penetration updates are done here
// SHOULD updates and movements be here? Yes, they have to be

class Entity {
public:
	void Draw();
	void UpdateX(float time_elapsed);
	void UpdateY(float time_elapsed);
	void FixXPenetration(Entity* other);
	void FixYPenetration(Entity* other);

	virtual void set_hitbox(float width, float height);
	
	//getters/setters
	ShaderProgram* get_program(){ return program; }
	float get_x(){ return x;}
	float get_y(){ return y; }
	float get_width(){ return width; }
	float get_height(){ return height; }
	void set_audio(Audio* new_audio){ audio = new_audio; }
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
	//Still setting some values here. Remember to move them when you can
	float x_velocity = 0.0f;
	float y_velocity = 0.0f;
	float y_gravity = 0.0f;
	float x_acceleration = 0.0f;
	float y_acceleration = 0.0f;
	float x_friction = 25.0f;
	float y_friction = 0.0f;
	float acceleration_decay = .95;

	SheetSprite spritesheet;
	Sheetposition sprite;
	
	float acceleration_rate = 4.0;
	float minimum_velocity = .01;
	float maximum_acceleration = 80.0;
	bool Collides(Entity* other);
	float lerp(float v0, float v1, float t);

	Audio* audio;
};

#endif