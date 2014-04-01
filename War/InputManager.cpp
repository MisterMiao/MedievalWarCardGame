#include "Engine.h"

static InputManager* instance;

InputManager* InputManager::GetInstance() {
	if (instance == nullptr)
		instance = new InputManager();

	return instance;
}

void InputManager::DestroyInputManager(InputManager*& inputManagerPointer) {
	if (instance) {
		delete instance;
		instance = nullptr;
		inputManagerPointer = nullptr;
	}
}

void InputManager::DeleteAllInputPointers() {
	keyboardInstance->DestroyKeyboardInput(keyboardInstance);
	mouseInstance->DestroyMouseInput(mouseInstance);
}

InputManager::InputManager() {
	keyboardInstance = KeyboardInput::GetInstance();
	mouseInstance = MouseInput::GetInstance();
}

InputManager::~InputManager() {
	DeleteAllInputPointers();
}

bool InputManager::Update(SDL_Event& inputEvent) {
	keyboardInstance->SetPreviousToCurrent();
	mouseInstance->SetPreviousStates();

	if (SDL_PollEvent(&inputEvent)) {
		if (inputEvent.type == SDL_QUIT)
			return true;

		keyboardInstance->Update(inputEvent);
		mouseInstance->Update(inputEvent);
	}

	return false;
}

bool InputManager::KeyDown(Uint32 key) {
	return keyboardInstance->KeyDown(key);
}

bool InputManager::KeyUp(Uint32 key) {
	return keyboardInstance->KeyUp(key);
}

bool InputManager::KeyPress(Uint32 key) {
	return keyboardInstance->KeyPress(key);
}

bool InputManager::MouseButtonDown(Uint8 button) {
	return mouseInstance->MouseButtonDown(button);
}

bool InputManager::MouseButtonUp(Uint8 button) {
	return mouseInstance->MouseButtonUp(button);
}

bool InputManager::MouseButtonPress(Uint8 button) {
	return mouseInstance->MouseButtonPress(button);
}

bool InputManager::GetMouseMoved() {
	return mouseInstance->GetMouseMoved();
}

Vector2 InputManager::GetMousePos() {
	return mouseInstance->GetMousePos();
}