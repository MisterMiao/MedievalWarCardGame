#ifndef ENGINE_H
#define ENGINE_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <time.h>
#include <string>
#include <sstream>
#include <map>
#include "HelperFunctions.h"
#include "Input.h"
#include "Geometry.h"
using namespace std;

// Forward declarations
class Game;
class ContentManager;
class InputManager;
class Timer;

// Typedefs of iterators for friendlyness
typedef map<string, SDL_Texture*>::iterator Texture2D; // Used to determine where in the textures std::map a SDL_Texture* is
typedef map<string, TTF_Font*>::iterator Font;
typedef map<string, Mix_Music*>::iterator Music;
typedef map<string, Mix_Chunk*>::iterator Sound;

// Define "null" sound as the end of the sound map
#define NULL_SOUND ContentManager::GetInstance()->NullSound()

// The class which governs the game loop and all services required by the game
class Game {
public:
	// Calls Init(), and then starts the game loop
	void Run();
protected:
	// The renderer for the game
	SDL_Renderer* renderer;
	// The content manager for the game (loading textures, etc)
	ContentManager* content;
	// The input manager for the game (mouse & keyboard)
	InputManager* input;

	// The title of the game, and the window resolution
	string gameTitle;
	Vector2 screenDimensions;

	// Initializes the game window, renderer, content manager,
	// and anything else important before any loading is done
	virtual void Init() = 0;
	// Loads any textures, sounds, fonts, etc,
	// using this.content
	virtual void Load() = 0;
	// Destroys the game window, renderer, and unloads
	// all content in the content manager
	virtual void Unload() = 0;
	// Updating of game objects
	virtual void Update(const float) = 0;
	// Drawing of game objects
	virtual void Draw() = 0;

	// Stops the game loop so that Quit() can be called
	void Exit() { running = false; }
private:
	// The main window for the game
	SDL_Window* window;
	// The input event handler for the game
	SDL_Event event;
	// Whether or not the game should be looping
	bool running;

	// Calls Unload() and quits all SDL libraries
	void Quit();
};

// The class that handles loading and unloading of external content, such as images, fonts, and sounds
class ContentManager {
public:
	static ContentManager* GetInstance();
	void DestroyContentManager(ContentManager*&);

	// The game's renderer, for loading textures
	SDL_Renderer* renderer;

	// Loads a SDL_Texture into the content manager
	Texture2D LoadTexture(string);
	// Loads a TTF_Font into the content manager
	Font LoadFont(string, int);
	// Loads a Mix_Music into the content manager
	Music LoadMusic(string);
	// Loads a Mix_Chunk into the content manager
	Sound LoadSound(string);

	// Used for a card that does not have a sound
	Sound NullSound() { return sounds.end(); }
	// Get the pixel dimensions of a string
	Vector2 MeasureString(string, Font);

	// Draws a texture
	void DrawTexture(Texture2D texture, const Vector2* position = NULL, const Rectangle* srcRect = NULL, float rotation = 0.0f, float scale = 1.0f, const Vector2* origin = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
	// Draws a string
	void DrawString(string, const Vector2*, Font, SDL_Color, bool = false);

	~ContentManager();
private:
	// A list of all textures loaded into the content manager
	map<string, SDL_Texture*> textures;
	// A list of all fonts loaded
	map<string, TTF_Font*> fonts;
	// A list of all music loaded
	map<string, Mix_Music*> music;
	// A list of all sound effects loaded
	map<string, Mix_Chunk*> sounds;

	// The root directory which all content is located in
	string rootDirectory;

	// Loops through the lists of content and calls their respective SDL_Destroy
	void Unload();

	ContentManager();
};

// The class that handles mouse and keyboard input
class InputManager {
public:
	// Get (and create) the InputManager instance
	static InputManager* GetInstance();
	// Deletes the input manager and the pointer to it, as well as KeyboardInput/MouseInput objects
	void DestroyInputManager(InputManager*&);

	// Updates the keyboard and mouse states
	bool Update(SDL_Event&);

	/// Keyboard functions
	bool KeyDown(Uint32); // Checks if a key is held down
	bool KeyUp(Uint32); // Checks if a key is up (released)
	bool KeyPress(Uint32); // Checks if a key has been pressed (but not held)

	/// Mouse functions
	bool MouseButtonDown(Uint8); // If a button is held
	bool MouseButtonUp(Uint8); // If a button is up
	bool MouseButtonPress(Uint8); // If a button is pressed
	bool GetMouseMoved(); // If the mouse has moved since last frame
	Vector2 GetMousePos(); // The mouse's current position

	~InputManager();
private:
	InputManager(); // Private constructor for singleton class

	KeyboardInput* keyboardInstance;
	MouseInput* mouseInstance;

	void DeleteAllInputPointers();
};

// A generic timer class that can be either a running time or countdown
class Timer {
public:
	// Start the timer, and provide a time for it to countdown if wanted
	void Start(int = 0);
	// Stop the timer
	void Stop();
	// Pause the timer
	void Pause();
	// Unpause the timer
	void UnPause();
	// Restart the timer and return the elapsed ticks
	int Restart();
	// Get the elapsed ticks
	int Elapsed() const;
	// Get the remaining ticks
	int Remaining() const;
	// Whether or not the timer is started
	bool Started() const;
	// Whether or not the timer is paused
	bool Paused() const;
	// Get the elapsed/remaining ticks in HH:MM:SS format
	string ToString() const;

	Timer();
private:
	int startTime, pauseTime, runningTime;
	bool started, paused, reverse;
};
#endif