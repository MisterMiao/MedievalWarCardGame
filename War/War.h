#ifndef WAR_H
#define WAR_H
#include "Player.h"
#include "Stats.h"
#include "MovingBackground.h"

class War : public Game {
private:
	static War* instance;
	enum GameState { Splash, Waiting, Playing, End };

	MovingBackground* background;
	Texture2D splashScreen;
	Font font;
	Music splashMusic;
	Music gameMusic;
	Music endMusic;

	Player* player;
	Player* enemy;

	Stats stats; // Stats loaded from XML file for win/loss
	Timer runTimer; // Timer for how long the game has ran
	GameState state; // The state of the game (ie loading, playing, etc)
	int lastTime; // Time since last state change
protected:
	void Init();
	void Load();
	void Unload();
	void Update(const float);
	void Draw();
public:
	static War* GetInstance();

	void EndGame(bool);
};
#endif