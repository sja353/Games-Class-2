#include "Sprite.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

void Sprite::move_right(){
	mirrored = false;
	acceleration.set_x(acceleration_rate);
}

void Sprite::move_left(){
	mirrored = true;
	acceleration.set_x (-acceleration_rate);
}
void Sprite::idle(){
}

void Sprite::jump(){
	if (bottom_flag){
		acceleration.set_y(0);
		velocity.set_y(jump_velocity);
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
		if (this_tile.is_win_tile()) { this->touched_win_tile(); }
	}
	if (upper_tile.is_there() && upper_tile.bottom_is_solid() && this->Collides(&upper_tile)){
		this->FixYPenetration(&upper_tile);
		top_flag = true;	
		velocity.set_y(0);
		if (upper_tile.damage_bottom()){
			hurt_from_top = true;
			hurt_from_tile(upper_tile.get_damage());
		}
		if (upper_tile.is_win_tile()) { this->touched_win_tile(); }
	}
	else{ top_flag = false; }
	if (lower_tile.is_there() && lower_tile.top_is_solid() && this->Collides(&lower_tile) && !(velocity.get_y() > 0)){
		this->FixYPenetration(&lower_tile);
		bottom_flag = true;
		velocity.set_y(0);
		if (lower_tile.damage_top()){
			hurt_from_bottom = true;
			hurt_from_tile(lower_tile.get_damage());
		}
		if (lower_tile.is_win_tile()) { this->touched_win_tile(); }
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
		if (this_tile.is_win_tile()) { this->touched_win_tile(); }
	}
	
	if (left_tile.is_there() && left_tile.right_is_solid() && this->Collides(&left_tile)){
		this->FixXPenetration(&left_tile);
		left_flag = true;
		velocity.set_x(0);
		if (left_tile.damage_right()){
			hurt_from_left = true;
			hurt_from_tile(left_tile.get_damage());
		}
		if (left_tile.is_win_tile()) { this->touched_win_tile(); }
	}
	else { left_flag = false; }
	if (right_tile.is_there() && right_tile.left_is_solid() && this->Collides(&right_tile)){
		this->FixXPenetration(&right_tile);
		right_flag = true;
		velocity.set_x(0);
		if (right_tile.damage_left()){
			hurt_from_right = true;
			hurt_from_tile(right_tile.get_damage());
		}
		if (right_tile.is_win_tile()) { this->touched_win_tile(); }
	}
	else { right_flag = false; }
	
}


// Remove frame decision stuff from this - Sprite should strictly handle movement
void Sprite::update(float time_elapsed, Level* level){
	if (acceleration.get_y() > maximum_acceleration){ acceleration.set_y ( maximum_acceleration); }
	if (acceleration.get_y() < -maximum_acceleration){ acceleration.set_y(-maximum_acceleration); }
	if (acceleration.get_x() > maximum_acceleration){ acceleration.set_x(maximum_acceleration); }
	if (acceleration.get_x() < -maximum_acceleration) {acceleration.set_x(-maximum_acceleration); }
	if (abs(velocity.get_x()) < minimum_velocity) { velocity.set_x(0); }
	if (velocity.get_y() < -maximum_y_velocity) { velocity.set_y(-maximum_y_velocity); }
	if (velocity.get_y() > maximum_y_velocity) { velocity.set_y(maximum_y_velocity); }

	//if (abs(velocity.get_y()) < minimum_velocity) { velocity.set_y(0); }
	if (velocity.get_y() < 0 && bottom_flag){ velocity.set_y(0); }
	if (velocity.get_y() > 0 && top_flag){ velocity.set_y(0); }
	if (velocity.get_x() < 0 && left_flag){ velocity.set_x(0); }
	if (velocity.get_x() > 0 && right_flag) { velocity.set_x(0); }
	UpdateX(time_elapsed);
	calculate_x_terrain_collisions(level);
	UpdateY(time_elapsed);
	calculate_y_terrain_collisions(level);
	animation_counter += time_elapsed;
}

// getters + setters

int Sprite::get_x_tile_position(float tilesize){
	int x_tile = (position.get_x()) / tilesize;
	return x_tile;
}

int Sprite::get_y_tile_position(float tilesize){
	int y_tile = (position.get_y())/ tilesize;
	return y_tile;
}

int Sprite::get_current_frame(){ return current_frame; }
