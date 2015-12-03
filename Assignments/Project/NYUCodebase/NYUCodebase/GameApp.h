#ifndef GAMEAPP_HEADER
#define GAMEAPP_HEADER
#include "LevelScene.h"
#include "ProjectileManager.h"
#include "LightManager.h"
using namespace std;
class GameApp {
public:
	GameApp();
	~GameApp();
	bool UpdateAndRender();
	
	//void Shoot_player_bullet();
	//void Shoot_enemy_bullet(Entity* enemy);
	
private:
	
	void Setup();
	void ProcessEvents();
	void EnemyActions();
	void Render();
	void Update(float timestep);
	void Drawtext(int fontTexture, std::string text, float size, float spacing);
	GLuint LoadTexture(const char* image_path, int img_type);
	void RenderMainMenu();
	void RenderGamePlay();
	void RenderGameOver();
	void UpdateMainMenu(float timestep);
	void UpdateGamePlay(float timestep);
	void UpdateGameOver(float timestep);
	void DrawBackGround();


	bool done;
	float lastFrameTicks =0.0f;
	SDL_Window* displayWindow;
	Matrix projectionMatrix;
	Matrix modelMatrix;
	Matrix viewMatrix;
	ShaderProgram* program;
	SDL_Event event;
	const Uint8* keys;
	const float FIXED_TIMESTEP = 0.01666666f;
	const int MAX_TIMESTEP = 6;
	float time_left_over = 0.0f;
	Player player;
	Slug slug;
	std::vector<Enemy*> enemies;
	std::vector<Enemy*> spawned_enemies;
	Level* level;
	GLuint background;
	Audio* audio;
	SpecialEffects* special_effects;
	enum {MAIN_MENU, GAMEPLAY, GAME_OVER};
	int state;
	
	//testing
	int count = 0;
	ProjectileManager* projectile_manager;
	LightManager* light_manager;
};
#endif