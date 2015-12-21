#include "Slug.h"
Slug::Slug(float size, float x_position, float y_position, int texture, ShaderProgram* program, 
	float color_r, float color_g, float color_b, LightManager* light_manager){
	this->program = program;
	position.set_x(x_position);
	position.set_y(y_position);
	this->size = size;
	color_shift[0] = color_r;
	color_shift[1] = color_g;
	color_shift[2] = color_b;
	coloruniform = glGetUniformLocation(program->programID, "color_shift");
	light = new Light();
	light->a = .15;
	light->b = 2;
	light->position.set_x(x_position);
	light->position.set_y(y_position);
	light->tint.r = color_r;
	light->tint.g = color_g;
	light->tint.b = color_b;
	light->tint.a = 1.0;
	light->is_off = false;
	light->slug_light = true;
	light_manager->accept_light(light);
	
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
	
	
	
	acceleration_rate = 15.0;
	gravity.set_y(1.0);
	friction.set_x(25.0);
	bottom_flag = top_flag = right_flag = left_flag = false;
	hp = 10;
	mirrored = true;
}


