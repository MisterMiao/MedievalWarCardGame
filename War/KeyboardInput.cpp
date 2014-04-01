#include "Input.h"

static KeyboardInput* instance;

bool currentKeyStates[323] = { false };
bool previousKeyStates[323] = { false };

KeyboardInput* KeyboardInput::GetInstance() {
	if (instance == nullptr)
		instance = new KeyboardInput();

	return instance;
}

void KeyboardInput::DestroyKeyboardInput(KeyboardInput*& pointerToKeyboard) {
	if (instance) {
		delete instance;
		instance = nullptr;
		pointerToKeyboard = nullptr;
	}
}

void KeyboardInput::SetPreviousToCurrent() {
	for (int i = 0; i < 323; i++)
		previousKeyStates[i] = currentKeyStates[i];
}

void KeyboardInput::Update(SDL_Event& keyboardEvent) {
	SDL_Keycode key = keyboardEvent.key.keysym.scancode;

	if (keyboardEvent.type == SDL_KEYDOWN)
		currentKeyStates[key] = true;
	if (keyboardEvent.type == SDL_KEYUP)
		currentKeyStates[key] = false;
}

bool KeyboardInput::KeyDown(Uint32 key) {
	return currentKeyStates[key];
}

bool KeyboardInput::KeyUp(Uint32 key) {
	return !currentKeyStates[key];
}

bool KeyboardInput::KeyPress(Uint32 key) {
	return previousKeyStates[key] && !currentKeyStates[key];
}
