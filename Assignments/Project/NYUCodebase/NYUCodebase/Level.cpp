#include "Level.h"
#include "Slug.h"
#include<vector>
#include "Enemy.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

void Level::get_enemies_to_draw(std::vector<Enemy>* enemy_list){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (sprite_save_map[i][j] == (unsigned char)1){
				Slug slug = Slug(0.1f, j*tilesize, i*tilesize, enemy_texture, program);
				slug.set_hitbox(0.3f, 0.1f);
				enemy_list->push_back(slug);
			}
		}
	}
}

TerrainTile Level::convert_byte(int y, int x, unsigned char byte){
	float sheet_width = 914.0f;
	float sheet_height = 936.0f;
	TerrainTile tile;
	//tile.exists = false;
	if (byte == (unsigned char)1){
		//"stoneCenter.png" x="144" y="576" width="70" height="70"
		Sheetposition position = Sheetposition(144.0f, 576.0f, 70.0f, 70.0f, tilesize, sheet_width, sheet_height);
		tile = TerrainTile(x*tilesize + tilesize/2, y*tilesize +tilesize/2, tile_texture, position, program);
		tile.set_behaviors(true, true, true, true);
		tile.set_exists(true);
		tile.set_hitbox(tilesize, tilesize);

	}
	else if (byte == (unsigned char)2){
		//"stoneMid.png" x="72" y="432" width="70" height="70"
		Sheetposition position = Sheetposition(72.0f, 432.0f, 70.0f, 70.0f, tilesize, sheet_width, sheet_height);
		tile = TerrainTile(x * tilesize + tilesize /2, y*tilesize + tilesize /2, tile_texture, position, program);
		tile.set_behaviors(true, true , true, true);
		tile.set_exists(true);
		tile.set_hitbox(tilesize, tilesize);

	}
	return tile;
}

void Level::render(int player_x, int player_y){
	std::vector<TerrainTile> to_render;
	int left_bound, right_bound, lower_bound, upper_bound;
	int render_length = 15;
	if (player_x - render_length < 0) { left_bound = 0; }
	else { left_bound = player_x - render_length; }
	if (player_x + render_length > width){ right_bound = width; }
	else { right_bound = player_x + render_length; }
	if (player_y - render_length < 0){ lower_bound = 0; }
	else { lower_bound = player_y - render_length; }
	if (player_y + render_length > height){ upper_bound = height; }
	else { upper_bound = player_y + render_length; }
	for (int i = lower_bound; i < upper_bound; i++){
		for (int j = left_bound; j < right_bound; j++){
			if (terrain_map[i][j].is_there()){terrain_map[i][j].Draw();}
		}
	}
	to_render.clear();
}
TerrainTile Level::get_tile(int x, int y){
	TerrainTile tile;
	if (!(x < 0) && !(x >= width) && !(y < 0) && !(y >= height)){
		tile = terrain_map[y][x];
	}
	return tile;
}
void Level::generate() {
	
	int previous_floor= 10;
	int length_counter = 0;
	int floor = rand() % 20 +1;
	int length = rand() % 10 + 1;
	length_counter = 0;
	while (length_counter < width){
		if (floor > previous_floor + 2) { floor = previous_floor + 2; }
		if (floor > height - 3){ floor = height - 3; }
		if (floor < 1) { floor = 1; }
		if (length_counter + length > width){ length = width - length_counter - 1; }
		for (int i = 0; i <= floor; i++){
			for (int j = length_counter; j < length_counter+length; j++){
				if (i < floor ){ terrain_save_map[i][j] = (unsigned char)1; }
				else if (i == floor){ terrain_save_map[i][j] = (unsigned char)2; }
				//else if (i < 10) { terrain_save_map[i][j] = (unsigned char)1; }
				else { terrain_save_map[i][j] = (unsigned char)8; }
			}
		}
		length_counter += length;
		previous_floor = floor;
		if (length_counter > width){ length_counter = width; }
		length = rand() % 10 + 1;
		floor = rand() % 20 + 1;
	}
	convert_byte_map();
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (!terrain_map[i][j].is_there() && rand() % 100 < 1){
				sprite_save_map[i][j] = (unsigned char)1;
			}
		}
	}

}

void Level::convert_byte_map(){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			terrain_map[i][j] = convert_byte(i, j, terrain_save_map[i][j]);
		}
	}
}
