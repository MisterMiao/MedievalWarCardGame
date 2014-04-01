#ifndef SPRITE_H
#define SPRITE_H
#include "Engine.h"

class Sprite {
protected:
	Texture2D image;
public:
	Vector2 position;
	Vector2 velocity;
	Vector2 initVelocity;
	Rectangle bounds;
	SDL_RendererFlip flip;
	float rotation;
	float rotationSpeed;
	float scale;

	Rectangle CollisionRect();

	virtual void Update(const float);
	virtual void Draw(ContentManager*);

	Sprite(Texture2D texture, Vector2 position = Vector2::Zero, Vector2 velocity = Vector2::Zero, float rotationSpeed = 0.0F, float scale = 1.0F, SDL_RendererFlip flip = SDL_FLIP_NONE);
	Sprite() {}
};
#endif