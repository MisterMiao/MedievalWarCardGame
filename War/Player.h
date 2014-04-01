#ifndef PLAYER_H
#define PLAYER_H
#include "Deck.h"

enum Players { Human, Computer };

class Player;
class PlayerEntity;

// Class for managing the game board, the players, and their input
class Player {
public:
	// The maximum number of cards that can be in a hand
	static const int CARD_LIMIT = 6;
	// The maximum amount of energy that can be used per turn
	static const int MAX_ENERGY = 5;
	// The maximum amount of health that a player can have
	static const int MAX_HEALTH = 50;
	// The amount of time in millis that a turn lasts (30 seconds)
	static const int TURN_TIME = 30000;
	// The font for the board
	static Font font;
	// Turn timer
	static Timer turnTimer;
	// Message to display in the middle of the screen
	static string message;

	// Get the player's deck
	Deck* GetDeck() { return deck; }
	// Get the player's hand
	Card** GetHand() { return hand; }
	// Get the player's board
	Card** GetBoard() { return board; }
	// Get a card from the player's hand
	Card* GetCardInHand(int index) { return hand[index]; }
	// Get a card from the player's board
	Card* GetCardOnBoard(int index) { return board[index]; }

	// Starts the game
	static void Start();

	void Update(const float);
	void Draw(ContentManager*);

	Player(ContentManager*);
	~Player();
private:
	enum Positions { PDeck, PDiscard, PHand0, PHand1, PHand2, PHand3, PHand4, PHand5, PBoard0, PBoard1, PBoard2, PBoard3, PBoard4, PBoard5 }; // Enum for positions on the board
	enum TurnState { WaitPhase, DrawPhase, SelectPhase, AttackPhase, TacticsPhase, EndPhase }; // Enum for turn phases

	// Both of the players
	static Player* players[2];
	// The positions on the board
	static Vector2 positions[2][14];
	// Indexer to manage playerNum
	static int playerIndex;

	// The card representing the player's attributes
	PlayerEntity* entityCard;
	// The player's deck of cards
	Deck* deck;
	// The player's hand
	Card* hand[CARD_LIMIT];
	// The player's board for playing cards
	Card* board[CARD_LIMIT];
	// The top of the discard pile (for drawing)
	Card* discard[2];
	// Index of the selected item
	int selected;

	// Which player this is
	int playerNum;
	// What the state of the turn is
	int turnState;
	// The player's hitpoints
	int health;
	// Energy in a turn
	int energy;

	// Handles the player's turn phases
	void Turn();
	// Draws a card if there is space in the hand
	int DrawCard();
	// Play a card from the hand by its index
	int PlayCard(int);
	// Use a card (in your hand or board) on another card (on your or the enemy's board)
	void UseCard(int, int, int);
	// Attack the other player
	void AttackOpponent(int);
	// Adjust the discard array; pass a spot on the board by reference to empty it
	void Discard(Card*&);

	// Reset's all cards on the board to their normal positions;
	void ResetBoard();
	// Finds the first free spot in the player's hand or board
	int FindFreeSpot(Card**);
	// Determines whether or not the board has any cards
	bool BoardHasCards();
	// Determines whetehr or not the hand has any cards
	bool HandHasCards();

	// Get the opposing player
	Player* GetOpponent();
};

// A card for the player, to allow directly attacking the player
class PlayerEntity : public Sprite {
public:
	void Update(const float);
	void Draw(ContentManager*, int, int);

	PlayerEntity::PlayerEntity(Vector2 position) : Sprite(Card::backImage, position, Vector2::Zero, 0.0F, Card::CARD_SCALE) {}
};
#endif