#include "Player.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

Player::Player(float size, float x_position, float y_position, int texture, ShaderProgram* program){
	
	this->program = program;
	x = x_position;
	y = y_position;
	this->size = size;

	// Animation stuff. Here, set the positions of the various sprites used, toss them in array
	float sheet_width = 508;
	float sheet_length = 288;
	animation_time = .05f;
	Sheetposition p1_stand = Sheetposition(67, 196, 66, 92, this->size, sheet_width, sheet_length);
	Sheetposition p1_jump = Sheetposition(438, 93, 67, 92, this->size, sheet_width, sheet_length);
	Sheetposition p1_duck = Sheetposition(365, 98, 69, 71, this->size, sheet_width, sheet_length);
	Sheetposition p1_front = Sheetposition(0, 196, 66, 92, this->size, sheet_width, sheet_length);
	Sheetposition p1_hurt = Sheetposition(438, 0, 69, 92, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk01 = Sheetposition(0, 0, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk02 = Sheetposition(73, 0, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk03 = Sheetposition(146, 0, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk04 = Sheetposition(0, 98, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk05 = Sheetposition(73, 98, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk06 = Sheetposition(146, 98, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk07 = Sheetposition(219, 0, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk08 = Sheetposition(292, 0, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk09 = Sheetposition(219, 98, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk10 = Sheetposition(365, 0, 72, 97, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk11 = Sheetposition(292, 98, 72, 97, this->size, sheet_width, sheet_length);
	spritesheet = SheetSprite(texture, p1_stand);
	frames = new Sheetposition[17];
	Sheetposition holder[] = { p1_stand, p1_jump, p1_duck, p1_front, p1_hurt, p1_walk01, p1_walk02, p1_walk03, p1_walk04, p1_walk05, p1_walk06,
		p1_walk07, p1_walk08, p1_walk09, p1_walk10, p1_walk11 };
	for (int i = 0; i < 17; i++){
		frames[i] = holder[i];
	}
	
	hurt_frame = 4;
	walk_begin = 5;
	walk_end = 15;
	sprite = p1_stand;
	hurt_time = .5f;

	// physics stuff
	bottom_flag = top_flag = right_flag = left_flag = false;
	jump_velocity = 3.0;
	acceleration_rate = 8.0;
	y_gravity = 20.0;
}

void Player::calculate_enemy_collision(Enemy* enemy){
	if (this->Collides(enemy)){
		if (this->y -height/2 > enemy->get_y()){ 
			enemy->die(); 
			this->y_acceleration = 0;
			this->y_velocity = 3.0;
		}
		else if (this->x + width / 2 < enemy->get_x() && !hurt){
			audio->hurtSound();
			this->y_velocity += 1.0;
			this->x_velocity -= 30.0;
			enemy->set_y_velocity(enemy->get_y_velocity() +1.0);
			enemy->set_x_velocity(enemy->get_x_velocity() + 20);
			hurt = true;
		}
		else if (this->x - width / 2 > enemy->get_x() && !hurt){
			audio->hurtSound();
			this->y_velocity += 1.0;
			this->x_velocity += 30.0;
			enemy->set_y_velocity(enemy->get_y_velocity() + 1.0);
			enemy->set_x_velocity(enemy->get_x_velocity() - 20);
			hurt = true;
		}
	}
}

void Player::decide_frame(){
	if (abs(x_velocity) < minimum_velocity){ current_frame = 0; }
	else if (current_frame < walk_begin){ current_frame = walk_begin; }
	else if (animation_counter > animation_time){
		animation_counter = 0;
		current_frame++;
		if (current_frame > walk_end){ current_frame = walk_begin; }
	}
	if (!bottom_flag && y_velocity > 0){
		current_frame = 1;
	}
	if (hurt){current_frame = hurt_frame;}
	sprite = frames[current_frame];
}

void Player::update(float time_elapsed, Level* level){
	if (hurt && hurt_counter < hurt_time){ hurt_counter += time_elapsed; }
	else if (hurt && hurt_counter >= hurt_time){
		hurt = false;
		hurt_counter = 0;
	}
	decide_frame();
	Sprite::update(time_elapsed, level);
}

//getters+setters
Sheetposition Player::return_sheet_position() { return sprite; }
float Player::get_x() { return x; }
float Player::get_y() { return y; }
Matrix Player::get_matrix(){ return modelMatrix; }
