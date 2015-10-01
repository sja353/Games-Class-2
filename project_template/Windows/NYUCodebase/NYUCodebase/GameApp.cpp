#include "GameApp.h"
#include "Entity.h"
#include <SDL_image.h>

GameApp::GameApp(){
	Setup();
}
GameApp::~GameApp(){
	delete program;
}
void GameApp::Setup(){
	#ifdef _WINDOWS
		#define RESOURCE_FOLDER ""
	#else
		#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
	#endif
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	#ifdef _WINDOWS
		glewInit();
	#endif
	glViewport(0, 0, 1280, 720);
    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program->programID);
	
	float x = 3.55;
	float y = 2.0;
	projectionMatrix.setOrthoProjection(-x, x, -y, y, -1.0f, 1.0f);
	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);
	
	float thin = .25f;
	float thick = .5f;

	// Player paddle setup
	player_paddle.x = -1.5;
	player_paddle.y = 0.0f;
	player_paddle.width = thin;
	player_paddle.height = thick*2;
	player_paddle.speed = 1.0f;
	player_paddle.y_direction = 0;
	player_paddle.x_direction = 0;
	player_paddle.textureID = LoadTexture("../../../../assets/graphics/Platformer Bricks (150 assets)/PNG/boxEmpty.png");

	//Enemy paddle setup
	enemy_paddle.x = 1.5;
	enemy_paddle.y = -2.0f;
	enemy_paddle.width = thin;
	enemy_paddle.height = thick*2;
	enemy_paddle.speed = 1.0f;
	enemy_paddle.y_direction = 0;
	enemy_paddle.x_direction = 0;
	enemy_paddle.textureID = LoadTexture("../../../../assets/graphics/Platformer Bricks (150 assets)/PNG/boxEmpty.png");

	// Ball setup 
	ball.x = 0.0f;
	ball.y = 0.0f;
	ball.width = thin;
	ball.height = thin;
	ball.x_direction = -1.0f;
	ball.y_direction = 0.0f;
	ball.speed = .5f;
	ball.textureID = LoadTexture("../../../../assets/graphics/Space Shooter assets (300 assets)/PNG/Power-ups/powerupRed.png");

	// Floor setup
	floor.x = 0.0f;
	floor.y = -1;
	floor.width = 2*x;
	floor.height = thin;
	floor.textureID = LoadTexture("../../../../assets/graphics/Space Shooter assets (300 assets)/PNG/UI/buttonBlue.png");

	// Ceiling setup
	ceiling.x = 0.0f;
	ceiling.y = 1;
	ceiling.width = 2*x;
	ceiling.height = thin;
	ceiling.textureID = LoadTexture("../../../../assets/graphics/Space Shooter assets (300 assets)/PNG/UI/buttonBlue.png");

	// goal setup
	player_goal.x = -1.7;
	player_goal.y = 0.0f;
	player_goal.width = thin;
	player_goal.height = (2 * y)+2;
	player_goal.textureID = LoadTexture("../../../../assets/graphics/Space Shooter assets (300 assets)/PNG/Lasers/laserBlue14.png");

	enemy_goal.x = 1.7;
	enemy_goal.y = 0.0f;
	enemy_goal.width = thin;
	enemy_goal.height = (2 * y)+2;
	enemy_goal.textureID = LoadTexture("../../../../assets/graphics/Space Shooter assets (300 assets)/PNG/Lasers/laserBlue14.png");

	// Net setup 
	net.x = 0.0f;
	net.y = 0.0f;
	net.width = thin;
	net.height = (2 * y) +2;
	net.textureID = LoadTexture("../../../../assets/graphics/Space Shooter assets (300 assets)/PNG/Lasers/laserRed15.png");


	player_score = 0;
	enemy_score = 0;
}

void GameApp::ProcessEvents(){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			done = true;
		}
	}
//	delete keys;
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_UP]){
		player_paddle.y_direction = 1;
	}
	else if (keys[SDL_SCANCODE_DOWN]){
		player_paddle.y_direction = -1;
	}
	else {
		player_paddle.y_direction = 0;
	}
}

void GameApp::BallBounce(Entity* paddle){
	float x = ball.x_direction * sqrt(powf(ball.x_direction, 2) + powf(ball.y_direction, 2));
	float y = ball.y_direction * sqrt(powf(ball.y_direction, 2) + powf(ball.x_direction, 2));
	y += (ball.y - paddle->y);
	ball.x_direction = - (x / sqrt(powf(x, 2) + powf(y, 2)));
	ball.y_direction = (y / sqrt(powf(x, 2) + powf(y, 2)));
}

void GameApp::Update(){
	
	// Ball collision 
	if (ball.Collides(&floor) || ball.Collides(&ceiling)){
		ball.y_direction -= 2*ball.y_direction;
		ball.speed += .01;
	}
	else if (ball.Collides(&player_paddle)){
		BallBounce(&player_paddle);
		ball.speed += .02;
	}
	else if (ball.Collides(&enemy_paddle)){
		BallBounce(&enemy_paddle);
		ball.speed += .02;
	}
	else if (ball.Collides(&player_goal)){
		enemy_score++;
		ball.x = 0.0;
		ball.y = 0.0;
		ball.speed = 0.5;
		ball.x_direction = -1.0;
		ball.y_direction = 0;
	}
	else if (ball.Collides(&enemy_goal)){
		ball.speed = 0.5;
		ball.x = 0.0;
		ball.y = 0.0;
		ball.x_direction = -1.0;
		ball.y_direction = 0;
		player_score++;
	}

	// Simple enemy AI
	if (enemy_paddle.y > ball.y){
		enemy_paddle.y_direction = -1;
	}
	else if (enemy_paddle.y < ball.y){
		enemy_paddle.y_direction = 1;
	}	
	
	// Paddle collision
	if (player_paddle.Collides(&ceiling)){
		player_paddle.y_direction = -0.1;
	}
	if (player_paddle.Collides(&floor)){
		player_paddle.y_direction = 0.1;
	}
	if (enemy_paddle.Collides(&ceiling)) {
		enemy_paddle.y_direction = -0.1;
	}
	if (enemy_paddle.Collides(&floor)){
		enemy_paddle.y_direction = 0.1;
	}
}

void GameApp::Render(float elapsed){
	if (ball.speed > 2){
		ball.speed -= .005f *elapsed;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	ceiling.Draw(elapsed, program, &modelMatrix);
	floor.Draw(elapsed, program, &modelMatrix);
	player_goal.Draw(elapsed, program, &modelMatrix);
	enemy_goal.Draw(elapsed, program, &modelMatrix);
	net.Draw(elapsed, program, &modelMatrix);
	player_paddle.Draw(elapsed, program, &modelMatrix);
	enemy_paddle.Draw(elapsed, program, &modelMatrix);
	ball.Draw(elapsed, program, &modelMatrix);
	SDL_GL_SwapWindow(displayWindow);
}

bool GameApp::UpdateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents();
	Update();
	Render(elapsed);
	return done;
}

GLuint GameApp::LoadTexture(const char* image_path){
	// texture
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SDL_FreeSurface(surface);
	return textureID;
}

