#include "Entity.h"

void Entity::Draw(float elapsed, ShaderProgram* program, Matrix* modelMatrix){
	x += x_direction*speed*elapsed;
	y += y_direction*speed*elapsed;
	
	modelMatrix->Translate(x, y, 0);
	program->setModelMatrix(*modelMatrix);

	float texcoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	//float texcoords[] = { 0.2f, .8f, .8f, .8f, .8f, 0.2f,.8f, 0.2f, 0.8f, 0.2f, 0.2f, 0.8f };

	float vertices[] = { x - (width/2), y-(height/2), x + (width/2), y - (height/2), x + (width/2), y + (height/2),
							x + (width/2), y + (height/2), x-(width/2), y + (height/2), x-(width/2), y-(height/2) };
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

	modelMatrix->identity();
}
bool Entity::Collides(Entity* other){
	return !((y - height / 2) >= (other->y + (other->height / 2)) || (y + height / 2) <= (other->y - (other->height / 2))
		|| (x - width / 2) >= (other->x + (other->width / 2)) || (x + width / 2) <= (other->x + (other->width / 2)));
}