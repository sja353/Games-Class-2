#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
#include "Shaderprogram.h"
#include "Matrix.h"

#ifdef _WINDOWS
	#define RESOURCE_FOLDER ""
#else
	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

// Texture loading function

GLuint LoadTexture(const char *image_path){
	SDL_Surface *surface = IMG_Load (image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;

}

void DrawSprite(ShaderProgram* program, float* vertices, const char* image_path) {
	
	float texcoords[] = { 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	
	//triangles
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	
	//texture
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texcoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	LoadTexture(image_path);

	glBindTexture(GL_TEXTURE_2D, the_int);
	//blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//finish
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

	
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	// Setup for triangles
	glViewport(0, 0, 1280, 640);
	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	projectionMatrix.setOrthoProjection(-3.55f, 3.55f, -2.0f, 2.0f, -1.0f, 1.0f);
	program.setModelMatrix(modelMatrix);
	program.setViewMatrix(viewMatrix);
	program.setProjectionMatrix(projectionMatrix);

	//
	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5 };
	float verticesb[] = { 2.0, 2.0, 1.5, 2.0, 1.5, 1.5, 1.5, 1.5, 2.0, 1.5, 2.0, 2.0 };
	float angle = 0.0f;
	float distance = 0.0f;
	bool trigger = true;
	// Time tracking
	float lastFrameTicks = 0.0f;

	SDL_Event event;
	bool done = false;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		//Time tracking
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		// Think of the full screen as multiple "model matrices" with various transformations dumped on top of 
		// the projection matrix. 


		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program.programID);

		angle += (elapsed*3.1415926);

		if (trigger){ distance += elapsed; }
		else { distance -= elapsed; }
		if (distance >= 1) {
			trigger = false;
		}
		if (distance <= 0) { 
			trigger = true; 
		}


		modelMatrix.Translate(1.0, 1.0, 0);
		modelMatrix.Translate(-distance, 0, 0); 
		if (trigger){ modelMatrix.Rotate(2 * angle); }
		else { modelMatrix.Rotate(-2 * angle); }
		program.setModelMatrix(modelMatrix);
		DrawSprite(&program, vertices, "../../../../assets/graphics/Physics assets (330 assets)/PNG/Aliens/alienBlue_round.png");
		modelMatrix.identity();
		

		// 2ND
		modelMatrix.Translate(-1.0, -1.0, 0);
		modelMatrix.Rotate( 6* angle);
		modelMatrix.Translate(-distance, 0, 0);
		program.setModelMatrix(modelMatrix);
		DrawSprite(&program, vertices, "../../../../assets/graphics/Physics assets (330 assets)/PNG/Aliens/alienGreen_round.png");
		modelMatrix.identity();
		
		modelMatrix.Translate(2.0, 1.0, 0);
		program.setModelMatrix(modelMatrix);
		DrawSprite(&program, vertices, "../../../../assets/graphics/Physics assets (330 assets)/PNG/Aliens/alienPink_round.png");
		modelMatrix.identity();
		SDL_GL_SwapWindow(displayWindow);
	}
	SDL_Quit();
	return 0;
}
