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
		y_velocity = jump_velocity;
	}
}
void Sprite::calculate_terrain_collisions(Level* level){
	int tile_x = this->get_x_tile_position(level->get_tilesize());
	int tile_y = this->get_y_tile_position(level->get_tilesize());
	TerrainTile lower_tile = level->get_tile(tile_x, tile_y - 1);
	TerrainTile upper_tile = level->get_tile(tile_x, tile_y + 1);
	TerrainTile left_tile = level->get_tile(tile_x - 1, tile_y);
	TerrainTile right_tile = level->get_tile(tile_x + 1, tile_y);
	
	/*if (left_tile.exists){
		DBOUT("Player leftmost");
		DBOUT(x); 
		DBOUT("Tile: ");
		DBOUT(left_tile.get_x() + left_tile.get_width()/2);

	}*/
	if (x < 4){
		//DBOUT(width);
	}
	if (left_tile.exists && left_tile.right_is_solid() && this->Collides(&left_tile)){
		//DBOUT("TRIGGERED!");
		this->FixXPenetration(&left_tile);
		left_flag = true;
		x_velocity = 0;
		//x_acceleration = 0;
	}
	else { left_flag = false; }
	if (right_tile.exists && right_tile.left_is_solid() && this->Collides(&right_tile)){
		this->FixXPenetration(&right_tile);
		right_flag = true;
		x_velocity = 0;
		//x_acceleration = 0;
	}
	else { right_flag = false; }
	if (upper_tile.exists && upper_tile.bottom_is_solid() && this->Collides(&upper_tile)){
		this->FixYPenetration(&upper_tile);
		top_flag = true;
		y_velocity = 0;
		//y_acceleration = 0;
	}
	else{ top_flag = false; }
	if (lower_tile.exists && lower_tile.top_is_solid() && this->Collides(&lower_tile) && !(y_velocity > 0)){
		this->FixYPenetration(&lower_tile);
		bottom_flag = true;
		y_velocity = 0;
		y_acceleration = 0;
		jump_counter = 0;
		jumping = false;
	}
	else { bottom_flag = false; }
}



void Sprite::update(float time_elapsed, Level* level){
	if (y_acceleration > maximum_acceleration){ y_acceleration = maximum_acceleration; }
	//if (y_acceleration < -maximum_acceleration){ y_acceleration = -maximum_acceleration; }
	if (x_acceleration > maximum_acceleration){ x_acceleration = maximum_acceleration; }
	if (x_acceleration < -maximum_acceleration) { x_acceleration = -maximum_acceleration; }
	if (abs(x_velocity) < minimum_velocity) { x_velocity = 0; }
	UpdateX(time_elapsed);
	calculate_terrain_collisions(level);
	UpdateY(time_elapsed);
	calculate_terrain_collisions(level);
	animation_counter += time_elapsed;
	if (abs(x_velocity)<minimum_velocity){ current_frame = 0; }
	else if (current_frame < first_animation_frame){ current_frame = first_animation_frame; }
	else if (animation_counter > animation_time){
		animation_counter = 0;
		current_frame++;
		if (current_frame > last_animation_frame){ current_frame = first_animation_frame; }
	}
	if (jumping && !bottom_flag && y_velocity > 0){
		current_frame = 1;
	}
	if (dead){ current_frame = death_frame; }
	if (hurt && hurt_counter < hurt_time){
		current_frame = hurt_frame;
		hurt_counter += time_elapsed;
	}
	if (hurt_counter >= hurt_time){
		hurt = false;
		hurt_counter = 0;
	}
	sprite = frames[current_frame];
}