#include "War.h"

War* War::instance = nullptr;

void War::Init() {
	gameTitle = "Medieval War";
	screenDimensions = Vector2(1280, 960);
	state = Splash;
	lastTime = 0;
	instance = this;

	Game::Init();
}

void War::Load() {
	try {
		splashScreen = content->LoadTexture("Splash.png");

		background = new MovingBackground(screenDimensions);
		//background->AddLayer(content->LoadTexture("Back.png")); // Background
		background->AddLayer(content->LoadTexture("Rotunda.png"), 0.0F, 5.0F); // Spinning church rotunda
		background->AddLayer(content->LoadTexture("Fire1.png"), 50.0F); // Moving fire on left side
		background->AddLayer(content->LoadTexture("Fire2.png"), -50.0F); // Moving fire on right side
		background->AddLayer(content->LoadTexture("Overlay.png"));
		background->AddLayer(content->LoadTexture("CardPositions.png")); // Card position outlines
		background->SetMovement(false); // Move vertically

		Card::backImage = content->LoadTexture("Cards/CardBack.png");

		Card::font = content->LoadFont("SourceSansPro-Bold.ttf", 15); // The font for cards
		Player::font = font = content->LoadFont("SourceSansPro-Bold.ttf", 24); // The font for the board

		splashMusic = content->LoadMusic("splash_music.wav");
		gameMusic = content->LoadMusic("game_music.wav");
		endMusic = content->LoadMusic("end_music.wav");

		stats = Stats("Stats.xml");

		player = new Player(content);
		enemy = new Player(content);

		background->Start();
	} catch (const runtime_error& e) {
		cout << e.what() << endl;
		Exit();
	}
}

void War::Unload() {
	delete background;
	delete player;
	delete enemy;
	Game::Unload();
}

void War::EndGame(bool win) {
	Mix_HaltMusic();

	stats.EndGame(win); // Update win/loss
	Player::message = win ? "You Win!" : "You Lose!";

	if (Mix_PlayingMusic() == 0)
		Mix_PlayMusic(endMusic->second, -1); // Play background music looping

	lastTime = SDL_GetTicks();
	state = End;
}

void War::Update(const float deltaTime) {
	if (input->KeyPress(SDL_SCANCODE_ESCAPE))
		Exit();

	switch (state) {
	case Splash:
		if (Mix_PlayingMusic() == 0) // If music is not playing
			Mix_PlayMusic(splashMusic->second, -1); // Play splashscreen music

		if (input->KeyPress(SDL_SCANCODE_RETURN)) { // Show splash screen until enter is pressed
			runTimer.Start();
			lastTime = SDL_GetTicks();
			Mix_HaltMusic();
			state = Waiting; // Start the game
		}
		break;
	case Waiting:
		if (Mix_PlayingMusic() == 0)
			Mix_PlayMusic(gameMusic->second, -1); // Play background music looping

		if (!Player::turnTimer.Started())
			Player::turnTimer.Start(3000); // Temporarily use as a start timer

		if (Player::turnTimer.Remaining() == -1) { // When timer is up, start the game
			Player::Start();
			lastTime = SDL_GetTicks();
			state = Playing;
		}
	case Playing:
		if (input->KeyPress(SDL_SCANCODE_M)) // Pause/Unpause the music by pressing M
			if (Mix_PausedMusic())
				Mix_ResumeMusic();
			else
				Mix_PauseMusic();

		background->Update(deltaTime);

		player->Update(deltaTime);
		enemy->Update(deltaTime);
		break;
	case End:
		if (SDL_GetTicks() > (unsigned)(3000 + lastTime)) // Wait three seconds to show stats, then exit
			Exit();
		break;
	}
}

void War::Draw() {
	SDL_RenderClear(renderer);

	switch (state) {
	case Splash:
		content->DrawTexture(splashScreen);
		break;
	case Waiting:
	case End:
	case Playing:
		background->Draw(content);

		content->DrawString("Time: " + runTimer.ToString(), &Vector2(1145, 460), font, CreateColor(255, 255, 255), true); // Draw the total game time
		content->DrawString("Turn Time: " + Player::turnTimer.ToString(), &Vector2(1145, 500), font, CreateColor(255, 255, 255), true); // Draw the turn timer

		if (strcmp(Player::message.c_str(), "") != 0) // Draw the turn state message
			content->DrawString(Player::message, &Vector2(screenDimensions.X / 2.0F, screenDimensions.Y / 2.0F), font, CreateColor(255, 255, 255), true);

		// Draw the players' hand, board, deck, discard pile, etc
		player->Draw(content);
		enemy->Draw(content);
		break;
	}

	SDL_RenderPresent(renderer);
}

War* War::GetInstance() {
	return instance;
}