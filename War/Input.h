#ifndef INPUT_H
#define INPUT_H
#include <SDL.h>
#include "Geometry.h"

class KeyboardInput;
class MouseInput;

class KeyboardInput {
public:
	static KeyboardInput* GetInstance();
	void DestroyKeyboardInput(KeyboardInput*&);

	void SetPreviousToCurrent();
	void Update(SDL_Event&);

	bool KeyDown(Uint32);
	bool KeyUp(Uint32);
	bool KeyPress(Uint32);
private:
	KeyboardInput() {}
};

class MouseInput {
public:
	static MouseInput* GetInstance();
	void DestroyMouseInput(MouseInput*&);

	void SetPreviousStates();
	void Update(SDL_Event&);

	bool MouseButtonDown(Uint8);
	bool MouseButtonUp(Uint8);
	bool MouseButtonPress(Uint8);
	bool GetMouseMoved();
	Vector2 GetMousePos();
private:
	MouseInput() {}
};
#endif