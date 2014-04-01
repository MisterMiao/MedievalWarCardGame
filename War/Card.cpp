#include "Card.h"

const float Card::CARD_SCALE = 0.4166F; // Downscale from 300:450 -> 125:187.5
Texture2D Card::backImage;
Font Card::font;

Card::Card(Texture2D texture, Sound _sound) : Sprite(texture, Vector2::Zero, Vector2::Zero, 0.0F, CARD_SCALE) {
	type = (CardType)-1;
	name = "";
	ranks = 0;
	maxHealth = health = 0;
	attack = 0;
	defense = 0;
	cost = 0;
	tactics = 0x0;
	sound = _sound;

	faceDown = false; 
	isMoving = false;
	discarded = false;
	used = false;
	destPos = Vector2::Zero;
}

void Card::SetMovement(const Vector2& destination, bool reveal) {
	destPos = destination;
	isMoving = true;

	if (reveal) { // Mainly kept false for enemy drawing cards from deck
		faceDown = false;
		flip = SDL_FLIP_NONE;
	}
}

void Card::Update(const float _deltaTime) {
	// Moving the card around the board
	if (isMoving) {
		position = position.Lerp(destPos, _deltaTime);
		position.X = ceilf(position.X * 100) / 100; // Round up to 2 decimal places
		position.Y = ceilf(position.Y * 100) / 100;
		if ((int)position.X == (int)destPos.X && (int)position.Y == (int)destPos.Y)
			isMoving = false;
	}

	// Hovering over a face-up card zooms in on it
	if (CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && !faceDown && !isMoving && !discarded && !used)
		scale = 0.5F;
	else
		scale = CARD_SCALE;
}

void Card::Draw(ContentManager* content) {
	Vector2 drawPosition = position;
	if (scale != CARD_SCALE) {
		drawPosition.X -= ((bounds.Width * scale) - (bounds.Width * CARD_SCALE)) / 2; // Center X position
		drawPosition.Y -= 25; // Draw card above actual position when scaled up
	}

	// Draw the card itself
	content->DrawTexture(!faceDown ? image : backImage, &drawPosition, &bounds, 0, scale, NULL, flip);

	SDL_Color cardTextColor = CreateColor(255, 0, 0);
	// Draw the card's attributes ontop of the card
	if (!faceDown) content->DrawString(to_string(cost), &(drawPosition + Vector2(260, 400) * scale), font, cardTextColor); // Draw the card's energy cost
	if (!faceDown && type == Army) {
		content->DrawString(to_string(ranks), &(drawPosition + Vector2(22, 52.5F) * scale), font, cardTextColor); // Draw the card's ranks
		content->DrawString(to_string(defense), &(drawPosition + Vector2(22, 100) * scale), font, cardTextColor); // Draw the card's defense
		content->DrawString(to_string(attack), &(drawPosition + Vector2(22, 157.5F) * scale), font, cardTextColor); // Draw the card's attack
		content->DrawString(to_string(health), &(drawPosition + Vector2(22, 217.5F) * scale), font, cardTextColor); // Draw the card's health
	}
}