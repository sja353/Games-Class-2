#ifndef TERRAINTILE_HEADER
#define TERRAINTILE_HEADER

#include "Entity.h"
#include "Sheetposition.h"



#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}
class TerrainTile: public Entity{
public:

	TerrainTile(){};
	TerrainTile(const TerrainTile &tile);
	TerrainTile& operator=(const TerrainTile &rhs);
	TerrainTile(float x, float y, int texture, Sheetposition position, ShaderProgram* program);


	void set_behaviors(bool top, bool bottom, bool right, bool left);
	//getters/setters
	bool top_is_solid(){ return top_solid; }
	bool bottom_is_solid() { return bottom_solid; }
	bool right_is_solid() { return right_solid; }
	bool left_is_solid(){ return left_solid; }
	float get_width() { return width; }
	bool is_there(){ return exists; }
	void set_exists(bool a){ exists = a; }

private:
	bool top_solid, bottom_solid, right_solid, left_solid;
	bool exists = false;

};
#endif