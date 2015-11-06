#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include "Sprite.h"
#include "Enemy.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

// Purpose: Handle player-specific movements and animation decisions

class Player : public Sprite{
public:

	Player(){};
	Player(float size, float x_position, float y_position, int texture, ShaderProgram* program);
	void calculate_enemy_collision(Enemy* enemy);
	
	// getters+setters
	Sheetposition return_sheet_position();
	float get_x();
	float get_y();
	Matrix get_matrix();
	void decide_frame();
	void update(float time_elapsed, Level* level);

protected:
	int walk_begin, walk_end, hurt_frame;
	bool hurt;
	float hurt_counter = 0;
	float hurt_time;
};
#endif