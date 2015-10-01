#include "Matrix.h"
#include "ShaderProgram.h"
#include <SDL.h>
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
#include "Shaderprogram.h"
#include "Matrix.h"
#include "Entity.h"

class GameApp {
public:
	GameApp();
	~GameApp();

	void Setup();
	void ProcessEvents();
	bool UpdateAndRender();

	void Render(float elapsed);
	void Update();
	
private:
	
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	ShaderProgram* program;
	SDL_Event event;
	const Uint8* keys;
	Entity player_paddle, enemy_paddle, ball, net, player_goal, enemy_goal, ceiling, floor;
	int player_score, enemy_score;

	GLuint LoadTexture(const char* image_path);
	void BallBounce(Entity* paddle);
};