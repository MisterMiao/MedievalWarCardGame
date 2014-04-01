#include "Input.h"

static MouseInput* instance;

bool previousMButtonStates[5] = { false };
bool currentMButtonStates[5] = { false };
Vector2 currentMousePos;
bool mouseMoved = false;

MouseInput* MouseInput::GetInstance() {
	if (instance == nullptr)
		instance = new MouseInput();

	return instance;
}

void MouseInput::DestroyMouseInput(MouseInput* &mouseInputPointer) {
	if (instance) {
		delete instance;
		instance = nullptr;
		mouseInputPointer = nullptr;
	}
}

void MouseInput::SetPreviousStates() {
	mouseMoved = false;

	for (int i = 0; i < 5; i++)
		previousMButtonStates[i] = currentMButtonStates[i];
}

void MouseInput::Update(SDL_Event& mouseEvent) {	
	// Mouse click events
	if (mouseEvent.type == SDL_MOUSEBUTTONDOWN)
		currentMButtonStates[mouseEvent.button.button - 1] = true;
	if (mouseEvent.type == SDL_MOUSEBUTTONUP)
		currentMButtonStates[mouseEvent.button.button - 1] = false;

	// Mouse movement events
	if (mouseEvent.type == SDL_MOUSEMOTION) {
		mouseMoved = true;

		currentMousePos.X = (float)mouseEvent.motion.x;
		currentMousePos.Y = (float)mouseEvent.motion.y;
	}
}

bool MouseInput::MouseButtonDown(Uint8 button) {
	return currentMButtonStates[button - 1];
}

bool MouseInput::MouseButtonUp(Uint8 button) {
	return !currentMButtonStates[button - 1];
}

bool MouseInput::MouseButtonPress(Uint8 button) {
	return previousMButtonStates[button - 1] && !currentMButtonStates[button - 1]; 
}

bool MouseInput::GetMouseMoved() {
	return mouseMoved;
}

Vector2 MouseInput::GetMousePos() {
	return currentMousePos;
}