#pragma once
#include "Enemy.h"
#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

class RedMonster :public Enemy{
public:
	~RedMonster(){
			delete frames;
	}
	RedMonster(){}
	RedMonster(float size, float x_position, float y_position, int texture, ShaderProgram* program){
		this->program = program;
		position.set_x(x_position);
		position.set_y(y_position);
		this->size = size;
		jump_velocity = 3.0;


		/*<SubTexture name = "monster.png" x = "112" y = "66" width = "74" height = "86" / >
		<SubTexture name = "monsterdead.png" x = "0" y = "173" width = "96" height = "86" / >
		<SubTexture name = "monsterjump.png" x = "0" y = "261" width = "93" height = "86" / >
		<SubTexture name = "monsterwalk1.png" x = "95" y = "261" width = "74" height = "86" / >
		<SubTexture name = "monsterwalk2.png" x = "76" y = "349" width = "74" height = "86" / >
		<SubTexture name = "monsterwalk3.png" x = "0" y = "349" width = "74" height = "86" / >
		<SubTexture name = "monsterwalk4.png" x = "98" y = "173" width = "74" height = "86" / >*/

		// Animation stuff
		animation_time = .05f;
		float sheet_width = 256;
		float sheet_length = 512;
		Sheetposition monsterStand = Sheetposition(112, 66, 74, 86, this->size, sheet_width, sheet_length);
		Sheetposition monsterWalk1 = Sheetposition(95, 261, 74, 86, this->size, sheet_width, sheet_length);
		Sheetposition monsterWalk2 = Sheetposition(76, 349, 74, 86, this->size, sheet_width, sheet_length);
		Sheetposition monsterWalk3 = Sheetposition(0, 349, 74, 86, this->size, sheet_width, sheet_length);
		Sheetposition monsterWalk4 = Sheetposition(98, 173, 74, 86, this->size, sheet_width, sheet_length);
		Sheetposition monsterJump = Sheetposition(0, 261, 93, 86, this->size, sheet_width, sheet_length);
		Sheetposition monsterdead = Sheetposition(0, 173, 96, 86, this->size, sheet_width, sheet_length);
		
		
		spritesheet = SheetSprite(texture, monsterStand);
		frames = new Sheetposition[7];
		Sheetposition holder[] = { monsterStand, monsterWalk1, monsterWalk2, monsterWalk3, monsterWalk4, monsterJump, monsterdead};
		for (int i = 0; i < 7; i++){
			frames[i] = holder[i];
		}
		death_frame = 6;
		walk_begin = 0;
		walk_end = 4;
		sprite = monsterStand;

		acceleration_rate = 1.5;
		gravity.set_y(25.0);
		friction.set_x(25.0);
		bottom_flag = top_flag = right_flag = left_flag = false;
		hp = 25;
	}

	void update(float time_elapsed, Level* level){
		if (hp <= 0 && !dead) { die(); }
		decide_frame();
		if (dead){
			velocity.set_x(0.0);
			//velocity.set_y(0.0);
			death_counter += time_elapsed;
			if (death_counter >= death_time){
				expired = true;
			}
		}
		Sprite::update(time_elapsed, level); 
	}
	void die(){
		audio->dieSound();
		current_frame = death_frame;
		dead = true;
		this->set_hitbox(0.3, 0.2);
	}
	void get_behavior(float player_x, float player_y) {
		if (!dead){
			float total_distance = abs(player_x - position.get_x());
			if (total_distance < 3.0){
				if (player_x < position.get_x() - 1.0) {
					this->move_left();
					this->mirrored = !this->mirrored;
				}
				else if (player_x > position.get_x() + 1.0){
					this->move_right();
					this->mirrored = !this->mirrored;
				}
				if (player_y > position.get_y() + 0.1){
					this->jump();
				}
			}
		}
	}
	void decide_frame(){
		if (abs(velocity.get_x()) < minimum_velocity){ current_frame = 0; }
		else if (animation_counter > animation_time){
			animation_counter = 0;
			current_frame++;
			if (current_frame > walk_end){ current_frame = walk_begin; }
		}
		
		if (abs(velocity.get_y()) > 0.5){ current_frame = 5; }
		if (dead){
			current_frame = death_frame;
		}
		sprite = frames[current_frame];
	}
private:
	float death_counter = 0.0f;
	float death_time = 5.0f;

	
};