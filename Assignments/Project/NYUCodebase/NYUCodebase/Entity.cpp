#include "Entity.h"
#include "Sheetposition.h"
#include "ProjectileManager.h"
#include "Projectile.h"

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
	velocity.set_x(lerp(velocity.get_x(), 0.0f, time_elapsed*friction.get_x()) +acceleration.get_x()*time_elapsed);
	acceleration.set_x(lerp(acceleration.get_x(), 0.0f, time_elapsed*acceleration_decay));
	position.set_x(position.get_x() + velocity.get_x()*time_elapsed);
}

void Entity::UpdateY(float time_elapsed){
	//y_velocity = lerp(y_velocity, 0.0f, time_elapsed*y_friction);
	velocity.set_y(velocity.get_y() + acceleration.get_y()*time_elapsed);
	acceleration.set_y(acceleration.get_y() - gravity.get_y()*time_elapsed);
	position.set_y(position.get_y()+ velocity.get_y()*time_elapsed);
}

void Entity::Draw(){
	modelMatrix.identity();
	spritesheet.set_position(sprite);
	glUniform3fv(coloruniform, 1, color_shift);
	modelMatrix.Translate(position.get_x(), position.get_y(), 0);
	if (mirrored){ modelMatrix.Scale(-1.0, 1.0, 0.0); }
	if (stretchy){ stretch(); }
	program->setModelMatrix(modelMatrix);
	spritesheet.Draw(program);

}
bool Entity::Collides(Entity* other){
	return (abs(position.get_x() - other->get_x()) * 2 <= (width + other->width) && 
		(abs(position.get_y() - other->get_y()) * 2 <= (height + other->height)));
}

bool Entity::is_shot(Projectile* bullet){
	return (abs(position.get_x() - bullet->get_x()) * 2 <= (width + bullet->get_diameter()) &&
		(abs(position.get_y() - bullet->get_y()) * 2 <= (height + bullet->get_diameter())));
}

void Entity::FixXPenetration(Entity* other){
	float x_distance = fabs(position.get_x() - other->get_x());
	float penetration = fabs(x_distance - (width / 2) - (other->width / 2));
	if (position.get_x() < other->get_x()){
		position.set_x(position.get_x() - penetration - .0000001);
	}
	else{
		position.set_x( position.get_x() + penetration + .00000001);
	}
}

void Entity::FixYPenetration(Entity* other){
	float y_distance = fabs(position.get_y() - other->get_y());
	float penetration = fabs(y_distance - (height / 2) - (other->height / 2));
	if (position.get_y() < other->get_y()){
		position.set_y( position.get_y() - penetration - .001);
	}
	else{
		position.set_y(position.get_y() + penetration + .001);
	}
}