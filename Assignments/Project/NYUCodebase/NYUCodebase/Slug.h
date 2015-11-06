#ifndef SLUG_HEADER
#define SLUG_HEADER
#include "Enemy.h"
#include "Player.h"

//Only enemy in the game. I would like to move most of the stuff that is in enemy into this class, but 
//have been having some trouble getting enemy to work as an abstract class
class Slug : virtual public Enemy{
public:
	Slug(){}
	Slug(float size, float x_position, float y_position, int texture, ShaderProgram* program);
	//void get_behavior(Player* player);
	//void die();
private:
};
#endif