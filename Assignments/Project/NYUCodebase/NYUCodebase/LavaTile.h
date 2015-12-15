#pragma once
#include "TerrainTile.h"
#include "LightManager.h"

class LavaTile :public TerrainTile{
public: 
	void update(float time_elapsed){

	}

private:
	float animation_counter = 0.0f;
	float animation_time = 0.5f;
	Light* light;


};