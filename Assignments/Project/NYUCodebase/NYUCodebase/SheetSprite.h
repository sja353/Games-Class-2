#pragma once
#include "ShaderProgram.h"
#include "Sheetposition.h"
class SheetSprite {
public:
	SheetSprite(){};
	SheetSprite& operator = (const SheetSprite &rhs){
		this->textureID = rhs.textureID;
		this->x = rhs.x;
		this->y = rhs.y;
		this->v_plus = rhs.v_plus;
		this->u_plus = rhs.u_plus;
		this->aspect = rhs.aspect;
		this->position = rhs.position;
		return *this;
	}
	SheetSprite(unsigned int textureID, Sheetposition position);
	void Draw(ShaderProgram* program);
	void set_position(Sheetposition position);

private:
	unsigned int textureID;
	float v_plus;
	float u_plus;
	float x;
	float y;
	float aspect;
	Sheetposition position;
};