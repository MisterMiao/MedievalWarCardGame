#include "Sprite.h"

Sprite::Sprite(Texture2D texture, Vector2 position, Vector2 velocity, float rotationSpeed, float scale, SDL_RendererFlip flip) {
	this->image = texture;
	this->position = position;
	this->initVelocity = this->velocity = velocity;
	this->scale = scale;
	this->rotation = 0;
	this->rotationSpeed = rotationSpeed;
	this->flip = flip;

	int w = 0, h = 0;
	SDL_QueryTexture(texture->second, NULL, NULL, &w, &h);
	bounds = Rectangle(0, 0, (float)w, (float)h);
}

Rectangle Sprite::CollisionRect() {
	return Rectangle(position.X, position.Y, bounds.Width * scale, bounds.Height * scale);
}

void Sprite::Update(const float deltaTime) {
	position += velocity * deltaTime;
	rotation += rotationSpeed * deltaTime;
	rotation = (float)fmod(rotation, M_PI * 360.0F);
}

void Sprite::Draw(ContentManager* content) {
	content->DrawTexture(image, &position, &bounds, rotation, scale, NULL, flip);
}