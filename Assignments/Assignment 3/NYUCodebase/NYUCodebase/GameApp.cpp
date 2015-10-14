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
	
	float x = 3.55f;
	float y = 2.0f;
	projectionMatrix.setOrthoProjection(-x, x, -y, y, -1.0f, 1.0f);
	program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);
	
	//setup
	sprite_sheet_texture = LoadTexture("../graphics/sheet.png", GL_RGBA);
	background = LoadTexture("../graphics/purple.png", GL_RGB);
	font = LoadTexture("../graphics/TEXT.png", GL_RGBA);
	//player setup
	player.sprite = SheetSprite(sprite_sheet_texture, 211.0f / 1024.0f, 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.2);
	player.x = -1.0f;
	player.y = -1.7f;
	player.width = .15f;
	player.height = .15f;
	//enemy setup
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 5; j++){
			enemies.push_back(Entity());
			enemies[enemies.size()-1].x = -1.5 + .3*i;
			enemies[enemies.size()-1].y = 1.2 + .2*j;
			enemies[enemies.size()-1].sprite = SheetSprite(sprite_sheet_texture, 423.0f / 1024.0f, 728.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.2);
			enemies[enemies.size() - 1].width = .15;
			enemies[enemies.size() - 1].height = .15;
			enemies[enemies.size() - 1].x_acceleration = -.5;
		}
	}
	enemies_alive = enemies.size();
	//wall setup

	left_wall.x = -3.55;
	left_wall.y = 0;
	left_wall.width = 0.2;
	left_wall.height = 4;

	right_wall.x = 3.55;
	right_wall.y = 0;
	right_wall.width = 0.2;
	right_wall.height = 4;

	// bullet setup
	/*<SubTexture name = "laserBlue08.png" x = "596" y = "961" width = "48" height = "46" / >
		<SubTexture name = "laserBlue09.png" x = "434" y = "325" width = "48" height = "46" / >
		<SubTexture name = "laserBlue10.png" x = "740" y = "724" width = "37" height = "37" / >
		<SubTexture name = "laserBlue11.png" x = "698" y = "795" width = "38" height = "37" / >*/
	
	float bullet_size = .1;
	player_bullet[0] = SheetSprite(sprite_sheet_texture, 596.0 / 1024.0f, 961.0f / 1024.0f, 48.0f / 1024.0f, 46.0f / 1024.0f, bullet_size);
	player_bullet[1] = SheetSprite(sprite_sheet_texture, 434.0 / 1024.0f, 325.0f / 1024.0f, 48.0f / 1024.0f, 46.0f / 1024.0f, bullet_size);
	player_bullet[2] = SheetSprite(sprite_sheet_texture, 740.0 / 1024.0f, 724.0f / 1024.0f, 37.0f / 1024.0f, 37.0f / 1024.0f, bullet_size);
	player_bullet[3] = SheetSprite(sprite_sheet_texture, 698.0 / 1024.0f, 795.0f / 1024.0f, 38.0f / 1024.0f, 37.0f / 1024.0f, bullet_size);
	/*<SubTexture name = "laserRed08.png" x = "580" y = "661" width = "48" height = "46" / >
		<SubTexture name = "laserRed09.png" x = "602" y = "600" width = "48" height = "46" / >
		<SubTexture name = "laserRed10.png" x = "738" y = "650" width = "37" height = "36" / >
		<SubTexture name = "laserRed11.png" x = "737" y = "613" width = "37" height = "37" / >*/
	enemy_bullet[0] = SheetSprite(sprite_sheet_texture, 580.0 / 1024.0f, 661.0f / 1024.0f, 48.0f / 1024.0f, 46.0f / 1024.0f, bullet_size);
	enemy_bullet[1] = SheetSprite(sprite_sheet_texture, 602.0 / 1024.0f, 600.0f / 1024.0f, 48.0f / 1024.0f, 46.0f / 1024.0f, bullet_size);
	enemy_bullet[2] = SheetSprite(sprite_sheet_texture, 738.0 / 1024.0f, 650.0f / 1024.0f, 37.0f / 1024.0f, 36.0f / 1024.0f, bullet_size);
	enemy_bullet[3] = SheetSprite(sprite_sheet_texture, 737.0 / 1024.0f, 613.0f / 1024.0f, 38.0f / 1024.0f, 37.0f / 1024.0f, bullet_size);

	state = MAIN_MENU;
}

void GameApp::ProcessEvents(){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			done = true;
		}
	}
	keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_SPACE] && state == MAIN_MENU){ state = GAMEPLAY; }
	if (keys[SDL_SCANCODE_X] && state == GAME_OVER){ done = true; }
	if (keys[SDL_SCANCODE_SPACE] && recharge_timer > player_recharge && state == GAMEPLAY){
		Shoot_player_bullet();
		recharge_timer = 0.0f;
	}
	if (keys[SDL_SCANCODE_LEFT] && state == GAMEPLAY){
		player.x_acceleration = -10;
	}
	else if (keys[SDL_SCANCODE_RIGHT] && state == GAMEPLAY){
		player.x_acceleration = 10;
	}
	else {
		player.x_acceleration = 0;
	}
	//delete keys;
}
void GameApp::EnemyActions(){

}
void GameApp::Update(){
	switch (state){
	case MAIN_MENU:
		UpdateMainMenu();
		break;
	case GAMEPLAY:
		UpdateGamePlay();
		break;
	case GAME_OVER:
		UpdateGameOver();
	break;
	}
}
void GameApp::UpdateMainMenu(){
	}
void GameApp::UpdateGameOver(){

}

void GameApp::UpdateGamePlay(){
	recharge_timer += FIXED_TIMESTEP;
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++){
		player_bullets[i].Update(FIXED_TIMESTEP);
	}
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++){
		enemy_bullets[i].Update(FIXED_TIMESTEP);
	}
	
	for (int i = 0; i < enemies.size(); i++){
		if (enemies[i].is_Alive){
			enemies[i].UpdateX(FIXED_TIMESTEP);
			if (enemies[i].Collides(&left_wall)){
				enemies[i].x_velocity = 0;
				enemies[i].x_acceleration = -enemies[i].x_acceleration;
				enemies[i].y_acceleration += -.025;
				enemies[i].FixXPenetration(&left_wall);
			}
			else if (enemies[i].Collides(&right_wall)){
				enemies[i].x_velocity = 0;
				enemies[i].x_acceleration = -enemies[i].x_acceleration;
				enemies[i].y_acceleration += -.05;
				enemies[i].FixXPenetration(&right_wall);
			}
			enemies[i].UpdateY(FIXED_TIMESTEP);
			if (rand() % 1000 > 998 - (player_score / 5)){
				Shoot_enemy_bullet(&enemies[i]);
			}
			for (int j = 0; j < MAX_PLAYER_BULLETS; j++){
				if (player_bullets[j].visible && enemies[i].Is_shot(&player_bullets[j])){
					player_bullets[j].visible = false;
					enemies[i].x_acceleration = 0;
					enemies[i].y_acceleration = 0;
					enemies[i].y_velocity = 0;
					enemies[i].x_velocity = 0;
					enemies[i].is_Alive = false;
					player_score++;
					enemies_alive--;
					if (enemies_alive == 0){ 
						state = GAME_OVER; 
						player_win = true;
					}
				}
			}
		}
	}

	player.UpdateX(FIXED_TIMESTEP);
	if (player.Collides(&left_wall)){ 
		player.x_velocity = 0; 
		player.x_acceleration = 0;
		player.FixXPenetration(&left_wall);
	}
	else if (player.Collides(&right_wall)){
		player.x_velocity = 0;
		player.x_acceleration = 0;
		player.FixXPenetration(&right_wall);
	}
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++){
		if (enemy_bullets[i].visible && player.Is_shot(&enemy_bullets[i])){
			state = GAME_OVER;
		}
	}
}

void GameApp::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
	DrawBackGround();
	switch (state){
		case MAIN_MENU:
			RenderMainMenu();
		break;
		case GAMEPLAY:
			RenderGamePlay();
		break;
		case GAME_OVER:
			RenderGameOver();
		break;
	}
	SDL_GL_SwapWindow(displayWindow);	
}
void GameApp::RenderMainMenu(){
	modelMatrix.identity();
	modelMatrix.Translate(-2, 1, 0);
	program->setModelMatrix(modelMatrix);
	Drawtext(font, "Space Shooter", .3, .025);
	modelMatrix.Translate(0, -.5, 0);
	program->setModelMatrix(modelMatrix);
	Drawtext(font, "Press Space To Play", .2, .025);

}

void GameApp::RenderGameOver(){
	modelMatrix.identity();
	modelMatrix.Translate(-1, 1, 0);
	program->setModelMatrix(modelMatrix);
	Drawtext(font, "Game Over!", .2, .015);
	modelMatrix.Translate(.2, -1, 0);
	program->setModelMatrix(modelMatrix);
	if (player_win){
		Drawtext(font, "You win!", .2, .015);
	}
	else {
		Drawtext(font, "You lose!", .2, .015);
	}
	modelMatrix.Translate(-.6, -1, 0);
	program->setModelMatrix(modelMatrix);
	Drawtext(font, "Press x to exit", .2, .015);
}

void GameApp::RenderGamePlay() {
	for (int i = 0; i < MAX_PLAYER_BULLETS; i++){
		if (player_bullets[i].visible){
			player_bullets[i].Draw(program, &modelMatrix);
		}
	}
	for (int i = 0; i < MAX_ENEMY_BULLETS; i++){
		if (enemy_bullets[i].visible){
			enemy_bullets[i].Draw(program, &modelMatrix);
		}
	}
	player.Draw(program, &modelMatrix);
	for (int i = 0; i < enemies.size(); i++){
		if (enemies[i].is_Alive){
			enemies[i].Draw(program, &modelMatrix);
		}
	}
}

bool GameApp::UpdateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents();
	EnemyActions();
	
	float fixed_elapsed = elapsed + time_left_over;
	if (fixed_elapsed > MAX_TIMESTEP * FIXED_TIMESTEP){
		fixed_elapsed = MAX_TIMESTEP*FIXED_TIMESTEP;
	}
	while (fixed_elapsed >= FIXED_TIMESTEP){
		fixed_elapsed -= FIXED_TIMESTEP;
		Update();
	}
	time_left_over = fixed_elapsed;
	Render();
	return done;
}

GLuint GameApp::LoadTexture(const char* image_path, int img_type){
	// texture
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, img_type, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	SDL_FreeSurface(surface);
	return textureID;
}

void GameApp::Shoot_player_bullet(){
	player_bullets[player_bullet_index].visible = true;
	player_bullets[player_bullet_index].x = player.x;
	player_bullets[player_bullet_index].y = player.y;
	player_bullets[player_bullet_index].y_direction = 1;
	player_bullets[player_bullet_index].sprite = player_bullet;
	player_bullet_index++;
	if (player_bullet_index >= MAX_PLAYER_BULLETS){ player_bullet_index = 0; }
}

void GameApp::Shoot_enemy_bullet(Entity* enemy){
	enemy_bullets[enemy_bullet_index].visible = true;
	enemy_bullets[enemy_bullet_index].x = enemy->x;
	enemy_bullets[enemy_bullet_index].y = enemy->y;
	enemy_bullets[enemy_bullet_index].y_direction = -1;
	enemy_bullets[enemy_bullet_index].sprite = enemy_bullet;
	enemy_bullet_index++;
	if (enemy_bullet_index >= MAX_ENEMY_BULLETS){ enemy_bullet_index = 0; }
}

void GameApp::Drawtext(int fontTexture, std::string text, float size, float spacing){
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	
	for (int i = 0; i < text.size(); i++){
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		vertexData.insert(vertexData.end(), {
			((size + spacing)*i) + (-0.5f * size), 0.5f *size,
			((size + spacing)*i) + (-0.5f * size), -0.5f *size,
			((size + spacing)*i) + (0.5f * size), 0.5f *size,
			((size + spacing)*i) + (0.5f * size), -0.5f *size,
			((size + spacing)*i) + (0.5f * size), 0.5f *size,
			((size + spacing)*i) + (-0.5f * size), -0.5f *size,
		});

		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y ,
			texture_x, texture_y + texture_size,

		});
	}

	//glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}

void GameApp::DrawBackGround(){
	modelMatrix.identity();
	program->setModelMatrix(modelMatrix);
	glBindTexture(GL_TEXTURE_2D, background);
	GLfloat texcoords[] = {
		0, 4,
		4, 0,
		0, 0,
		4, 0,
		0, 4,
		4, 4
	};
	float vertices[] = {
		-3.55, -2,
		3.55, 2,
		-3.55, 2,
		3.55, 2,
		-3.55, -2,
		3.55, -2
	};
	/*float vertices[] = {
		-.5, -.5,
		.5, .5,
		-.5, .5,
		.5, .5,
		-.5, -.5,
		.5, -.5
	};*/
	//triangles
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);

	//texture
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texcoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, background);

	//blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//wrap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//finish
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}