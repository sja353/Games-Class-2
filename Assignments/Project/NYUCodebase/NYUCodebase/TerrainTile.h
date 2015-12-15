#pragma once
#include "Entity.h"
#include "Sheetposition.h"
//#include "Projectile.h"
#include "ParticleEmitter.h"
#include "Light.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}
class TerrainTile: public Entity{
public:
	virtual void update(float time_elapsed){}
	~TerrainTile(){
		if (has_light){
			light->turn_off();
		}
	}
	TerrainTile(){};
	TerrainTile(const TerrainTile &tile);
	TerrainTile& operator=(const TerrainTile &rhs);
	TerrainTile(float x, float y, int texture, Sheetposition position, ShaderProgram* program);


	void set_behaviors(bool top, bool bottom, bool right, bool left);
	void set_harmful_behaviors(bool top, bool bottom, bool right, bool left){
		hurts_top = top; 
		hurts_bottom = bottom;
		hurts_right = right;
		hurts_left = left;
	}
	//getters/setters
	bool top_is_solid(){ return top_solid; }
	bool bottom_is_solid() { return bottom_solid; }
	bool right_is_solid() { return right_solid; }
	bool left_is_solid(){ return left_solid; }
	float get_width() { return width; }
	bool is_there(){ return exists; }
	void set_exists(bool a){ exists = a; }
	bool get_destructible(){ return destructible; }
	void set_light(Light* light) { this->light = light; }
	void set_light_inactive() { light->turn_inactive(); }
	void set_light_active() { light->turn_active(); }
	bool is_lit(){ return has_light; }
	bool damage_top() { return hurts_top; }
	bool damage_bottom(){ return hurts_bottom; }
	bool damage_right() { return hurts_right; }
	bool damage_left(){ return hurts_left; }
	int get_damage() { return damage; }
	void set_damage(int damage) { this->damage = damage; }
	void set_destructible(bool destructible) { this->destructible = destructible; }
private:
	bool top_solid, bottom_solid, right_solid, left_solid;
	bool hurts_top = false, hurts_bottom = false, hurts_right = false, hurts_left = false;
	Light* light;
	int damage = 5;
	bool exists = false;
	bool destructible = true;
	bool has_light = false;
};