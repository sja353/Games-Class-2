#pragma once 

//#include "Sprite.h"
#include "Player.h"
#include <SDL_mixer.h>


// Mostly trying to make this into an abstract class 
//(so I can have a vector of enemies and just call for their behaviors, instead of vectors of different types of enemies)
// Having some trouble implementing this
class Enemy : public Sprite{
public:
	
	virtual void update(float time_elapsed, Level* level){}
	virtual void die(){}
	virtual void get_behavior(float player_x) {}
	virtual void decide_frame(){}

	
protected:
	bool dead;
	int death_frame;
	int walk_begin, walk_end;
};

