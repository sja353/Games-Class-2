#include "Matrix.h"
#include "ShaderProgram.h"
#include <SDL.h>
#ifdef _WINDOWS
	#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <stdlib.h>
#include "Shaderprogram.h"
#include "Matrix.h"
#include "Entity.h"
#include <vector>
#include "Bullet.h"

using namespace std;
class GameApp {
public:
	GameApp();
	~GameApp();

	void Setup();
	void ProcessEvents();
	bool UpdateAndRender();
	void EnemyActions();

	void Render();
	void Update();

	void Drawtext(int fontTexture, std::string text, float size, float spacing);

	void Shoot_player_bullet();
	void Shoot_enemy_bullet(Entity* enemy);
	
private:
	
	bool done;
	float lastFrameTicks;
	SDL_Window* displayWindow;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	ShaderProgram* program;
	SDL_Event event;
	const Uint8* keys;
	GLuint LoadTexture(const char* image_path, int img_type);
	const float FIXED_TIMESTEP = 0.01666666f;
	const int MAX_TIMESTEP = 6;
	float time_left_over = 0.0f;
	GLuint sprite_sheet_texture, background, font;
	Entity player, left_wall, right_wall; 
	vector<Entity> enemies;

	float player_recharge = 0.5f;
	float recharge_timer = 0.0f;
	SheetSprite player_bullet[4];
	SheetSprite enemy_bullet[4];
	int player_bullet_index = 0;
	int enemy_bullet_index = 0;
	const static int MAX_PLAYER_BULLETS = 30;
	const static int MAX_ENEMY_BULLETS = 100;
	Bullet player_bullets[MAX_PLAYER_BULLETS];
	Bullet enemy_bullets[MAX_ENEMY_BULLETS];
	int player_score = 0;
	int enemies_alive;
	bool player_win = false;

	enum {MAIN_MENU, GAMEPLAY, GAME_OVER};
	int state;
	void RenderMainMenu();
	void RenderGamePlay();
	void RenderGameOver();

	void UpdateMainMenu();
	void UpdateGamePlay();
	void UpdateGameOver();

	void DrawBackGround();
};