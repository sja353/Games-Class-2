#include "TerrainTile.h"

void TerrainTile::set_behaviors(bool top, bool bottom, bool right, bool left){
	top_solid = top;
	bottom_solid = bottom;
	right_solid = right;
	left_solid = left;
}
TerrainTile::TerrainTile(const TerrainTile &tile){
	this->position.set_x(tile.get_x());
	this->position.set_y(tile.get_y());
	this->spritesheet = tile.spritesheet;
	this->sprite = tile.sprite;
	this->program = tile.program;
	this->exists = tile.exists;
	this->top_solid = tile.top_solid;
	this->bottom_solid = tile.bottom_solid;
	this->right_solid = tile.right_solid;
	this->left_solid = tile.left_solid;
	this->width = tile.width;
	this->height = tile.height;
	this->hp = tile.hp;
	this->hurts_bottom = tile.hurts_bottom;
	this->hurts_right = tile.hurts_right;
	this->hurts_left = tile.hurts_left;
	this->hurts_top = tile.hurts_top;
	this->damage = tile.damage;
	this->destructible = tile.destructible;
	this->has_light = tile.has_light;
	this->light = tile.light;
}

TerrainTile& TerrainTile::operator= (const TerrainTile &rhs){
	this->position.set_x(rhs.get_x());
	this->position.set_y(rhs.get_y()); 
	this->spritesheet = rhs.spritesheet;
	this->sprite = rhs.sprite;
	this->program = rhs.program;
	this->exists = rhs.exists;
	this->top_solid = rhs.top_solid;
	this->bottom_solid = rhs.bottom_solid;
	this->right_solid = rhs.right_solid;
	this->left_solid = rhs.left_solid;
	this->width = rhs.width;
	this->height = rhs.height;
	this->hp = rhs.hp;
	this->hurts_bottom = rhs.hurts_bottom;
	this->hurts_right = rhs.hurts_right;
	this->hurts_left = rhs.hurts_left;
	this->hurts_top = rhs.hurts_top;
	this->damage = rhs.damage;
	this->destructible = rhs.destructible;
	this->has_light = rhs.has_light;
	this->light = rhs.light;
	return *this;
}

TerrainTile::TerrainTile(float x, float y, int texture, Sheetposition position, ShaderProgram* program){
	this->position.set_x(x);
	this->position.set_y(y);
	spritesheet = SheetSprite(texture, position);
	this->sprite = position;
	this->program = program;
}