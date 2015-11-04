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
class Player : public Sprite{
public:
	~Player(){
		//Mix_FreeChunk(hurt_sound);
		//Mix_FreeChunk(jump_sound);
	}
	Player(){};
	Player(float size, float x_position, float y_position, int texture, ShaderProgram* program);
	void move_left(){ 
		Sprite::move_left(); 
	}

	void calculate_enemy_collision(Enemy* enemy);
	
	void move_right(){
		Sprite::move_right();
	}
	void idle(){}

	Sheetposition return_sheet_position() {
		return sprite;
	}

	void Draw(){
		Entity::Draw();
	}
	
	float get_x() { return x; }
	float get_y() { return y; }
	Matrix get_matrix(){ return modelMatrix; }
};
#endif