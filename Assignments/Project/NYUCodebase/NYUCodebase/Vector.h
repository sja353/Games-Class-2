#pragma once
class Vector {
public:
	Vector(){}
	float get_x(){ return x; }
	float get_y(){ return y; }

	void set_x(float x2){ x = x2; }
	void set_y(float y2){ y = y2; }

private:
	float x = 0.0f;
	float y = 0.0f;

 
};