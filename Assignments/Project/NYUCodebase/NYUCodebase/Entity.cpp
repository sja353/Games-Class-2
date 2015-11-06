#include "Entity.h"
#include "Sheetposition.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

float Entity::lerp(float v0, float v1, float t){
	return (1.0 - t)*v0 + t*v1;
}

void Entity::set_hitbox(float width, float height){
	this->width = width;
	this->height = height;
}

void Entity::UpdateX(float time_elapsed){
	
	x_velocity = lerp(x_velocity, 0.0f, time_elapsed*x_friction);
	x_velocity += x_acceleration*time_elapsed;
	x_acceleration = acceleration_decay* x_acceleration;
	x += x_velocity*time_elapsed;
}

void Entity::UpdateY(float time_elapsed){
	//y_velocity = lerp(y_velocity, 0.0f, time_elapsed*y_friction);
	y_velocity += y_acceleration*time_elapsed;
	y_acceleration =  y_acceleration - y_gravity*time_elapsed;
	y += y_velocity*time_elapsed;
}

void Entity::Draw(){
	spritesheet.set_position(sprite);
	modelMatrix.Translate(x, y, 0);
	if (mirrored){ modelMatrix.Scale(-1.0, 1.0, 0.0); }
	program->setModelMatrix(modelMatrix);
	spritesheet.Draw(program);
	modelMatrix.identity();
}

bool Entity::Collides(Entity* other){
	return (abs(x - other->x) * 2 <= (width + other->width) && (abs(y - other->y) * 2 <= (height + other->height)));
}


void Entity::FixXPenetration(Entity* other){
	float x_distance = fabs(x - other->x);
	float penetration = fabs(x_distance - (width / 2) - (other->width / 2));
	if (x < other->x){
		x = x - penetration - .0000001;
	}
	else{
		x = x + penetration + .00000001;
	}
}

void Entity::FixYPenetration(Entity* other){
	float y_distance = fabs(y - other->y);
	float penetration = fabs(y_distance - (height / 2) - (other->height / 2));
	if (y < other->y){
		y = y - penetration - .001;
	}
	else{
		y = y + penetration + .001;
	}
}