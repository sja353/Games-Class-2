#include "Level.h"
#include "TerrainTile.h"

class LevelModifier{
private:
	Level* level;
public:
	float get_tilesize(){ return level->get_tilesize(); }
	LevelModifier(){}
	LevelModifier(Level* level){
		this->level = level;
	}
	bool add_tile(int x, int y){
		bool success;
		success = level->modify_tile(x, y, unsigned char(16));
		return success;
	}
};