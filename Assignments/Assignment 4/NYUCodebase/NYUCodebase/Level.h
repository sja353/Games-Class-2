#ifndef LEVEL_HEADER
#define LEVEL_HEADER

#include "TerrainTile.h"
#include <vector>
class Enemy;

class Level{
public:
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
	void generate(); 
	void render(int player_x, int player_y);
	float get_tilesize(){ return tilesize; }
	TerrainTile get_tile(int x, int y);
	void Level::get_enemies_to_draw(std::vector<Enemy>* enemies_list);

private:
	int width= 250;
	int height=30;
	float tilesize = .4;
	unsigned char** terrain_save_map;
	unsigned char** sprite_save_map;
	TerrainTile** terrain_map;
	int tile_texture, enemy_texture;
	void convert_byte_map();
	ShaderProgram* program;
	TerrainTile convert_byte(int x, int y, unsigned char byte);
};

#endif