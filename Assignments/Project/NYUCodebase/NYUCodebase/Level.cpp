#include "Level.h"
#include "Slug.h"
#include "RedMonster.h"
#include<vector>
#include "Enemy.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

void Level::get_enemies_to_draw(std::vector<Enemy*>* enemy_list){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (sprite_save_map[i][j] == (unsigned char)1){
				float r = (float)rand() / RAND_MAX;
				float g = (float)rand() / RAND_MAX;
				float b = (float)rand() / RAND_MAX;
				Slug* slug = new Slug(0.2f, j*tilesize, i*tilesize, enemy_texture, program, r, g, b, light_manager);
				slug->set_hitbox(0.3f, 0.2f);
				slug->set_level(this);
				enemy_list->push_back(slug);
				
			}
			if (sprite_save_map[i][j] == (unsigned char)2){
				RedMonster* red_monster = new RedMonster(0.3f, j*tilesize, i*tilesize, enemy_texture, program);
				red_monster->set_hitbox(0.3f, 0.27f);
				enemy_list->push_back(red_monster);
			}
		}
	}
}

TerrainTile Level::convert_byte(int y_coord, int x_coord, unsigned char byte){

	float sheet_width = 1100.0f;
	float sheet_height = 1100.0f;
	TerrainTile tile;
	bool triggered = false;
	float x = 0.0f;
	float y = 0.0f;
	bool is_lit = false;
	Light*light;
	bool hurts_top, hurts_bottom, hurts_right, hurts_left;
	hurts_top = hurts_bottom = hurts_right = hurts_left = false;
	bool destructible = true;
	int damage = 0;
	bool win_tile = false;
	if (byte == (unsigned char)1){
		//bare stone
		/*<SubTexture name = "tile2.png" x = "216" y = "432" width = "70" height = "70" / >
		<SubTexture name = "tile3.png" x = "72" y = "360" width = "70" height = "70" / >
		<SubTexture name = "tile4.png" x = "432" y = "0" width = "70" height = "70" / >*/
		int roll = rand() % 3;
		Sheetposition position;
		if (roll == 1){ 
			x=216.0f;
			y = 432.0f;
		}
		else if (roll == 2){  
			x = 72.0f;
			y = 360.0f;
		}
		else { 
			x = 432.0f;
			y = 0.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)2){
		// grass on top
		/*<SubTexture name = "tile3grass.png" x = "72" y = "288" width = "70" height = "70" / >
		<SubTexture name = "tile2grass.png" x = "216" y = "360" width = "70" height = "70" / >
		<SubTexture name = "tile4grass.png" x = "360" y = "432" width = "70" height = "70" / >*/
		int roll = rand() % 3;
		if (roll == 1){ 
			x = 72.0f;
			y = 288.0f;
		}
		else if (roll == 2){ 
			x = 216.0f; 
			y = 360.0f;
		}
		else { 
			x = 360.0f;
			y = 432.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)3){
		// grass on bottom
		/*<SubTexture name="tile4grassbottom.png" x="360" y="360" width="70" height="70"/>
			<SubTexture name="tile3grassbottom.png" x="72" y="216" width="70" height="70"/>
		<SubTexture name="tile2grassbottom.png" x="216" y="288" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){ 
			x = 360.0f; 
			y = 360.0f;
		}
		else if (roll == 2){ 
			x=72.0f; 
			y = 216.0f;
		}
		else {
			x = 216.0f;
			y = 288.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)4){
		// grass on left
		/*<SubTexture name="tile2grassleft.png" x="216" y="0" width="70" height="70"/>
		<SubTexture name="tile3grassleft.png" x="0" y="432" width="70" height="70"/>
		<SubTexture name="tile4grassleft.png" x="360" y="72" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 216.0f;
			y = 0.0f;
		}
		else if (roll == 2){
			x = 0.0f;
			y = 432.0f;
		}
		else {
			x = 360.0f;
			y = 72.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)5){
		// grass on right
		/*<SubTexture name="tile4grassright.png" x="288" y="432" width="70" height="70"/>
		<<SubTexture name="tile2grassright.png" x="144" y="432" width="70" height="70"/>
		<SubTexture name="tile3grassright-.png" x="0" y="360" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 288.0f;
			y = 432.0f;
		}
		else if (roll == 2){
			x = 144.0f;
			y = 432.0f;
		}
		else {
			x = 0.0f;
			y = 360.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)6){
		// grass on top, left
		/*<SubTexture name="tile2grasstopleft.png" x="144" y="0" width="70" height="70"/>
		<SubTexture name="tile3grasstopleft.png" x="216" y="72" width="70" height="70"/>
		<SubTexture name="tile4grasstopleft.png" x="288" y="72" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 144.0f;
			y = 0.0f;
		}
		else if (roll == 2){
			x = 216.0f;
			y = 72.0f;
		}
		else {
			x = 288.0f;
			y = 72.0f;
		}
		triggered = true;
	}

	else if (byte == (unsigned char)7){
		// grass on top, right
		/*<SubTexture name="tile4grasstopright.png" x="288" y="0" width="70" height="70"/>
		<SubTexture name="tile3grasstopright.png" x="432" y="72" width="70" height="70"/>
		<SubTexture name="tile2grasstopright.png" x="72" y="432" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 288.0f;
			y = 0.0f;
		}
		else if (roll == 2){
			x = 432.0f;
			y = 72.0f;
		}
		else {
			x = 72.0f;
			y = 432.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)8){
		// grass on top, bottom
		/*<SubTexture name="tile2grasstopbottom.png" x="144" y="288" width="70" height="70"/>
		<SubTexture name="tile3grasstopbottom.png" x="0" y="216" width="70" height="70"/>
		<SubTexture name="tile4grasstopbottom.png" x="288" y="360" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 144.0f;
			y = 288.0f;
		}
		else if (roll == 2){
			x = 0.0f;
			y = 216.0f;
		}
		else {
			x = 288.0f;
			y = 360.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)9){
		// grass on bottom, left 
		/*<SubTexture name="tile4grassbottomleft.png" x="360" y="288" width="70" height="70"/>
		<SubTexture name="tile3grassbottomleft.png" x="72" y="144" width="70" height="70"/>
		<SubTexture name="tile2grassbottomleft.png" x="216" y="216" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 360.0f;
			y = 288.0f;
		}
		else if (roll == 2){
			x = 72.0f;
			y = 144.0f;
		}
		else {
			x = 216.0f;
			y = 216.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)10){
		// grass on bottom, right
		/*<SubTexture name="tile2grassbottomright.png" x="216" y="144" width="70" height="70"/>
		<SubTexture name="tile3grassbottomright.png" x="72" y="72" width="70" height="70"/>
		<SubTexture name="tile4grassbottomright.png" x="360" y="144" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 216.0f;
			y = 144.0f;
		}
		else if (roll == 2){
			x = 72.0f;
			y = 72.0f;
		}
		else {
			x = 360.0f;
			y = 144.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)11){
		// grass on left, right
		/*<SubTexture name="tile3grassrighteft.png" x="0" y="288" width="70" height="70"/>
		<SubTexture name="tile4grassleftright.png" x="360" y="0" width="70" height="70"/>
		SubTexture name="tile2grassrightleft.png" x="144" y="360" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 0.0f;
			y = 288.0f;
		}
		else if (roll == 2){
			x = 360.0f;
			y = 0.0f;
		}
		else {
			x = 144.0f;
			y = 360.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)12){
		// grass on top, left, right - NOT DONE
		/*<SubTexture name="tile2grasstopleftright.png" x="0" y="586" width="70" height="70"/>
	<SubTexture name="tile3grasstoprighteft.png" x="0" y="658" width="70" height="70"/>
	<SubTexture name="tile4grasstopleftright.png" x="0" y="514" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 0.0f;
			y = 586.0f;
		}
		else if (roll == 2){
			x = 0.0f;
			y = 658.0f;
		}
		else {
			x = 0.0f;
			y = 514.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)13){
		// grass on top, left, bottom
		/*<SubTexture name="tile2grasstopbottomleft.png" x="144" y="216" width="70" height="70"/>
		<SubTexture name="tile3grasstopbottomleft.png" x="0" y="144" width="70" height="70"/>
		<SubTexture name="tile4grasstopbottomleft.png" x="288" y="288" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 144.0f;
			y = 216.0f;
		}
		else if (roll == 2){
			x = 0.0f;
			y = 144.0f;
		}
		else {
			x = 288.0f;
			y = 288.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)14){
		// grass on top, right, bottom
		/*<SubTexture name="tile4grasstopbottomright.png" x="288" y="216" width="70" height="70"/>
		<<SubTexture name="tile3grasstopbottomright.png" x="0" y="72" width="70" height="70"/>
		<SubTexture name="tile2grasstopbottomright.png" x="144" y="144" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 288.0f;
			y = 216.0f;
		}
		else if (roll == 2){
			x = 0.0f;
			y = 72.0f;
		}
		else {
			x = 144.0f;
			y = 144.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)15){
		// grass on bottom, left, right
		/*<SubTexture name="tile2grassbottomrightleft.png" x="432" y="144" width="70" height="70"/>
		<SubTexture name="tile3grassbottomrighteft.png" x="72" y="0" width="70" height="70"/>
		<SubTexture name="tile4grassbottomleftright.png" x="360" y="216" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 432.0f;
			y = 144.0f;
		}
		else if (roll == 2){
			x = 72.0f;
			y = 0.0f;
		}
		else {
			x = 360.0f;
			y = 216.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)16){
		// grass on all sides
		/*<SubTexture name="tile3grasstopbottomrightleft.png" x="0" y="0" width="70" height="70"/>
		<SubTexture name="tile4grasstopbottomrightleft.png" x="288" y="144" width="70"
		<<SubTexture name="tile2grasstopbottomrightleft.png" x="144" y="72" width="70" height="70"/>*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 0.0f;
			y = 0.0f;
		}
		else if (roll == 2){
			x = 288.0f;
			y = 144.0f;
		}
		else {
			x = 144.0f;
			y = 72.0f;
		}
		triggered = true;
	}
	else if (byte == (unsigned char)17){
		// spike on top tile
		//<SubTexture name="spiketile.png" x="0" y="1026" width="70" height="70"/>
		x = 0.0f;
		y = 1026.0f;
		triggered = true;
		hurts_top = true;
		damage = 1;
	}
	else if (byte == (unsigned char)18){
		// burnt tile
		//<SubTexture name="burnttile.png" x="72" y="1026" width="70" height="70"/>
		x = 72.0f;
		y = 1026.0f;
		triggered = true;
		destructible = false;
	}
	else if (byte == (unsigned char)19) {
		// lava tile 
		//	<SubTexture name="lavatile.png" x="144" y="1026" width="70" height="70"/>
		x = 144.0f;
		y = 1026.0f;
		triggered = true;
		hurts_top = hurts_bottom = hurts_right = hurts_left = true;
		damage = 2;
		destructible = false;
	}
	else if (byte == (unsigned char)20){
		// skull tile
		//<SubTexture name="skull.png" x="216" y="1026" width="70" height="70"/>
		x = 216.0f;
		y = 1026.0f;
		triggered = true;
	}
	else if (byte == (unsigned char)21){
		//background
		/*<SubTexture name = "background1.png" x = "1028" y = "0" width = "70" height = "70" / >
		<SubTexture name = "background2.png" x = "1028" y = "72" width = "70" height = "70" / >
		<SubTexture name = "background3.png" x = "1028" y = "144" width = "70" height = "70" / >*/
		int roll = rand() % 3;
		if (roll == 1){
			x = 1028;
			y = 0;
		}
		else if (roll == 2){
			x = 1028;
			y = 72;
		}
		else{
			x = 1028;
			y = 144;
		}
	}
	else if (byte == (unsigned char)22){
		//win tile
		x = 0.0f;
		y = 0.0f;
		Vector exit_position;
		exit_position.set_x((float)x_coord * tilesize + tilesize/2);
		exit_position.set_y((float)y_coord * tilesize + tilesize/2);
		special_effects->exit_effect(exit_position);
		win_tile = true;
		triggered = true;
		destructible = false;

	}
	
	Sheetposition position;
	position = Sheetposition(x, y, 70.0f, 70.0f, tilesize, sheet_width, sheet_height);
	tile = TerrainTile(x_coord*tilesize + tilesize / 2, y_coord*tilesize + tilesize / 2, tile_texture, position, program);
	tile.set_behaviors(true, true, true, true);
	tile.set_harmful_behaviors(hurts_top, hurts_bottom, hurts_right, hurts_left);
	tile.set_exists(triggered);
	tile.set_hitbox(tilesize, tilesize);
	tile.set_hp(10);
	tile.set_damage(damage);
	tile.set_destructible(destructible);
	tile.set_win_tile(win_tile);
	return tile;
}

void Level::render(int player_x, int player_y, bool draw_level){

	//std::vector<TerrainTile> to_render;
	std::vector<float> tile_vertices;
	std::vector<float> tile_texcoords;
	std::vector<float> background_tile_vertices;
	std::vector<float> background_tile_texcoords;
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
			float u_plus_b = background_map[i][j].get_sheet_position().u + background_map[i][j].get_sheet_position().width;
			float v_plus_b = background_map[i][j].get_sheet_position().v + background_map[i][j].get_sheet_position().height;
			background_tile_texcoords.insert(background_tile_texcoords.end(), {
				background_map[i][j].get_sheet_position().u, v_plus_b,
				u_plus_b, background_map[i][j].get_sheet_position().v,
				background_map[i][j].get_sheet_position().u, background_map[i][j].get_sheet_position().v,
				u_plus_b, background_map[i][j].get_sheet_position().v,
				background_map[i][j].get_sheet_position().u, v_plus_b,
				u_plus_b, v_plus_b
			});
			background_tile_vertices.insert(background_tile_vertices.end(), {
				tilesize*j, tilesize*i,
				tilesize*j + tilesize, tilesize*i + tilesize,
				tilesize*j, tilesize*i + tilesize,
				tilesize*j + tilesize, tilesize*i + tilesize,
				tilesize*j, tilesize*i,
				tilesize*j + tilesize, tilesize*i
			});
			
			if (terrain_map[i][j].is_there()){
				/*if (terrain_map[i][j].is_lit()){
					terrain_map[i][j].set_light_active();
				}*/
				float u_plus = terrain_map[i][j].get_sheet_position().u + terrain_map[i][j].get_sheet_position().width;
				float v_plus = terrain_map[i][j].get_sheet_position().v + terrain_map[i][j].get_sheet_position().height;
				tile_texcoords.insert(tile_texcoords.end(), {
					terrain_map[i][j].get_sheet_position().u, v_plus,
					u_plus, terrain_map[i][j].get_sheet_position().v,
					terrain_map[i][j].get_sheet_position().u, terrain_map[i][j].get_sheet_position().v,
					u_plus, terrain_map[i][j].get_sheet_position().v,
					terrain_map[i][j].get_sheet_position().u, v_plus,
					u_plus, v_plus
				});
				tile_vertices.insert(tile_vertices.end(), {
					tilesize*j, tilesize*i,
					tilesize*j + tilesize, tilesize*i + tilesize,
					tilesize*j, tilesize*i + tilesize,
					tilesize*j + tilesize, tilesize*i + tilesize,
					tilesize*j, tilesize*i,
					tilesize*j + tilesize, tilesize*i
				});

				//terrain_map[i][j].Draw();
			}
		}
	}
	
	//glBindTexture(GL_TEXTURE_2D, tile_texture);
	glUniform3fv(coloruniform, 1, color_shift);
	glUseProgram(program->programID);
	//triangles
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, background_tile_vertices.data());
	glEnableVertexAttribArray(program->positionAttribute);

	//texture
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, background_tile_texcoords.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, tile_texture);

	//blend
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//finish
	glDrawArrays(GL_TRIANGLES, 0, background_tile_vertices.size()/2);

	if (draw_level){
		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, tile_vertices.data());
		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, tile_texcoords.data());
		glDrawArrays(GL_TRIANGLES, 0, tile_vertices.size() / 2);
	}
	//
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
	//to_render.clear();
}
TerrainTile Level::get_tile(int x, int y){
	TerrainTile tile;
	if (!(x < 0) && !(x >= width) && !(y < 0) && !(y >= height)){
		tile = terrain_map[y][x];
	}
	return tile;
}

void Level::set_tile(int x, int y, TerrainTile tile){
	terrain_map[y][x] = tile;
}

bool Level::modify_tile(int x, int y, unsigned char type){
	if (!this->get_tile(x, y).is_there()){  
		audio->transformSound();
		TerrainTile tile = convert_byte(y, x, type);
		terrain_map[y][x] = tile;
		return true;
	}
	else { return false; }
}

// Level Generation stuff begins here
void Level::form_platforms(int begin_x, int end_x, int begin_y, int end_y){
	// ASSUME ALL BLOCKS PASSED IN ARE CLEAR
	int attempted_platforms = 10;
	int left_edge = rand() % ((end_x - begin_x)/2) + begin_x +1;
	int platform_length = rand() % 5 + 3;
	int right_edge; 
	int height = begin_y+ 1;
	while (attempted_platforms > 0){
		int count = left_edge;
		while (count <= end_x-1 && platform_length > 0){
			terrain_save_map[height][count] = (unsigned char)1;
			count++;
			platform_length--;
		}
		right_edge = count;
		int attempted_left_edge;
		int x_jump = rand() % 3 + 1;
		platform_length = rand() % 5 + 3;
		if (rand() % 2 == 1){ 
			attempted_left_edge = right_edge + x_jump;
		}
		else { attempted_left_edge = left_edge - x_jump - platform_length; }
		while (attempted_left_edge > end_x || attempted_left_edge < begin_x){
			x_jump = rand() % 3 + 1;
			platform_length = rand() % 5 + 3;
			if (rand() % 2 == 1){
				attempted_left_edge = right_edge + x_jump;
			}
			else { attempted_left_edge = left_edge - x_jump - platform_length; }
		}
		left_edge = attempted_left_edge;
		int y_jump = rand() % 2 + 1;
		if (height + y_jump >= end_y -1){
			height = height - y_jump * 2;
		}
		else { height = height + y_jump; }
		attempted_platforms--;
	}
	/*OLD METHOD, will probably delete
	for (int i = begin_y + 1; i < end_y-1; i++){
		for (int j = begin_x; j < end_x; j++){
			if (terrain_save_map[i][j] != (unsigned char)1 && terrain_save_map[i][j] != (unsigned char)254 && 
				terrain_save_map[i][j] != (unsigned char)255 && rand()%100 < 20){
				int attempted_platform_length = rand() %10 + 4;
				int attempted_platform_width =rand()%3 + 1;
				int k = i;
				int l = j;
				int width_count = 0;
				int length_count = 0;
				while (k < end_y-2 && width_count < attempted_platform_width){
					while (l < end_x && length_count < attempted_platform_length){
						terrain_save_map[k][l] = (unsigned char)1;
						l++;
						length_count++;
					}
					width_count++;
					k++;
				}
				j = l + rand() % 2;
			}
		}
		i += rand() % 2 + 1;
	}*/
}
void Level::split(int level, int max_depth, int begin_x, int end_x, int begin_y, int end_y) {
	// Might need to record final room dimensions
	// form platforms based off how tall the room is?
	level++;
	//if (level > max_depth){ return; }
	if (level % 2 == 1 ){
		int max_range = (end_y - begin_y)/2;
		int min_range = (end_y - begin_y)/4;
		if ((end_y - begin_y)<= 16 && (end_x - begin_x) <=20 ){ 
			form_platforms(begin_x, end_x, begin_y, end_y);
			form_platforms(begin_x, end_x,begin_y, end_y);
			return; 
		}
		else if ((end_y - begin_y <= 16) && !(end_x - begin_x <= 20)){
			split(level, max_depth, begin_x, end_x, begin_y, end_y);
			return;
		}
		int wall_thickness = rand() % 3 + 3;
		int dividing_line = (rand() % max_range) + min_range + begin_y;
		int door = (rand() % (end_x - begin_x))/3 + begin_x + (end_x-begin_x)/3;
		for (int i = begin_x; i <= end_x; i++){
			if (i != door && i!= door -1 && i!= door+1){
				for (int j = 0; j < wall_thickness; j++){
					terrain_save_map[dividing_line - j][i] = (unsigned char)1;
					terrain_save_map[dividing_line + j][i] = (unsigned char)1;
				}
			}
			else { 
				terrain_save_map[dividing_line][i] = (unsigned char)255; 
				terrain_save_map[dividing_line-1][i] = (unsigned char)255;
				terrain_save_map[dividing_line+1][i] = (unsigned char)255;
			}
		}
		if (level == max_depth){
			form_platforms(begin_x, end_x, begin_y, dividing_line - wall_thickness -1);
			form_platforms(begin_x, end_x, dividing_line +wall_thickness+ 1, end_y);
		}
		split(level, max_depth, begin_x, end_x, begin_y, dividing_line - wall_thickness - 1);
		split(level, max_depth, begin_x, end_x, dividing_line + wall_thickness + 1, end_y);
	}
	else if (level % 2 == 0){
		int max_range = (end_x - begin_x)/2;
		int min_range = (end_x - begin_x)/4;
		if ((end_y - begin_y) <= 16 && (end_x - begin_x) <= 20){
			form_platforms(begin_x, end_x, begin_y, end_y);
			form_platforms(begin_x, end_x, begin_y, end_y);
			return; 
		}
		else if (!(end_y - begin_y <= 16) && (end_x - begin_x <= 20)){
			split(level, max_depth, begin_x, end_x, begin_y, end_y);
			return;
		}
		int wall_thickness = rand() % 3 + 3;
		int dividing_line = (rand() % max_range) + min_range + begin_x;
		int door = (rand()%(end_y - begin_y))/3 + begin_y + (end_y-begin_y)/3;
		for (int i = begin_y; i <= end_y; i++){
			if (i != door && i!= door +1 && i != door-1){
				for (int j = 0; j < wall_thickness; j++) {
					terrain_save_map[i][dividing_line - j] = (unsigned char)1;
					terrain_save_map[i][dividing_line + j] = (unsigned char)1;
				}
			}
			else { 
				terrain_save_map[i][dividing_line] = (unsigned char)254; 
				terrain_save_map[i][dividing_line-1] = (unsigned char)254;
				terrain_save_map[i][dividing_line+1] = (unsigned char)254;
			}
		}
		if (level == max_depth){
			form_platforms(begin_x, dividing_line-wall_thickness-1, begin_y, end_y);
			form_platforms(dividing_line + wall_thickness + 1, end_x, begin_y, end_y);
		}
		split(level, max_depth, begin_x, dividing_line - wall_thickness - 1, begin_y, end_y);
		split(level, max_depth, dividing_line + wall_thickness + 1, end_x, begin_y, end_y);
	}
}


void Level::build_horizontal_hall_and_room_left(int x, int y, int hallway_length, int room_width, int room_height){
	for (int i = x; i > x - hallway_length; i--){
		terrain_save_map[y][i] = (unsigned char)200;
		terrain_save_map[y + 1][i] = (unsigned char)200;
		terrain_save_map[y - 1][i] = (unsigned char)200;
	}
	for (int i = x - hallway_length; i > x - hallway_length - room_width; i--){
		for (int j = y - 1; j <= y + room_height; j++){
			terrain_save_map[j][i] = (unsigned char)200;
		}
	}
	form_platforms(x - hallway_length - room_width + 1, x - hallway_length,  y - 1, y + room_height - 1);
	int new_hallway_length = rand() % 10 + 5;
	int new_x = x - hallway_length - room_width;
	int new_y = rand() % ((room_height) / 2) + (y);
	int new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	int new_room_height = rand() % (max_room_height - min_room_height)+ min_room_height;
	bool make_horizontal = is_room_for_horizontal_hall_and_room_left(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	if (make_horizontal){
		build_horizontal_hall_and_room_left(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	}
	new_hallway_length = rand() % 10 + 5;
	new_x = x - hallway_length - room_width / 2 - rand() % (room_width / 2) + 2;
	new_y = y - 2;
	new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
	bool is_room_vertical = (is_room_for_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height));
	int attempts = attempts_to_fit;
	while (!is_room_vertical && attempts > 0){
		new_hallway_length = rand() % 10 + 5;
		new_x = x - hallway_length - room_width / 2 - rand() % (room_width / 2) + 2;
		new_y = y - 2;
		new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
		new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
		is_room_vertical = (is_room_for_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height));
		attempts--;
	}
	bool make_vertical = ((rand() % 100 < vertical_drop_chance && is_room_vertical) || (!make_horizontal && is_room_vertical));
	if (make_vertical){
		build_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	}
}

bool Level::is_room_for_horizontal_hall_and_room_left(int x, int y, int hallway_length, int room_width, int room_height){
	int breathing_room = 5;	
	for (int i = x; i > x - hallway_length; i--){
		for (int j = 0; j < breathing_room; j++){
			if (i >= width || i < 0 || y + j >= height || y - j <0){ return false; }
			if (terrain_save_map[y][i] != (unsigned char)1){ return false; }
			if (terrain_save_map[y + j][i] != (unsigned char)1){ return false; }
			if (terrain_save_map[y - j][i] != (unsigned char)1){ return false; }
		}
	}
	for (int i = x - hallway_length + breathing_room; i > x - hallway_length - room_width - room_from_edge; i--){
		for (int j = y - 1-breathing_room; j <= y + room_height+breathing_room; j++){
			if (i >= width || i < 0 || j >= height || j < 0){ return false; }
			if (terrain_save_map[j][i] != (unsigned char)1) { return false; }
		}
	}
	return true;
}
void Level::build_horizontal_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height){
	for (int i = x; i < x + hallway_length; i++){
		terrain_save_map[y][i] = (unsigned char)200;
		terrain_save_map[y+1][i] = (unsigned char)200;
		terrain_save_map[y - 1][i] = (unsigned char)200;
	}
	for (int i = x + hallway_length; i < x + hallway_length + room_width; i++){
		for (int j = y - 1; j <= y + room_height; j++){
			terrain_save_map[j][i] = (unsigned char)200;
		}
	}
	form_platforms(x + hallway_length, x + hallway_length + room_width - 1, y - 1, y + room_height -1);
	int new_hallway_length = rand() % 10 + 5;
	int new_x = x + hallway_length + room_width;
	int new_y = rand() % ((room_height) / 2) + (y);
	int new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	int new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
	bool make_horizontal = is_room_for_horizontal_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	if (make_horizontal){
		build_horizontal_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	}
	new_hallway_length = rand() % 10 + 5;
	new_x = x + hallway_length + room_width / 2 + rand() % (room_width / 2) - 2;
	new_y = y -2;
	new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	new_room_height = rand() % (max_room_height - min_room_height) + min_room_height; 
	bool is_room_vertical = (is_room_for_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height));
	int attempts = attempts_to_fit;
	while (!is_room_vertical && attempts > 0) {
		new_hallway_length = rand() % 10 + 5;
		new_x = x + hallway_length + room_width / 2 + rand() % (room_width / 2) - 2;
		new_y = y - 2;
		new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
		new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
		is_room_vertical = (is_room_for_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height));
		attempts--;
	}
	bool make_vertical = ((rand() % 100 < vertical_drop_chance && is_room_vertical) || (!make_horizontal && is_room_vertical));
	if (make_vertical){
		build_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	}
}

void Level::build_vertical_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height){
	for (int i = y; i > y - hallway_length; i--){
		terrain_save_map[i][x] = (unsigned char)200;
		terrain_save_map[i][x+1] = (unsigned char)200;
		terrain_save_map[i][x-1] = (unsigned char)200;
	}
	for (int i = y - hallway_length; i > y - hallway_length - room_height; i--){
	 	for (int j = x - 1; j < x + room_width; j++){
			terrain_save_map[i][j] = (unsigned char)200;
		}
	}
	form_platforms(x - 1, x + room_width - 1, y - hallway_length - room_height +1 , y- hallway_length);
	int new_hallway_length = rand() % 10 + 5;
	int new_x = x + room_width;
	int new_y = rand() % ((room_height) / 2) + (y - hallway_length - room_height +2);
	int new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	int new_room_height = rand() % (max_room_height - min_room_height) + min_room_height; 
	bool make_horizontal = is_room_for_horizontal_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	int attempts = attempts_to_fit;
	while (!make_horizontal && attempts > 0) {
		new_hallway_length = rand() % 10 + 5;
		new_x = x + room_width;
		new_y = rand() % ((room_height) / 2) + (y - hallway_length - room_height + 2);
		new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
		new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
		make_horizontal = is_room_for_horizontal_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
		attempts--;
	}
	if (make_horizontal){
		build_horizontal_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	}
	new_hallway_length = rand() % 10 + 5;
	new_x = x-2;
	new_y = rand() % ((room_height) / 2) + (y - hallway_length - room_height + 2);
	new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	new_room_height = rand() % (max_room_height - min_room_height) + min_room_height; 
	bool make_horizontal_left = is_room_for_horizontal_hall_and_room_left(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	attempts = attempts_to_fit;
	while (!make_horizontal_left && attempts > 0){
		new_hallway_length = rand() % 10 + 5;
		new_x = x - 2;
		new_y = rand() % ((room_height) / 2) + (y - hallway_length - room_height + 2);
		new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
		new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
		make_horizontal_left = is_room_for_horizontal_hall_and_room_left(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
		attempts--;
	}
	if (make_horizontal_left) {
		build_horizontal_hall_and_room_left(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
	}
	if (!make_horizontal_left && !make_horizontal){
		new_hallway_length = rand() % 10 + 5;
		new_x = x + 2 + rand()%(room_width/2);
		new_y = y - hallway_length - room_height;
		new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
		new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
		bool is_room_vertical = (is_room_for_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height));
		attempts = attempts_to_fit;
		while (!is_room_vertical && attempts > 0){
			new_hallway_length = rand() % 10 + 5;
			new_x = x +  rand() % (room_width / 2) + 2;
			new_y = y - hallway_length - room_height;
			new_room_width = rand() % (max_room_width - min_room_width) + min_room_width;
			new_room_height = rand() % (max_room_height - min_room_height) + min_room_height;
			is_room_vertical = (is_room_for_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height));
			attempts--;
		}
		if (is_room_vertical){
			build_vertical_hall_and_room(new_x, new_y, new_hallway_length, new_room_width, new_room_height);
		}
	}
}

bool Level::is_room_for_horizontal_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height){
	int breathing_room = 5;
	for (int i = x; i < x + hallway_length; i++){
		for (int j = 0; j < breathing_room; j++){
			if (i >= width || i < 0 || y + j >= height || y - j <0){ return false; }
			if (terrain_save_map[y][i] != (unsigned char)1){ return false; }
			if (terrain_save_map[y + j][i] != (unsigned char)1){ return false; }
			if (terrain_save_map[y - j][i] != (unsigned char)1){ return false; }
		}
	}
	for (int i = x + hallway_length - breathing_room; i < x + hallway_length + room_width + room_from_edge; i++){
		for (int j = y - 1 - breathing_room; j <= y + room_height + breathing_room; j++){
			if (i >= width || i < 0 || j >= height ||  j < 0){ return false; }
			if (terrain_save_map[j][i] != (unsigned char)1) { return false; }
		}
	}
	return true;
}

bool Level::is_room_for_vertical_hall_and_room(int x, int y, int hallway_length, int room_width, int room_height){
	int breathing_room = 5;
	for (int i = y; i > y - hallway_length; i--){
		for (int j = 0; j < breathing_room; j++){
			if (i >= height || i < 0 || x + j >= width || x - j < 0) { return false; }
			if (terrain_save_map[i][x] != (unsigned char)1) { return false; }
			if (terrain_save_map[i][x + j] != (unsigned char)1) { return false; }
			if (terrain_save_map[i][x - j] != (unsigned char)1) { return false; }
		}
	}
	for (int i = y - hallway_length + breathing_room; i > y - hallway_length - room_height - breathing_room; i--){
		for (int j = x - 1 - breathing_room; j < x + room_width + breathing_room; j++){
			if(i >= height || i < 0 ||  j >= width ||  j < 0) { return false; }
			if (terrain_save_map[i][j] != (unsigned char)1) { return false; }
		}
	}
	return true;
}
void Level::generate(int difficulty) {
	/*SIMPLE LEVEL GENERATION for testing stuff
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
	*/
	//ROOM GENERATION
	/*for (int i = 0; i < height; i++){
		delete[] terrain_save_map[i];
		delete[] terrain_map[i];
		delete[] sprite_save_map[i];
	}
	delete[] terrain_save_map;
	delete[] terrain_map;
	delete[] sprite_save_map;
	width = 300;
	height = 200;
	terrain_save_map = new unsigned char*[height];
	terrain_map = new TerrainTile*[height];
	sprite_save_map = new unsigned char*[height];

	for (int i = 0; i < height; i++){
		terrain_save_map[i] = new unsigned char[width];
		terrain_map[i] = new TerrainTile[width];
		sprite_save_map[i] = new unsigned char[width];
	}
	
	
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (i < 10 || i > height - 10 || j < 10 || j > width - 10){
				terrain_save_map[i][j] = (unsigned char)1;
			}
		}
	}
	
	
	// recursive function for splitting map up into rooms (seems to work well)
	split(0, 16, 11, width - 11, 11, height - 11);
	*/
	//Trying to get stalactite-like growth
	/*int drip_chance = 20;
	int drip_rate = 30;
	int iterations = 5;
	for (int i = 1; i < height; i++){
		for (int j = 1; j < width; j++){
			if (terrain_save_map[i][j] == (unsigned char)1 && rand()%100 < drip_chance){
				terrain_save_map[i][j] = (unsigned char)233;
				j += 15;
			}
		}
	}
	for (int x = 0; x < iterations; x++){
		for (int i = 1; i < height-1; i++){
			for (int j = 1; j < width-1; j++){
				if (terrain_save_map[i][j] != (unsigned char)255 && terrain_save_map[i][j] != (unsigned char)254){
					if (terrain_save_map[i - 1][j] == (unsigned char)233 && rand() % 100 < drip_rate){ terrain_save_map[i][j] = (unsigned char)232; }
					if (terrain_save_map[i - 1][j + 1] == (unsigned char)233 && rand() % 100 < drip_rate / 2){ terrain_save_map[i][j] = (unsigned char)232; }
					if (terrain_save_map[i - 1][j - 1] == (unsigned char)233 && rand() % 100 < drip_rate / 2){ terrain_save_map[i][j] = (unsigned char)232; }
					if (terrain_save_map[i][j + 1] == (unsigned char)233 && rand() % 100 < drip_rate / 2){ terrain_save_map[i][j] = (unsigned char)232; }
					if (terrain_save_map[i][j - 1] == (unsigned char)233 && rand() % 100 < drip_rate / 2){ terrain_save_map[i][j] = (unsigned char)232; }
				}
			}
		}
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (terrain_save_map[i][j] == (unsigned char)232){ terrain_save_map[i][j] = (unsigned char)233; }
			}
		}
	}
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (terrain_save_map[i][j] == (unsigned char)233){ terrain_save_map[i][j] = (unsigned char)2; }
		}
	}*/	
	
	/*int formation_chance = 10;
	int iterations = 5;

	for (int i = 0; i < iterations; i++){
		for (int j = 1; j < height-1; j++){
			for (int k = 1; k < width - 1; k++){
				
				if (terrain_save_map[j][k] != (unsigned char)1 && terrain_save_map[j][k] != (unsigned char)255 &&
					terrain_save_map[j][k] != (unsigned char)225 && terrain_save_map[j][k] != (unsigned char)254){
					int cumulative_chance = 0;
					if (terrain_save_map[j - 1][k] == (unsigned char)1) { cumulative_chance += 2*formation_chance; }
					
					if (terrain_save_map[j - 1][k - 1] == (unsigned char)1) { cumulative_chance += formation_chance; }
				
					if (terrain_save_map[j - 1][k + 1] == (unsigned char)1) { cumulative_chance += formation_chance; }
					
					//if (terrain_save_map[j][k - 1] == (unsigned char)1) { cumulative_chance += formation_chance/2; }
					
					//if (terrain_save_map[j][k + 1] == (unsigned char)1) { cumulative_chance += formation_chance/2; }
					
					//if (terrain_save_map[j + 1][k] == (unsigned char)1) { //cumulative_chance += formation_chance; }
					
					//if (terrain_save_map[j + 1][k - 1] == (unsigned char)1) { //cumulative_chance += formation_chance; }
					
					//if (terrain_save_map[j + 1][k + 1] == (unsigned char)1) { //cumulative_chance += formation_chance; }
					
					if (rand() % 100 < cumulative_chance){ terrain_save_map[j][k] = (unsigned char)225; }
				}
			}
		}
		for (int j = 1; j < height - 1; j++){
			for (int k = 1; k < width - 1; k++){
				if (terrain_save_map[j][k] == (unsigned char)225){ terrain_save_map[j][k] = (unsigned char)1; }
			}
		}
	}*/

	//255 up/down, 254 left/right
	// trying to ensure a door into each room. Some reason creates long vertical shafts?
	/*for (int i = 1; i < height-1; i++){
		for (int j = 1; j < width-1; j++){
			if (terrain_save_map[i][j] == (unsigned char)255){			
				int k = i+1;
				while ( k < height && terrain_save_map[k][j] == (unsigned char)1){
					terrain_save_map[k][j] = (unsigned char)255;
					k++;
				}
				k = i-1;
				while (k > 0 && terrain_save_map[k][j] == (unsigned char)1){
					terrain_save_map[k][j] = (unsigned char)255;
					k--;
				}
			}
			if (terrain_save_map[i][j] == (unsigned char)254){
				int k = j + 1;
				while (k < width && terrain_save_map[i][k] == (unsigned char)1){
					terrain_save_map[i][k] = (unsigned char)254;
					k++;
				}
				k = j - 1;
				while (k > 0 && terrain_save_map[i][k] == (unsigned char)1){
					terrain_save_map[i][k] = (unsigned char)254;
					k--;
				}
			}
		}
	}*/


	// ROOM GENERATION: METHOD 2

// REMEMBER: 0, 0 is the bottom left (y counts up)
	





	for (int i = 0; i < lights.size(); i++){
		lights[i]->turn_off();
		lights.erase(lights.begin() + i);
	}	
	for (int i = 0; i < height; i++){
		delete[] terrain_save_map[i];
		delete[] terrain_map[i];
		delete[] background_map[i];
		delete[] sprite_save_map[i];
	}
	delete[] terrain_save_map;
	delete[] terrain_map;
	delete[] background_map;
	delete[] sprite_save_map;
	width = 200 + 50*difficulty;
	height = 100 + 25* difficulty;
	terrain_save_map = new unsigned char*[height];
	terrain_map = new TerrainTile*[height];
	background_map = new TerrainTile*[height];
	sprite_save_map = new unsigned char*[height];

	for (int i = 0; i < height; i++){
		terrain_save_map[i] = new unsigned char[width];
		terrain_map[i] = new TerrainTile[width];
		background_map[i] = new TerrainTile[width];
		sprite_save_map[i] = new unsigned char[width];
	}

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			background_map[i][j] = convert_byte(i, j, (unsigned char)21);
		}
	}

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			terrain_save_map[i][j] = (unsigned char)1;
		}
	}
	
	int x = 30;
	int y = height - 40;

	int end_x = x +5;
	int end_y = y;

	x_spawn = (float)x * tilesize;
	y_spawn = (float)y*tilesize;
	
	int hallway_length = 20;
	max_room_width = 20;
	min_room_width = 15;
	max_room_height = 12;
	min_room_height = 8;
	room_from_edge = 30;
	vertical_drop_chance = 50;
	attempts_to_fit = 10000;
	int room_width = rand() % (max_room_width - min_room_width) + min_room_width;
	int room_height = rand() % (max_room_height - min_room_height) + min_room_height;
	build_horizontal_hall_and_room(x, y, hallway_length, room_width, room_height);
	int y2 = 0; 
	int x2 = 0;
	bool found = false;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (terrain_save_map[i][j] != (unsigned char)1 && !found){
				x2 = j;
				y2 = i;
				found = true;
			}
		}
	}
	end_y = y2;
	end_x = x2;
	//DBOUT("earliest cleared y: " << y2);
		
	//Covering things with grass
	//1 =bare stone
		//2=grass on top
		//3= grass on bottom
		//4= grass on left
		//4= grass on right
		//6=grass on top, left
		//7= grass on top, right
		//8 = grass on top, bottom
		// 9= grass on bottom, left 
		//10= grass on bottom, right
		//11= grass on left, right
		//12= grass on top, left, right
		//13= grass on top, left, bottom
		//14=grass on top, right, bottom
		//15= grass on bottom, left, right
		//16= grass on all sides
		//17 = spike on top
		//18 = burnt
		// 19 = lava
		// 20 = skull
		// 2, 6, 7, 8, 12, 13, 14, 16

	// forming lava pools

	Light* light;
	for (int i = 0; i < height - 1; i++){
		for (int j = 1; j < width - 1; j++){
			int roll = rand() % 3 + 1;
			if (terrain_save_map[i][j] == (unsigned char)1 && is_room_for_lava_pool(i, j + 1, roll) && rand() % 100 < 10 + difficulty){
				terrain_save_map[i][j] = (unsigned char)18;
				terrain_save_map[i-1][j] = (unsigned char)18;
				for (int k = 1; k <= roll; k++){
					terrain_save_map[i][j+k] = (unsigned char)19;
					terrain_save_map[i - 1][j+k] = (unsigned char)18;
				}
				terrain_save_map[i][j + roll + 1] = (unsigned char)18;
				terrain_save_map[i - 1][j+roll+1] = (unsigned char)18;
				float light_location = (float)(((float)roll / 2) + (float)j + 1)*tilesize;
				light = new Light();
				light->a = .5;
				light->b = 1;
				light->position.set_x(light_location);
				light->position.set_y((float)i*tilesize);
				light->tint.r = 1.0;
				light->tint.g = 0.0;
				light->tint.b = 0.0;
				light->tint.a = 1.0;
				light->is_off = false;
				lights.push_back(light);
				light_manager->accept_light(light);
			}
			
		}
	}
	for (int i = 1; i < height - 1; i++){
		for (int j = 1; j < width - 1; j++){
			bool exposed_top = terrain_save_map[i + 1][j] == (unsigned char)200;
			bool exposed_bottom = terrain_save_map[i - 1][j] == (unsigned char)200;
			bool exposed_left = terrain_save_map[i][j - 1] == (unsigned char)200;
			bool exposed_right = terrain_save_map[i][j + 1] == (unsigned char)200;
			if (terrain_save_map[i][j] == (unsigned char)1){
				if (exposed_top && exposed_bottom && exposed_right && exposed_left){
					terrain_save_map[i][j] = (unsigned char)16;
				}
				else if (exposed_bottom && exposed_right && exposed_left){
					terrain_save_map[i][j] = (unsigned char)15;
				}
				else if (exposed_bottom && exposed_right && exposed_top){
					terrain_save_map[i][j] = (unsigned char)14;
				}
				else if (exposed_bottom && exposed_top && exposed_left){
					terrain_save_map[i][j] = (unsigned char)13;
				}
				else if (exposed_top && exposed_right && exposed_left){
					terrain_save_map[i][j] = (unsigned char)12;
				}
				else if (exposed_left && exposed_right){
					terrain_save_map[i][j] = (unsigned char)11;
				}
				else if (exposed_bottom && exposed_right){
					terrain_save_map[i][j] = (unsigned char)10;
				}
				else if (exposed_bottom && exposed_left){
					terrain_save_map[i][j] = (unsigned char)9;
				}
				else if (exposed_bottom && exposed_top){
					terrain_save_map[i][j] = (unsigned char)8;
				}
				else if (exposed_right && exposed_top){
					terrain_save_map[i][j] = (unsigned char)7;
				}
				else if (exposed_left && exposed_top){
					terrain_save_map[i][j] = (unsigned char)6;
				}
				else if (exposed_right){
					terrain_save_map[i][j] = (unsigned char)5;
				}
				else if (exposed_left){
					terrain_save_map[i][j] = (unsigned char)4;
				}
				else if (exposed_bottom){
					terrain_save_map[i][j] = (unsigned char)3;
				}
				else if (exposed_top){
					terrain_save_map[i][j] = (unsigned char)2;
				}
			}// 2, 6, 7, 8, 12, 13, 14, 16
			if ((terrain_save_map[i][j] == (unsigned char)2 || terrain_save_map[i][j] == (unsigned char)6 ||
				terrain_save_map[i][j] == (unsigned char)7 || terrain_save_map[i][j] == (unsigned char)8 ||
				terrain_save_map[i][j] == (unsigned char)12 || terrain_save_map[i][j] == (unsigned char)13 ||
				terrain_save_map[i][j] == (unsigned char)14 || terrain_save_map[i][j] == (unsigned char)16) &&
				rand() % 100 < 10 + 3*difficulty){
				terrain_save_map[i][j] = (unsigned char)17;
			}
		}
	}
	terrain_save_map[end_y][end_x] = (unsigned char)22;
	//CAVE GENERATION
	
	/*
	for (int i = 0; i < height; i++){
		delete[] terrain_save_map[i];
		delete[] terrain_map[i];
		delete[] sprite_save_map[i];
	}
	delete[] terrain_save_map;
	delete[] terrain_map;
	delete[] sprite_save_map;
	width = 100;
	height = 1000;


	terrain_save_map = new unsigned char*[height];
	terrain_map = new TerrainTile*[height];
	sprite_save_map = new unsigned char*[height];
	for (int i = 0; i < height; i++){
		terrain_save_map[i] = new unsigned char[width];
		terrain_map[i] = new TerrainTile[width];
		sprite_save_map[i] = new unsigned char[width];
	}

	
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			terrain_save_map[i][j] = (unsigned char)(rand() % 2);
		}
	}

	int repeat = 7; 
	int upper_threshold = 5;
	int lower_threshold = 3;
	int* count_storage = new int[height*width];
	for (int i = 0; i < height*width; i++){
		count_storage[i] = 0;
	}
	for (int i = 0; i < repeat; i++){
		for (int j = 0; j < height-1; j++){
			for (int k = 0; k < width-1; k++){
				int count = 0;
				if (j != 0 && k != 0){
					if (terrain_save_map[j - 1][k - 1] == (unsigned char)1){ count++; }
					if (terrain_save_map[j][k - 1] == (unsigned char)1){ count++; }
					if (terrain_save_map[j + 1][k - 1] == (unsigned char)1){ count++; }
					if (terrain_save_map[j - 1][k] == (unsigned char)1){ count++; }
					//if (terrain_save_map[j][k] == (unsigned char)1){ count++; }
					if (terrain_save_map[j + 1][k] == (unsigned char)1){ count++; }
					if (terrain_save_map[j - 1][k + 1] == (unsigned char)1){ count++; }
					if (terrain_save_map[j][k + 1] == (unsigned char)1){ count++; }
					if (terrain_save_map[j + 1][k + 1] == (unsigned char)1){ count++; }
					//if (count >= threshold){ terrain_save_map[j][k] = (unsigned char)1; }
				}
				if ((width*j) + k > height*width){
					DBOUT((height*j) + k);
				}
				count_storage[(width*j) + k] = count;
			}
		}
		for (int i = 0; i < height; i++){
			for (int j = 0; j < width; j++){
				if (count_storage[(width*i) + j] >= upper_threshold){
					terrain_save_map[i][j] = (unsigned char)1;
				}
				else if (count_storage[(width*i) + j] <= lower_threshold){
					terrain_save_map[i][j] = (unsigned char)0;
				}
			}
		}
	}
	delete count_storage;
	*/
	convert_byte_map();
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			if (!terrain_map[i][j].is_there() && rand() % 1000 < 15 + difficulty){
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

