#pragma once
#include "Vector.h"
#include "Color.h"
class Light{
public:
	bool is_dark(){
		return (tint.r == 0.0 && tint.g == 0.0 && tint.b == 0.0);
	}
	Light(){
		position.set_x(0);
		position.set_y(0);
		tint.r = 0.0;
		tint.g = 0.0;
		tint.b = 0.0;
		tint.a = 1.0;
		float a = 0.0;
		float b = 0.0;
	}
	Vector position;
	Color tint;
	float a, b;
};