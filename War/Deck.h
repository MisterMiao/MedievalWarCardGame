#ifndef DECK_H
#define DECK_H
#include "tinyxml2.h"
#include "Card.h"

class Deck {
public:
	// The maximum amount of cards in the deck (30)
	static const int DECK_SIZE = 30;

	// The remaining cards in the deck (access to nextCard)
	int GetRemainingCards() { return nextCard; }
	// The amount of cards that have been discarded
	int GetDiscardedCards() { return DECK_SIZE - nextCard; }
	// Get the top card from the deck and decrement nextCard
	Card* DrawToHand() { return cards[--nextCard]; }
	// Sort the top card to be the bottom card
	void DrawToBottom();
	// Decrement nextCard to ignore the top card
	void DrawToDiscard() { --nextCard; }
	// Sort the deck based on randomly generated numbers
	// _reset determines whether to shuffle the discard pile back into the deck
	void Shuffle(bool reset = false);

	Deck(string, ContentManager*);
	~Deck();
private:
	// Array of cards (pointers to them) in the deck
	Card* cards[DECK_SIZE];
	// Index of card to be drawn next
	int nextCard;
};
#endif