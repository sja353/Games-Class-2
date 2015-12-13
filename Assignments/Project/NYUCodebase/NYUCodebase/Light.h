#pragma once
#include "Vector.h"
#include "Color.h"
class Light{
public:
	bool is_finished(){
		return is_off;
	}
	Light(){
		position.set_x(0);
		position.set_y(0);
		tint.r = 0.0;
		tint.g = 0.0;
		tint.b = 0.0;
		tint.a = 1.0;
		a = 1.0;
		b = 1.0;
	}
	void turn_off(){
		position.set_x(0);
		position.set_y(0);
		tint.r = 0.0;
		tint.g = 0.0;
		tint.b = 0.0;
		tint.a = 1.0;
		a = 1.0;
		b = 1.0;
		is_off = true;
	}
	Vector position;
	Color tint;
	float a, b;
	bool is_off = true; 
};