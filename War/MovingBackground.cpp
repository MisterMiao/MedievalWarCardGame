#include "MovingBackground.h"

MovingBackground::BackgroundLayer::BackgroundLayer() {
	position = Vector2::Zero;
	imageSize = Vector2::Zero;
	moveRate = 0.0F;
	rotationRate = 0.0F;
	rotation = 0.0F;
}

MovingBackground::MovingBackground(const Vector2& dimensions) {
	moving = false;
	moveHorizontally = true;
	windowSize = dimensions;
}

MovingBackground::~MovingBackground() {
	for (unsigned int i = 0; i < layerList.size(); i++)
		delete layerList[i];
}

void MovingBackground::AddLayer(Texture2D picture, float moveRate, float rotationRate) {
	BackgroundLayer* layer = new BackgroundLayer();
	layer->image = picture;
	layer->moveRate = moveRate;
	layer->rotationRate = rotationRate;

	int w = 0, h = 0;
	SDL_QueryTexture(picture->second, NULL, NULL, &w, &h);
	layer->imageSize = Vector2((float)w, (float)h);

	layerList.push_back(layer);
}

void MovingBackground::Update(const float deltaTime) {
	for (auto layer : layerList) {
		float moveDistance = layer->moveRate * deltaTime;

		if (moving) {
			if (layer->moveRate != 0.0F) {
				if (moveHorizontally) { // Change X if moving horizontally
					layer->position.X += moveDistance;
					// Reset to 0 once the whole image has been drawn witg modulo
					layer->position.X = (float)fmod(layer->position.X, layer->imageSize.X);
				} else { // Otherwise change Y
					layer->position.Y += moveDistance;
					layer->position.Y = (float)fmod(layer->position.Y, layer->imageSize.Y);
				}
			}

			if (layer->rotationRate != 0.0F) {
				layer->rotation += layer->rotationRate * deltaTime;
				layer->rotation = (float)fmod(layer->rotation, M_PI * 360.0F);
			}
		}
	}
}

void MovingBackground::Draw(ContentManager* content) {
	for (unsigned int i = 0; i < layerList.size(); i++) {
		if (moveHorizontally) { // Moving left or right
			content->DrawTexture(layerList[i]->image, &Vector2(layerList[i]->position.X, 0.0F), NULL, layerList[i]->rotation);

			// Draw again when image is moving offscreen
			if (layerList[i]->position.X > 0.0F) // Left > Right
				content->DrawTexture(layerList[i]->image, &Vector2(layerList[i]->position.X - layerList[i]->imageSize.X, 0.0F), NULL, layerList[i]->rotation);
			else { // Right > Left
				if (abs(layerList[i]->position.X) > layerList[i]->imageSize.X - windowSize.X)
					content->DrawTexture(layerList[i]->image, &Vector2(layerList[i]->position.X - layerList[i]->imageSize.X, 0.0F), NULL, layerList[i]->rotation);
			}
		} else { // Moving up or down
			content->DrawTexture(layerList[i]->image, &Vector2(0.0F, layerList[i]->position.Y), NULL, layerList[i]->rotation);

			if (layerList[i]->position.Y > 0.0F) // Top > Bottom
				content->DrawTexture(layerList[i]->image, &Vector2(0.0F, layerList[i]->position.Y - layerList[i]->imageSize.Y), NULL, layerList[i]->rotation);
			else { // Bottom > Top
				if (abs(layerList[i]->position.Y) > layerList[i]->imageSize.Y - windowSize.Y)
					content->DrawTexture(layerList[i]->image, &Vector2(0.0F, layerList[i]->position.Y + layerList[i]->imageSize.Y), NULL, layerList[i]->rotation);
			}
		}
	}
}