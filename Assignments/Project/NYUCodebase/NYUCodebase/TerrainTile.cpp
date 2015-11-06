#include "TerrainTile.h"

void TerrainTile::set_behaviors(bool top, bool bottom, bool right, bool left){
	top_solid = top;
	bottom_solid = bottom;
	right_solid = right;
	left_solid = left;
}
TerrainTile::TerrainTile(const TerrainTile &tile){
	this->x = tile.x;
	this->y = tile.y;
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
}

TerrainTile& TerrainTile::operator= (const TerrainTile &rhs){
	this->x = rhs.x;
	this->y = rhs.y;
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
	return *this;
}

TerrainTile::TerrainTile(float x, float y, int texture, Sheetposition position, ShaderProgram* program){
	this->x = x;
	this->y = y;
	spritesheet = SheetSprite(texture, position);
	this->sprite = position;
	this->program = program;
}