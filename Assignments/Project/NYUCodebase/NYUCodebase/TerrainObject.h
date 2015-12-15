#pragma once
#include "Entity.h"
#include "Level.h"
#include "Player.h"
class TerrainObject : public Entity {
	// class for unconventional terrain that has animation, does damage, etc. 
	virtual void update(float time_elapsed, Level* level){}
	virtual void Collides(Player* player);

};