#include "Bullet.h"

void Bullet::Choose_animation(float time_elapsed){
	animation_counter += time_elapsed;
	if (animation_counter >= switch_point){
		animation_counter = 0.0f;
		if (advancing) { which_sprite++; }
		else { which_sprite--; }
		if (which_sprite >= 4) {
			which_sprite = 3;
			advancing = !advancing;
		}
		else if (which_sprite <= -1){
			which_sprite = 0;
			advancing = !advancing;
		}
	}
}

void Bullet::Draw(ShaderProgram* program, Matrix* modelMatrix){
	modelMatrix->Translate(x, y, 0);
	program->setModelMatrix(*modelMatrix);
	sprite[which_sprite].Draw(program);
	modelMatrix->identity();
}

void Bullet::Update(float time_elapsed){
	Choose_animation(time_elapsed);
	y += speed*y_direction*time_elapsed;
}