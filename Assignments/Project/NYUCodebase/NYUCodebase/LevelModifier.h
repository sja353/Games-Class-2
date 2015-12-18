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
	void add_tile(int x, int y){
		level->modify_tile(x, y, unsigned char(16));
	}
};