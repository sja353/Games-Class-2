#include "GameApp.h"
#include "Entity.h"
#include <SDL_image.h>
#include <vector>
#include <SDL_mixer.h>
//#include "LevelModifier.h"

GameApp::GameApp(){
	Setup();
}
GameApp::~GameApp(){
	delete program;
	delete level;
	delete projectile_manager;
	delete audio;
	delete special_effects;
	delete light_manager;
	/*for (int i = 0; i < spawned_enemies.size(); i++){
		delete spawned_enemies[i];
		spawned_enemies.erase(spawned_enemies.begin() + i);
	}
	for (int i = 0; i < enemies.size(); i++){
		delete enemies[i];
		enemies.erase(enemies.begin() + i);
	}*/
}
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
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
    program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured_lights.glsl", RESOURCE_FOLDER"fragment_textured_lights.glsl");
	ShaderProgram* program2 = new ShaderProgram(RESOURCE_FOLDER"vertex.glsl", RESOURCE_FOLDER"fragments2.glsl");
	glUseProgram(program->programID);
	
	float x = 3.55f;
	float y = 2.0f;
	projectionMatrix.setOrthoProjection(-x, x, -y, y, -1.0f, 1.0f);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	//program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program2->setViewMatrix(viewMatrix);
	program2->setProjectionMatrix(projectionMatrix);
	//setup
	audio =new Audio();
	special_effects = new SpecialEffects(program2, &viewMatrix, &projectionMatrix);
	GLuint player_texture = LoadTexture("../graphics/player.png", GL_RGBA);
	GLuint level_texture = LoadTexture("../graphics/tiles.png", GL_RGBA);
	GLuint enemy_texture = LoadTexture("../graphics/enemies.png", GL_RGBA);
	GLuint particle_texture = LoadTexture("../graphics/fire.png", GL_RGBA);
	special_effects->set_texture(particle_texture);
	background = LoadTexture("../graphics/purple.png", GL_RGB);
	level = new Level(level_texture, enemy_texture, program);
	level->generate();
	light_manager = new LightManager();
	light_manager->initialize();
	projectile_manager = new ProjectileManager(special_effects);
	projectile_manager->set_light_manager(light_manager);
	
	player = Player(.2f, level->get_x_spawn_position(), level->get_y_spawn_position(), player_texture, program);
	player.set_hitbox(((66.0f/92.0f)*.2f)-.05, .2f);
	player.set_audio(audio);
	player.set_effects(special_effects);
	player.set_projectile_manager(projectile_manager);
	player.set_level_modifier(level);
	level->get_enemies_to_draw(&enemies);
	state = GAMEPLAY;

	
	audio->playMusic();
	//DBOUT(slug.get_width());
	//DBOUT(slug.get_height());
	//DBOUT(program);*/

}

void GameApp::ProcessEvents(){
	while (SDL_PollEvent(&event)){
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE){
			done = true;
		}
	}
	keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT] && state == GAMEPLAY){
		player.move_left();
	}
	else if (keys[SDL_SCANCODE_RIGHT] && state == GAMEPLAY){
		player.move_right();
	}
	if (keys[SDL_SCANCODE_UP]){ player.jump(); }
	if (keys[SDL_SCANCODE_DOWN]){ player.down_key(); }
	if (keys[SDL_SCANCODE_SPACE]) { player.shoot(); }
	if (keys[SDL_SCANCODE_X]){ player.create_tile(); }
	if (keys[SDL_SCANCODE_Z]){ player.Transform(); }
	else {
		player.idle();
	}
	//delete keys;
}
// Remember to add something here for erasing dead enemies from the vector
void GameApp::EnemyActions(){
	for (int i = 0; i < spawned_enemies.size(); i++){
		spawned_enemies[i]->get_behavior(player.get_x(), player.get_y());
	}
}


void GameApp::Update(float timestep){
	switch (state){
	case MAIN_MENU:
		UpdateMainMenu(timestep);
		break;
	case GAMEPLAY:
		UpdateGamePlay(timestep);
		break;
	case GAME_OVER:
		UpdateGameOver(timestep);
	break;
	}
}

void GameApp::UpdateMainMenu(float timestep){
	}
void GameApp::UpdateGameOver(float timestep){

}

void GameApp::UpdateGamePlay(float timestep){
	projectile_manager->update(timestep, level);
	player.update(timestep, level);
	special_effects->update(timestep);
	for (int i = 0; i < spawned_enemies.size(); i++){
		spawned_enemies[i]->update(timestep, level);
		if (spawned_enemies[i]->is_expired()){
			delete spawned_enemies[i];
			spawned_enemies.erase(spawned_enemies.begin() + i);
		}
		else{
			player.calculate_enemy_collision(spawned_enemies[i]);
			projectile_manager->check_shot_enemies(spawned_enemies[i]);
		}
	}
}

void GameApp::Render(){
	glClear(GL_COLOR_BUFFER_BIT);
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
}

void GameApp::RenderGameOver(){
}

void GameApp::RenderGamePlay() {
	DrawBackGround();
	projectile_manager->render();
	special_effects->render(0);
	for (int i = 0; i < enemies.size(); i++){
		if (!(enemies[i]->get_x_tile_position(level->get_tilesize()) < player.get_x_tile_position(level->get_tilesize())-20) 
		&& !(enemies[i]->get_x_tile_position(level->get_tilesize()) > player.get_x_tile_position(level->get_tilesize()) + 20)){
			spawned_enemies.push_back(enemies[i]);
			spawned_enemies.back()->set_audio(audio);
			enemies.erase(enemies.begin() + i);
		}
	}
	viewMatrix.identity();
	viewMatrix.Translate(-player.get_x(), -player.get_y() - 0.6, 0);
	program->setViewMatrix(viewMatrix);
	level->render(player.get_x_tile_position(level->get_tilesize()), player.get_y_tile_position(level->get_tilesize()));
	player.Draw();
	for (int i = 0; i < spawned_enemies.size(); i++){
		spawned_enemies[i]->Draw();
	}
	light_manager->draw_lights(program, player.get_x(), player.get_y());
}

bool GameApp::UpdateAndRender(){
	
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	ProcessEvents();
	EnemyActions();
	
	float fixed_elapsed = elapsed;
	if (fixed_elapsed > MAX_TIMESTEP * FIXED_TIMESTEP){
		fixed_elapsed = MAX_TIMESTEP*FIXED_TIMESTEP;
	}
	while (fixed_elapsed >= FIXED_TIMESTEP){
		fixed_elapsed -= FIXED_TIMESTEP;
		Update(FIXED_TIMESTEP);
	}
	Update(fixed_elapsed);
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
		player.get_x()-3.55, player.get_y()-2+.6,
		player.get_x()+3.55, player.get_y()+2+.6,
		player.get_x()-3.55, player.get_y()+2+.6,
		player.get_x()+3.55, player.get_y()+2+.6,
		player.get_x()-3.55, player.get_y()-2+.6,
		player.get_x()+3.55, player.get_y()-2+.6
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