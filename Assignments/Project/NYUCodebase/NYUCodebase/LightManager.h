#pragma once
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}
#include <vector>
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
		ambient_light[0] = .1;
		ambient_light[1] = .3;
		ambient_light[2] = 0.0;
	}
	void accept_light(Light* light) {
		active_lights.push_back(light);
	}
	void draw_lights(ShaderProgram* program, float player_x, float player_y){
		for (int i = 0; i < j; i++){
			lights[i] = new Light();
		}
		j = 0;
		for (int i = 0; i < active_lights.size(); i++){
			if (active_lights[i]->is_finished()){
				delete active_lights[i];
				active_lights.erase(active_lights.begin() + i);
			}
			else if (abs(active_lights[i]->position.get_x() - player_x) < 5 && abs(active_lights[i]->position.get_y() - player_y) < 5){
				if (j < 32 && !active_lights[i]->is_dark()){
					delete lights[j];
					lights[j] = active_lights[i];
					j++;
				}
			}
		}

		GLint lightPositionsUniform = glGetUniformLocation(program->programID, "lightPositions");
		GLint lightColorsUniform = glGetUniformLocation(program->programID, "lightColors");
		GLint lightABUniform = glGetUniformLocation(program->programID, "lightAB");
		GLint brightUniform = glGetUniformLocation(program->programID, "brightness");
		GLfloat lightAB[32 * 2];
		GLfloat lightPositions[32 * 2];
		GLfloat lightColors[32 * 3];
		for (int i = 0; i < 32; i++){
			lightPositions[i * 2] = lights[i]->position.get_x();
			lightPositions[(i * 2) + 1] = lights[i]->position.get_y();
			lightAB[i * 2] = lights[i]->a;
			lightAB[(i * 2) + 1] = lights[i]->b;
			lightColors[(i * 3)] = lights[i]->tint.r;
			lightColors[(i * 3) + 1] = lights[i]->tint.g;
			lightColors[(i * 3) + 2] = lights[i]->tint.b;
		}
		glUniform2fv(lightABUniform, 32, lightAB);
		glUniform2fv(lightPositionsUniform, 32, lightPositions);
		glUniform3fv(lightColorsUniform, 32, lightColors);
		glUniform3fv(brightUniform, 1, ambient_light);
		
	}
private:
	int j = 0;
	Light* lights[32];
	float ambient_light[3];
	std::vector<Light*> active_lights;
};