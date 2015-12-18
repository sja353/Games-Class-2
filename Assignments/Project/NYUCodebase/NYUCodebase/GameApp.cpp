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
	for (int i = 0; i < enemies.size(); i++){
		delete enemies[i];
		enemies.erase(enemies.begin() + i);
	}
	for (int i = 0; i < spawned_enemies.size(); i++){
		delete spawned_enemies[i];
		spawned_enemies.erase(spawned_enemies.begin() + i);
	}
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
	ShaderProgram* program3 = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program3->programID);
	
	float x = 3.55f;
	float y = 2.0f;
	projectionMatrix.setOrthoProjection(-x, x, -y, y, -1.0f, 1.0f);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	//program->setModelMatrix(modelMatrix);
	program->setViewMatrix(viewMatrix);
	program->setProjectionMatrix(projectionMatrix);
	program2->setViewMatrix(viewMatrix);
	program2->setProjectionMatrix(projectionMatrix);
	program3->setViewMatrix(viewMatrix);
	program3->setProjectionMatrix(projectionMatrix);
	//setup
	audio =new Audio();
	special_effects = new SpecialEffects(program2, &viewMatrix, &projectionMatrix);
	GLuint player_texture = LoadTexture("../graphics/player.png", GL_RGBA);
	GLuint level_texture = LoadTexture("../graphics/tiles.png", GL_RGBA);
	GLuint enemy_texture = LoadTexture("../graphics/enemies.png", GL_RGBA);
	GLuint particle_texture = LoadTexture("../graphics/particles.png", GL_RGBA);
	GLuint overlay_texture = LoadTexture("../graphics/overlay.png", GL_RGBA);
	GLuint font_texture = LoadTexture("../graphics/TEXT.png", GL_RGBA);
	info_overlay = InfoOverlay(overlay_texture, font_texture, program3);
	special_effects->set_texture(particle_texture);
	background = LoadTexture("../graphics/purple.png", GL_RGB);
	
	light_manager = new LightManager();
	light_manager->initialize(program);
	level = new Level(level_texture, enemy_texture, program);
	level->set_light_manager(light_manager);
	level->set_special_effects(special_effects);
	level->set_audio(audio);
	level->generate();
	projectile_manager = new ProjectileManager(special_effects);
	projectile_manager->set_light_manager(light_manager);
	
	player = Player(.3f, level->get_x_spawn_position(), level->get_y_spawn_position(), player_texture, program);
	player.set_hitbox(((66.0f/92.0f)*.3f)-.05, .3f);
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
	if (keys[SDL_SCANCODE_UP]){ 
		player.jump(); 
	}
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
	audio->update(timestep);
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
	//DrawBackGround();
	program->setModelMatrix(modelMatrix);
	level->render(player.get_x_tile_position(level->get_tilesize()), player.get_y_tile_position(level->get_tilesize()));
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
	
	player.Draw();
	special_effects->render(1);
	for (int i = 0; i < spawned_enemies.size(); i++){
		spawned_enemies[i]->Draw();
	}
	light_manager->draw_lights(player.get_x(), player.get_y());
	info_overlay.DrawOverlay(player.get_current_health(), player.get_max_health(),
		player.get_current_mana(), player.get_max_mana(), player.get_position_vector());
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


void GameApp::DrawBackGround(){
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