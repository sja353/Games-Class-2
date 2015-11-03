#include "Player.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

Player::Player(float size, float x_position, float y_position, int texture, ShaderProgram* program){
	
	this->program = program;
	animation_time = .05f;
	x = x_position;
	y = y_position;
	this->size = size;
	float sheet_width = 508;
	float sheet_length = 288;
	/*
	p1_duck = 365 98 69 71
	p1_front = 0 196 66 92
	p1_hurt = 438 0 69 92
	p1_jump = 438 93 67 94
	p1_stand = 67 196 66 92
	p1_walk01 = 0 0 72 97
	p1_walk02 = 73 0 72 97*/
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
	first_animation_frame = 5;
	last_animation_frame = 15;
	sprite = p1_stand;
	y_gravity = .15;
	bottom_flag = top_flag = right_flag = left_flag = false;
	jump_velocity = 6.0;
	acceleration_rate = 10.0;
}

void Player::calculate_enemy_collision(Enemy* enemy){
	if (this->Collides(enemy)){
		if (this->y -height/2 > enemy->get_y()){ 
			enemy->die(); 
			this->y_acceleration = 0;
			this->y_velocity = 3.0;
		}
		else if (this->x + width / 2 < enemy->get_x() && !hurt){
			this->y_velocity += 1.0;
			this->x_velocity -= 30.0;
			hurt = true;
		}
		else if (this->x - width / 2 > enemy->get_x() && !hurt){
			this->y_velocity += 1.0;
			this->x_velocity += 30.0;
			hurt = true;
		}
	}
}
