#pragma once
#include "Entity.h"
#include "TextDrawer.h"
#include "Vector.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

class InfoOverlay{
public:
	InfoOverlay(){}
	InfoOverlay(int graphic_texture, int font_texture, ShaderProgram* program){
		this->graphic_texture = graphic_texture;
		this->font_texture = font_texture;
		this->program = program;
		health.set_program(program);
		mana.set_program(program);
		float sheet_width = 128;
		float sheet_height = 64;
		Sheetposition mana_circle = Sheetposition(0, 0, 58, 58, .5, sheet_width, sheet_height);
		Sheetposition health_circle = Sheetposition(60, 0, 58, 58,  .5, sheet_width, sheet_height);
		SheetSprite health_sprite = SheetSprite(graphic_texture, health_circle);
		SheetSprite mana_sprite = SheetSprite(graphic_texture, mana_circle);
		
		health.set_sprite(health_circle);
		mana.set_sprite(mana_circle);
		health.set_spritesheet(health_sprite);
		mana.set_spritesheet(mana_sprite);
	}
	void DrawOverlay(int current_health, int max_health, int current_mana, int max_mana, Vector player_position){
		Vector health_position = player_position;
		Vector mana_position = player_position;
		health_position.set_x(health_position.get_x() - 3.2);
		health_position.set_y(health_position.get_y() + 2.0);
		mana_position.set_x(mana_position.get_x() - 2.5);
		mana_position.set_y(mana_position.get_y() + 2.0);
		
		health.set_position_vector(health_position);
		mana.set_position_vector(mana_position);
		std::string health_string = std::to_string(current_health) + "/" + std::to_string(max_health);
		std::string mana_string = std::to_string(current_mana) + "/" + std::to_string(max_mana);
		health.Draw();
		TextDrawer::Drawtext(font_texture, health_string, .1, -.05, program, -.1);
		mana.Draw();
		TextDrawer::Drawtext(font_texture, mana_string, .1, -.05, program, -.1);
	}
private:
	Entity health, mana;
	int graphic_texture, font_texture;
	ShaderProgram* program;

};