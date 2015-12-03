#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include <SDL.h>
#include "ShaderProgram.h"
#include "SheetSprite.h"
#include "Audio.h"
#include "SpecialEffects.h"
class ProjectileManager;
class Projectile;

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
	Vector get_position(){ return position; }
	float get_x() const { return position.get_x(); }
	float get_y() const { return position.get_y(); }
	float get_width()const { return width; }
	float get_height()const { return height; }
	void set_audio(Audio* new_audio){ audio = new_audio; }
	void set_effects(SpecialEffects* new_effects){ special_effects = new_effects; }
	void set_projectile_manager(ProjectileManager* new_manager){ projectile_manager = new_manager; }
	void set_hp(int hitpoints){ hp = hitpoints; }
	int get_hp(){ return hp; }
	bool is_shot(Projectile* bullet);
	Sheetposition get_sheet_position() { return sprite; }
protected:
	ShaderProgram* program;
	Matrix modelMatrix;
	bool mirrored;
	Vector position, velocity, gravity, acceleration, friction;
	float rotation =0.0f;
	float size;
	float width = 0.0f;
	float height= 0.0f;
	float acceleration_decay = 25.0;
	SheetSprite spritesheet;
	Sheetposition sprite;
	
	float acceleration_rate = 4.0;
	float minimum_velocity = .01;
	float maximum_acceleration = 80.0;
	bool Collides(Entity* other);
	void stretch(){
		modelMatrix.Scale(1 + abs((velocity.get_x() / 30.0)), 1 -(velocity.get_y() / 10.0), 0.0);
	}
	bool stretchy = false;
	float lerp(float v0, float v1, float t);
	SpecialEffects* special_effects;
	ProjectileManager* projectile_manager;

	Audio* audio;
	int hp = 0;
};

#endif