#ifndef SLUG_HEADER
#define SLUG_HEADER
#include "Enemy.h"
#include "Player.h"
class Slug : virtual public Enemy{
public:
	~Slug(){
		//Mix_FreeChunk(death_sound);
	}
	Slug(){}
	Slug(float size, float x_position, float y_position, int texture, ShaderProgram* program);
	void get_behavior(Player* player);
	//void die();
};
#endif