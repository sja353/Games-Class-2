#include "Entity.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)*v0 + t*v1;
}

void Entity::UpdateX(float time_elapsed){
	x_velocity = lerp(x_velocity, 0.0f, time_elapsed*x_friction);
	x_velocity += x_acceleration*time_elapsed;
	x += x_velocity*time_elapsed;
}

void Entity::UpdateY(float time_elapsed){
	y_velocity = lerp(y_velocity, 0.0f, time_elapsed*y_friction);
	y_velocity += y_acceleration*time_elapsed;
	y += y_velocity*time_elapsed;
}

void Entity::Draw(ShaderProgram* program, Matrix* modelMatrix){
	
	modelMatrix->Translate(x, y, 0);
	program->setModelMatrix(*modelMatrix);
	sprite.Draw(program);
	modelMatrix->identity();
}

bool Entity::Collides(Entity* other){
	return (abs(x - other->x) * 2 <= (width + other->width) && (abs(y - other->y) * 2 <= (height + other->height)));
}

bool Entity::Is_shot(Bullet* bullet){
	return (abs(x - bullet->x) * 2 <= (width) && (abs(y - bullet->y) * 2 <= (height )));
}

void Entity::FixXPenetration(Entity* other){
	float x_distance = fabs(x - other->x);
	float penetration = fabs(x_distance - (width / 2) - (other->width / 2));
	if (x < other->x){
		x = x - penetration - .001;
	}
	else{
		x = x + penetration + .001;
	}
}

void Entity::FixYPenetration(Entity* other){
	float y_distance = fabs(y - other->y);
	float penetration = fabs(y_distance - (height / 2) - (other->height / 2));
	if ((y - other->y) < 1){
		y = y - penetration - .001;
	}
	else{
		y = y + penetration + .001;
	}
}