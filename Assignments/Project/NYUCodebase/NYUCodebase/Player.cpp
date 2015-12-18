#include "Player.h"

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}

Player::Player(float size, float x_position, float y_position, int texture, ShaderProgram* program){
	current_health = max_health = 10;
	current_mana = max_mana = 75;
	stretchy = true;
	this->program = program;
	position.set_x(x_position);
	position.set_y(y_position);
	this->size = size;
	coloruniform = glGetUniformLocation(program->programID, "color_shift");
	// Animation stuff. Here, set the positions of the various sprites used, toss them in array
	float sheet_width = 256;
	float sheet_length = 512;
	animation_time = .05f;
	/*<TextureAtlas imagePath="sprites.png">
	<SubTexture name="bathighflap.png" x="0" y="53" width="96" height="58"/>
	<SubTexture name="batlowflap.png" x="0" y="113" width="88" height="66"/>
	<SubTexture name="batmidhighflap.png" x="0" y="181" width="83" height="57"/>
	<SubTexture name="batmidlowflap.png" x="0" y="0" width="100" height="51"/>
	<SubTexture name="player_jumping.png" x="148" y="307" width="66" height="94"/>
	<SubTexture name="player_standing.png" x="148" y="209" width="66" height="96"/>
	<SubTexture name="player_walking1.png" x="102" y="0" width="72" height="94"/>
	<SubTexture name="player_walking2.png" x="90" y="113" width="72" height="94"/>
	<SubTexture name="player_walking3.png" x="74" y="336" width="72" height="94"/>
	<SubTexture name="player_walking4.png" x="74" y="240" width="72" height="94"/>
	<SubTexture name="player_walking5.png" x="0" y="336" width="72" height="94"/>
	<SubTexture name="player_walking6.png" x="0" y="240" width="72" height="94"/>
</TextureAtlas>*/
	Sheetposition p1_stand = Sheetposition(148, 209, 66, 96, this->size, sheet_width, sheet_length);
	Sheetposition p1_jump = Sheetposition(148, 307, 66, 94, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk01 = Sheetposition(102, 0, 72, 94, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk02 = Sheetposition(90, 113, 72, 94, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk03 = Sheetposition(74, 336, 72, 94, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk04 = Sheetposition(74, 240, 72, 94, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk05 = Sheetposition(0, 336, 72, 94, this->size, sheet_width, sheet_length);
	Sheetposition p1_walk06 = Sheetposition(0, 240, 72, 94, this->size, sheet_width, sheet_length);
	Sheetposition bat_highflap = Sheetposition(0, 53, 96, 57, this->size/2, sheet_width, sheet_length);
	Sheetposition bat_midhighflap = Sheetposition(0, 181, 83, 57, this->size/2, sheet_width, sheet_length);
	Sheetposition bat_midlowflap = Sheetposition(0, 0, 100, 51, this->size/2, sheet_width, sheet_length);
	Sheetposition bat_lowflap = Sheetposition(0, 113, 88, 66, this->size/2, sheet_width, sheet_length);
	spritesheet = SheetSprite(texture, p1_stand);
	frames = new Sheetposition[12];
	Sheetposition holder[] = { p1_stand, p1_jump, p1_walk01, p1_walk02, p1_walk03, p1_walk04, p1_walk05, p1_walk06,
		bat_highflap, bat_midhighflap, bat_midlowflap, bat_lowflap};
	for (int i = 0; i < 12; i++){
		frames[i] = holder[i];
	}

	hurt_frame = 1;
	walk_begin = 2;
	walk_end = 7;
	sprite = p1_stand;
	hurt_time = .5f;

	
	
	// physics stuff
	bottom_flag = top_flag = right_flag = left_flag = false;
	jump_velocity = 3.5;
	acceleration_rate = 17.5;
	gravity.set_y(25.0);
	friction.set_x(5.0);
	friction.set_y(0);
}
void Player::Transform(){
	if (time_since_transformed > min_transform_wait){
		Color start_color;
		start_color.r = 0;
		start_color.g = 1.0;
		start_color.b = 0;
		start_color.a = 1.0;
		Color end_color = start_color;
		end_color.a = 0.0;
		Color color_deviation;
		color_deviation.a = 0.0;
		audio->transformSound();
		special_effects->smoke_puff(position, start_color, end_color, color_deviation);
		if (transformed){
			time_since_transformed = 0.0f;
			transform_timer = 0.0;
			transformed = false;
			gravity.set_y(25.0);
			hurt_frame = 7;
			stretchy = true;
		}
		else{
			velocity.set_y(0);
			acceleration.set_y(0);
			time_since_transformed = 0.0f;
			transform_timer = 0.0;
			transformed = true;
			gravity.set_y(0.0);
			hurt_frame = 8;
			current_frame = 8;
			stretchy = false;
		}
	}
	else{ audio->negativeSound(); }
}
void Player::calculate_enemy_collision(Enemy* enemy){
	if (this->Collides(enemy)){
		if (this->position.get_y() -height/2 > enemy->get_y()){ 
			enemy->die(); 
			this->acceleration.set_y(0);
			this->velocity.set_y(3.0);
		}
		else if (this->position.get_x()+ width / 2 < enemy->get_position().get_x() && !hurt){
			audio->hurtSound();
			this->velocity.set_y(this->velocity.get_y()+1.0);
			this->velocity.set_x(this->velocity.get_x() - 30.0);
			enemy->set_y_velocity(enemy->get_y_velocity() +1.0);
			enemy->set_x_velocity(enemy->get_x_velocity() + 20);
			hurt = true;
		}
		else if (this->position.get_x() - width / 2 > enemy->get_position().get_x() && !hurt){
			audio->hurtSound();
			this->velocity.set_y(this->velocity.get_y() + 1.0);
			this->velocity.set_x(this->velocity.get_x() + 30.0);
			enemy->set_y_velocity(enemy->get_y_velocity() + 1.0);
			enemy->set_x_velocity(enemy->get_x_velocity() - 20);
			hurt = true;
		}
	}
}

void Player::decide_frame(){
	if (transformed){
		
		if (animation_counter > animation_time){
			current_frame += s;
			animation_counter = 0;
		}
		if (current_frame > 11){
			current_frame = 11;
			s = -1;
		}
		else if (current_frame < 8) {
			current_frame = 8;
			s = 1;
		}
	}
	else{
		if (abs(velocity.get_x()) < minimum_velocity){ current_frame = 0; }
		else if (current_frame < walk_begin){ current_frame = walk_begin; }
		else if (animation_counter > animation_time){
			animation_counter = 0;
			current_frame++;
			if (current_frame > walk_end){ current_frame = walk_begin; }
		}
		if (!bottom_flag && velocity.get_y() > 0){
			current_frame = 1;
		}
	}
	if (hurt){ current_frame = hurt_frame; }
	sprite = frames[current_frame];
	if (transformed && current_frame < 8 || transformed && current_frame > 11){
		DBOUT(current_frame);
	}
}

void Player::update(float time_elapsed, Level* level){
	time_since_transformed += time_elapsed;
	if (transformed) { transform_timer+=time_elapsed; }
	if (transform_timer > transform_time_limit){
		Transform();
	}
	update_count++;
	shoot_timer += time_elapsed;
	if (hurt && hurt_counter < hurt_time){ hurt_counter += time_elapsed; }
	else if (hurt && hurt_counter >= hurt_time){
		hurt = false;
		hurt_counter = 0;
	}
	decide_frame();
	Sprite::update(time_elapsed, level);
}

//getters+setters
Sheetposition Player::return_sheet_position() { return sprite; }
float Player::get_x() { return position.get_x(); }
float Player::get_y() { return position.get_y(); }
Matrix Player::get_matrix(){ return modelMatrix; }
