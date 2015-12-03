#pragma once
class Color{
public:
	Color(){}
	float r = 0.0f;
	float g = 0.0f;
	float b = 0.0f;
	float a = 1.0f;

	void clear(){
		r = 0.0;
		g = 0.0;
		b = 0.0;
		a = 1.0;
	}
};