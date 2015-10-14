#include "SheetSprite.h"
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size){
	this->textureID = textureID;
	this->u = u;
	this->v = v;
	this->width = width;
	this->height = height;
	this->size = size;
	u_plus = u + width;
	v_plus = v + height;
	aspect = width / height;
	x = 0.5f*aspect*size;
	y = 0.5f*size;
}

SheetSprite::SheetSprite(){

}

void SheetSprite::Draw(ShaderProgram* program){
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	GLfloat texcoords[] = {
		u, v_plus,
		u_plus, v,
		u, v,
		u_plus, v,
		u, v_plus,
		u_plus, v_plus
	};
	float vertices[] = {
		-x, -y,
		x, y,
		-x, y,
		x, y,
		-x, -y,
		x, -y
	};

	//triangles
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	//texture
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texcoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//finish
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}