#include "TerrainTile.h"

void TerrainTile::set_behaviors(bool top, bool bottom, bool right, bool left){
	top_solid = top;
	bottom_solid = bottom;
	right_solid = right;
	left_solid = left;

}