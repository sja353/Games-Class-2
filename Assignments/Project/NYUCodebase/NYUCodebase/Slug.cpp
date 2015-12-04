#include "Slug.h"
Slug::Slug(float size, float x_position, float y_position, int texture, ShaderProgram* program){
	this->program = program;
	position.set_x(x_position);
	position.set_y(y_position);
	this->size = size;
	
	// Animation stuff
	animation_time = .5f;
	float sheet_width = 256;
	float sheet_length = 512;
	Sheetposition slimeDead = Sheetposition(0, 66, 110, 39, this->size/2, sheet_width, sheet_length);
	Sheetposition slimeWalk1 = Sheetposition(0, 107, 110, 64, this->size, sheet_width, sheet_length);
	Sheetposition slimeWalk2 = Sheetposition(0, 0, 115, 64, this->size, sheet_width, sheet_length);
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
	
	
	
	acceleration_rate = .5;
	gravity.set_y(1.0);
	friction.set_x(25.0);
	bottom_flag = top_flag = right_flag = left_flag = false;
	hp = 10;
}


