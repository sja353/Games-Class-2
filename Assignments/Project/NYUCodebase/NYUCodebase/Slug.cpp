#include "Slug.h"
Slug::Slug(float size, float x_position, float y_position, int texture, ShaderProgram* program){
	this->program = program;
	x = x_position;
	y = y_position;
	this->size = size;
	
	// Animation stuff
	animation_time = .5f;
	float sheet_width = 353;
	float sheet_length = 153;
	Sheetposition slimeDead = Sheetposition(0, 112, 59, 12, this->size/2, sheet_width, sheet_length);
	Sheetposition slimeWalk1 = Sheetposition(52, 125, 50, 28, this->size, sheet_width, sheet_length);
	Sheetposition slimeWalk2 = Sheetposition(0, 125, 51, 26, this->size, sheet_width, sheet_length);
	spritesheet = SheetSprite(texture, slimeWalk1);
	frames = new Sheetposition[3];
	Sheetposition holder[]= { slimeWalk1, slimeWalk2, slimeDead};
	for (int i = 0; i < 3; i++){
		frames[i] = holder[i];
	}
	death_frame = 2;
	walk_begin = 0;
	walk_end = 1;
	sprite = slimeWalk1;
	
	
	
	acceleration_rate = 1.0;
	y_gravity = 1.0;
	bottom_flag = top_flag = right_flag = left_flag = false;
}

/*void Slug::get_behavior(Player* player){
	if (player->get_x() < x) {
		this->move_left();
		this->mirrored = !this->mirrored;
	}
	else{ 
		this->move_right(); 
		this->mirrored = !this->mirrored;
	}
}*/

