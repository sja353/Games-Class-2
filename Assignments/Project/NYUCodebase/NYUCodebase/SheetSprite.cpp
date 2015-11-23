#include "SheetSprite.h"
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

SheetSprite::SheetSprite(unsigned int textureID, Sheetposition position){
	this->textureID = textureID;
	this->position = position;
}

void SheetSprite::set_position(Sheetposition position){
	this->position = position;
}

void SheetSprite::Draw(ShaderProgram* program){
	
	u_plus = position.u + position.width;
	v_plus = position.v + position.height;
	aspect = position.width / position.height;
	x = 0.5f*aspect*position.size;
	y = 0.5f*position.size;
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat texcoords[] = {
		position.u, v_plus,
		u_plus, position.v,
		position.u, position.v,
		u_plus, position.v,
		position.u, v_plus,
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
	
	glUseProgram(program->programID);
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