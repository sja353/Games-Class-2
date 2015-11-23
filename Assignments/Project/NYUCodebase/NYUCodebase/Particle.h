#pragma once

#include "Vector.h"
#include "Color.h"
class Particle {
public:
	Particle(){}
	Vector position;
	Vector velocity;
	float lifetime =0.0f;
	Color color_deviation;
};