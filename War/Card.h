#ifndef CARD_H
#define CARD_H
#include "Sprite.h"

enum CardType {
	Army,    // Generic cards that can be played on board
	Tactics, // Spell cards that have special effects
	Entity   // Represents the players themselves
};

enum Tactics {
	BoostRanks   = 0x001, // 000000000001
	BoostHealth  = 0x002, // 000000000010
	BoostAttack  = 0x004, // 000000000100
	BoostDefense = 0x008, // 000000001000
	LowerRanks   = 0x010, // 000000010000
	LowerHealth  = 0x020, // 000000100000
	LowerAttack  = 0x040, // 000001000000
	LowerDefense = 0x080, // 000010000000
	MoveToDeck   = 0x100, // 000100000000
	MoveToTrash  = 0x200, // 001000000000
};

class Card : public Sprite {
public:
	static const float CARD_SCALE;
	static Texture2D backImage;
	static Font font;

	// Name of the card
	string name;
	// Type of the card
	CardType type;
	// Card's armor value
	int ranks;
	// Card's health value
	int health, maxHealth;
	// Card's uses
	int attack;
	// Card's attack strength
	int defense;
	// Card's energy cost
	int cost;
	// Card's tactics effects
	unsigned short tactics;
	// Card's sound effect
	Sound sound;

	// Whether or not the card is face up on the board
	bool faceDown;
	// Whether or not the card is moving on the board
	bool isMoving;
	// Whether or not the card has been discarded
	bool discarded;
	// Whether or not the card has been used (in a turn)
	bool used;
	// Where the card is moving
	Vector2 destPos;

	void SetMovement(const Vector2&, bool = true);

	void Update(const float);
	void Draw(ContentManager*);

	Card(Texture2D, Sound);
};
#endif