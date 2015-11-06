#include "Sprite.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

void Sprite::move_right(){
	mirrored = false;
	x_acceleration += acceleration_rate;
}

void Sprite::move_left(){
	mirrored = true;
	x_acceleration -= acceleration_rate;
}

void Sprite::jump(){
	if (bottom_flag){
		y_acceleration = 0;
		y_velocity = jump_velocity;
		//might not want this here...
		audio->jumpSound();
	} 	
}

// Really necessary to declare new tiles for this? No, fix later? Makes code ugly 
void Sprite::calculate_y_terrain_collisions(Level* level){
	int tile_x = this->get_x_tile_position(level->get_tilesize());
	int tile_y = this->get_y_tile_position(level->get_tilesize());
	TerrainTile lower_tile = level->get_tile(tile_x, tile_y - 1);
	TerrainTile upper_tile = level->get_tile(tile_x, tile_y + 1);
	TerrainTile this_tile = level->get_tile(tile_x, tile_y);
	if (this_tile.is_there() && this->Collides(&this_tile)){
		this->FixYPenetration(&this_tile);
	}
	if (upper_tile.is_there() && upper_tile.bottom_is_solid() && this->Collides(&upper_tile)){
		this->FixYPenetration(&upper_tile);
		top_flag = true;
		y_velocity = 0;
	}
	else{ top_flag = false; }
	if (lower_tile.is_there() && lower_tile.top_is_solid() && this->Collides(&lower_tile) && !(y_velocity > 0)){
		this->FixYPenetration(&lower_tile);
		bottom_flag = true;
		y_velocity = 0;
	}
	else { bottom_flag = false; }
}

void Sprite::calculate_x_terrain_collisions(Level* level){
	int tile_x = this->get_x_tile_position(level->get_tilesize());
	int tile_y = this->get_y_tile_position(level->get_tilesize());
	TerrainTile left_tile = level->get_tile(tile_x - 1, tile_y);
	TerrainTile right_tile = level->get_tile(tile_x + 1, tile_y);
	TerrainTile this_tile = level->get_tile(tile_x, tile_y);
	
	if (this_tile.is_there() && this->Collides(&this_tile)){
		this->FixXPenetration(&this_tile);
	}
	
	if (left_tile.is_there() && left_tile.right_is_solid() && this->Collides(&left_tile)){
		this->FixXPenetration(&left_tile);
		left_flag = true;
		x_velocity = 0;
	}
	else { left_flag = false; }
	if (right_tile.is_there() && right_tile.left_is_solid() && this->Collides(&right_tile)){
		this->FixXPenetration(&right_tile);
		right_flag = true;
		x_velocity = 0;
	}
	else { right_flag = false; }
	
}


// Remove frame decision stuff from this - Sprite should strictly handle movement
void Sprite::update(float time_elapsed, Level* level){
	if (y_acceleration > maximum_acceleration){ y_acceleration = maximum_acceleration; }
	if (y_acceleration < -maximum_acceleration){ y_acceleration = -maximum_acceleration; }
	if (x_acceleration > maximum_acceleration){ x_acceleration = maximum_acceleration; }
	if (x_acceleration < -maximum_acceleration) { x_acceleration = -maximum_acceleration; }
	if (abs(x_velocity) < minimum_velocity) { x_velocity = 0; }
	if (y_velocity < 0 && bottom_flag){ y_velocity = 0; }
	if (y_velocity > 0 && top_flag){ y_velocity = 0; }
	if (x_velocity < 0 && left_flag){ x_velocity = 0; }
	if (x_velocity > 0 && right_flag) { x_velocity = 0; }
	UpdateX(time_elapsed);
	calculate_x_terrain_collisions(level);
	UpdateY(time_elapsed);
	calculate_y_terrain_collisions(level);
	animation_counter += time_elapsed;
}

// getters + setters

int Sprite::get_x_tile_position(float tilesize){
	int x_tile = (x) / tilesize;
	return x_tile;
}

int Sprite::get_y_tile_position(float tilesize){
	int y_tile = (y) / tilesize;
	return y_tile;
}

int Sprite::get_current_frame(){ return current_frame; }
