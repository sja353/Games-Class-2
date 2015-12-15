#pragma once
#include "LightManager.h"
#include "TerrainTile.h"
#include <vector>
//class TerrainTile;
class Enemy;
// Handles arrays for representing and saving levels and some functions for generating them currently
class Level{
public:
	float get_x_spawn_position() { return x_spawn; }
	float get_y_spawn_position() { return y_spawn; }
	void modify_tile(int x, int y, unsigned char type);
	~Level(){
		for (int i = 0; i < height; i++){
			delete[] terrain_save_map[i];
			delete[] terrain_map[i];
			delete[] sprite_save_map[i];
		}
		delete[] terrain_save_map;
		delete[] terrain_map;
		delete[] sprite_save_map;
	}
	Level(){};
	Level(int tile_texture, int enemy_texture, ShaderProgram* program){
		this->tile_texture = tile_texture;
		this->enemy_texture = enemy_texture;
		this->program = program;
		terrain_save_map = new unsigned char*[height];
		terrain_map = new TerrainTile*[height];
		sprite_save_map = new unsigned char*[height];
		for (int i = 0; i < height; i++){
			terrain_save_map[i] = new unsigned char[width];
			terrain_map[i] = new TerrainTile[width];
			sprite_save_map[i] = new unsigned char[width];
		}
	}
	bool is_room_for_lava_pool(int i2, int j2, int length){
		for (int j = j2; j < j2 + length; j++){
			if (!(terrain_save_map[i2][j] == (unsigned char)1 && (terrain_save_map[i2 + 1][j] == (unsigned char)200))){ return false; }
		}
	}
	void generate(); 
	void render(int player_x, int player_y);
	float get_tilesize(){ return tilesize; }
	TerrainTile get_tile(int x, int y);
	void Level::get_enemies_to_draw(std::vector<Enemy*>* enemies_list);
	void set_tile(int x, int y, TerrainTile tile);
	void set_light_manager(LightManager* light_manager){ this->light_manager = light_manager; }
private:
	void split(int level, int max_depth, int begin_x, int end_x, int begin_y, int end_y);
	void form_platforms(int begin_x, int end_x, int begin_y, int end_y);
	
	void build_horizontal_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height);
	bool is_room_for_horizontal_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height);
	void build_vertical_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height);
	bool is_room_for_vertical_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height);
	void build_horizontal_hall_and_room_left(int x, int y, int hallway_length, int room_width, int room_height);
	bool is_room_for_horizontal_hall_and_room_left(int x, int y, int hallway_length, int room_width, int room_height);
	float x_spawn;
	float y_spawn;
	int width= 500;
	int height=20;
	float tilesize = .4;
	int max_room_height;
	int min_room_height;
	int max_room_width;
	int min_room_width;
	int vertical_drop_chance;
	int room_from_edge;
	int attempts_to_fit;
	unsigned char** terrain_save_map;
	unsigned char** sprite_save_map;
	TerrainTile** terrain_map;
	int tile_texture, enemy_texture;
	void convert_byte_map();
	ShaderProgram* program;
	LightManager* light_manager;
	TerrainTile convert_byte(int x, int y, unsigned char byte);
};
