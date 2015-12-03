#pragma once
#include <math.h>
class Vector {
public:
	Vector(){}
	float get_x() const { return x; }
	float get_y() const { return y; }

	void set_x(float x2){ x = x2; }
	void set_y(float y2){ y = y2; }
	float get_distance(Vector* other){
		return sqrt((pow(x - other->x, 2) + pow(y - other->y, 2)));
	}
	float get_hypotenuse(){
		return sqrt((pow(x, 2) + pow(y, 2)));
	}

	void clear(){
		x = 0.0;
		y = 0.0;
	}
protected:
	float x = 0.0f;
	float y = 0.0f;

 
};