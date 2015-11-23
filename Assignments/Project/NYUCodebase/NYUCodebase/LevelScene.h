#pragma once 
#include "Matrix.h"
#include "ShaderProgram.h"
#include <SDL.h>
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
//#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
//#include "Shaderprogram.h"
#include "Matrix.h"
#include "Player.h"
#include "Level.h"
#include "Slug.h"
#include "Audio.h"
#include "SpecialEffects.h"
//Plan on moving a lot of the code in GameApp.cpp here to make it less of a mess
class LevelScene{
/*public:
	LevelScene(ShaderProgram* program){
		this->program = program;
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
		audio = Audio();
		special_effects = SpecialEffects(program);
		GLuint player_texture = LoadTexture("../graphics/p1_spritesheet.png", GL_RGBA);
		GLuint level_texture = LoadTexture("../graphics/tiles_spritesheet.png", GL_RGBA);
		GLuint enemy_texture = LoadTexture("../graphics/slugs.png", GL_RGBA);
		background = LoadTexture("../graphics/purple.png", GL_RGB);
		level = new Level(level_texture, enemy_texture, program);
		level->generate();
		player = Player(.2f, 10 * .4f, 30 * .4f, player_texture, program);
		player.set_hitbox(((66.0f / 92.0f)*.2f) - .05, .2f);
		player.set_audio(&audio);
		player.set_effects(&special_effects);
		level->get_enemies_to_draw(&enemies);
	}
	void update(float time_elapsed){
	}
private:
	GLuint LoadTexture(const char* image_path, int img_type){
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

	Player player;
	std::vector<Enemy> enemies;
	std::vector<Enemy> spawned_enemies;
	ShaderProgram* program;
	SpecialEffects special_effects;
	Audio audio;
	GLuint background;
	Level* level;*/
};
