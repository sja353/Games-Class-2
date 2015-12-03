#pragma once
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

#include "Light.h"
#include "ShaderProgram.h"
class LightManager{
public:
	~LightManager(){
		for (int i = 0; i < 32; i++){
			delete lights[i];
		}
	}
	LightManager(){}
	void initialize() {
		for (int i = 0; i < 32; i++){
			lights[i] = new Light();
		}
	}
	void accept_light(Light* light) {
		for (int i = 0; i < 32; i++){
			if (lights[i]->is_dark()){
				delete lights[i];
				lights[i] = light;
				i = 32;
			}
		}
	}
	void draw_lights(ShaderProgram* program){
		GLint lightPositionsUniform = glGetUniformLocation(program->programID, "lightPositions");
		GLint lightColorsUniform = glGetUniformLocation(program->programID, "lightColors");
		GLfloat lightPositions[32 * 2];
		GLfloat lightColors[32 * 3];
		for (int i = 0; i < 32; i++){
			lightPositions[i * 2] = lights[i]->position.get_x();
			lightPositions[(i * 2) + 1] = lights[i]->position.get_y();
			lightColors[(i * 3)] = lights[i]->tint.r;
			lightColors[(i * 3) + 1] = lights[i]->tint.g;
			lightColors[(i * 3) + 2] = lights[i]->tint.b;
		}
		glUniform2fv(lightPositionsUniform, 32, lightPositions);
		glUniform3fv(lightColorsUniform, 32, lightColors);
	}
private:
	Light* lights[32];
};