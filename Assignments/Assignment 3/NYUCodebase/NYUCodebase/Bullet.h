#ifndef BULLET_HEADER
#define BULLET_HEADER
#include "SheetSprite.h"

class Bullet {
public:
	float x;
	float y;
	float y_direction;
	float speed=1;
	bool visible = false;

	SheetSprite* sprite;
	float animation_counter = 0.0f;
	int which_sprite = 0; 
	float switch_point = 0.5f;
	bool advancing = true;

	void Choose_animation(float time_elapsed);
	void Draw(ShaderProgram* program, Matrix* modelMatrix);
	void Update(float time_elapsed);
};
#endif