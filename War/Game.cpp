#include "Engine.h"

void Game::Init() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		cout << "Failed to initialize SDL: " << SDL_GetError() << endl;
		return;
	}

	// Initialize SDL_image
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cout << "Failed to initialize SDL_image: " << IMG_GetError() << endl;
		return;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1) {
		cout << "Failed to initialize SDL_ttf: " << TTF_GetError() << endl;
		return;
	}

	// Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		cout << "Failed to initialize SDL_mixer: " << Mix_GetError() << endl;
		return;
	}

	window = SDL_CreateWindow(gameTitle.c_str(), 100, 100, (int)screenDimensions.X, (int)screenDimensions.Y, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	content = ContentManager::GetInstance();
	content->renderer = renderer;
	input = InputManager::GetInstance();

	srand((unsigned int)time(NULL));

	Load();
}

void Game::Unload() {
	input->DestroyInputManager(input);
	content->DestroyContentManager(content);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Game::Quit() {
	Unload();
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void Game::Run() {
	Init();

	running = true;
	int frameTime = 0, lastTime = 0, delta = 0;

	// Game loop
	while (running) {
		lastTime = frameTime;
		frameTime = SDL_GetTicks();
		delta = frameTime - lastTime;

		if (input->Update(event)) // input->Update returns whether or not the SDL_QUIT event was dispatched
			Exit();

		Update(delta / 1000.0F);
		Draw();

		SDL_Delay(1); // Save CPU usage
	}

	Quit();
}