#ifndef SHEETSPRITE_HEADER
#define SHEETSPRITE_HEADER
#include "ShaderProgram.h"
class SheetSprite {
public:
	SheetSprite();
	SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);

	void Draw(ShaderProgram* program);
	
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;
	float size;
	float v_plus;
	float u_plus;
	float x;
	float y;
	float aspect;
};
#endif