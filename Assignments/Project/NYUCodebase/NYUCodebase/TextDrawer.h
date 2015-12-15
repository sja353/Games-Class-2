#pragma once 
#include "ShaderProgram.h"
#include <vector>
class TextDrawer {
private: 
	TextDrawer(){}
public:

	static void Drawtext(int fontTexture, std::string text, float size, float spacing, ShaderProgram* program, float offset = 0.0){
		float texture_size = 1.0 / 16.0f;
		std::vector<float> vertexData;
		std::vector<float> texCoordData;

		for (int i = 0; i < text.size(); i++){
			float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
			float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

			vertexData.insert(vertexData.end(), {
				((size + spacing)*i) + (-0.5f * size)+offset, 0.5f *size,
				((size + spacing)*i) + (-0.5f * size)+offset, -0.5f *size,
				((size + spacing)*i) + (0.5f * size)+offset, 0.5f *size,
				((size + spacing)*i) + (0.5f * size)+offset, -0.5f *size,
				((size + spacing)*i) + (0.5f * size)+offset, 0.5f *size,
				((size + spacing)*i) + (-0.5f * size)+offset, -0.5f *size,
			});

			texCoordData.insert(texCoordData.end(), {
				texture_x, texture_y,
				texture_x, texture_y + texture_size,
				texture_x + texture_size, texture_y,
				texture_x + texture_size, texture_y + texture_size,
				texture_x + texture_size, texture_y,
				texture_x, texture_y + texture_size,

			});
		}

		glUseProgram(program->programID);
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
		glEnableVertexAttribArray(program->positionAttribute);
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
		glEnableVertexAttribArray(program->texCoordAttribute);
		glBindTexture(GL_TEXTURE_2D, fontTexture);
		glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

		glDisableVertexAttribArray(program->positionAttribute);
		glDisableVertexAttribArray(program->texCoordAttribute);
	}
};