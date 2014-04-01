#include "Player.h"
#include "War.h"
#include <iostream>
using namespace std;

Player* Player::players[2];
int Player::playerIndex = 0;
Font Player::font;
Timer Player::turnTimer;
string Player::message = "Waiting...";

Vector2 Player::positions[2][14] = { // Positions for various places on the board. Dimension 0 is player, dimension 1 is the positions. Positions can be found with the Positions enum.
	{ Vector2(1085, 670), Vector2(70, 670), Vector2(640, 760), Vector2(515, 760), Vector2(765, 760), Vector2(390, 760), Vector2(890, 760), Vector2(265, 760), Vector2(645, 525), Vector2(505, 525), Vector2(785, 525), Vector2(365, 525), Vector2(925, 525), Vector2(225, 525) }, 
	{ Vector2(70, 105), Vector2(1085, 105), Vector2(640, 10), Vector2(515, 10), Vector2(765, 10), Vector2(390, 10), Vector2(890, 10), Vector2(265, 10), Vector2(645, 245), Vector2(505, 245), Vector2(785, 245), Vector2(365, 245), Vector2(925, 245), Vector2(225, 245) }
};

Player::Player(ContentManager* content) {
	playerNum = playerIndex; // Set player num to the indexer. This results in 0 = player, 1 = AI
	players[playerNum] = this; // Add the player to the array of players

	// Initialize the player entity card
	entityCard = new PlayerEntity(positions[playerNum][PDeck]);

	// Load the deck
	deck = new Deck("Deck.xml", content);

	for (int i = 0; i < CARD_LIMIT; i++) // Initialize the hand array
		hand[i] = nullptr;
	for (int i = 0; i < CARD_LIMIT; i++) // Initialize the board array
		board[i] = nullptr;
	for (int i = 0; i < 2; i++)          // Initialize the discard array
		discard[i] = nullptr;

	selected = -1;
	health = MAX_HEALTH;
	energy = MAX_ENERGY;

	for (int i = 0; i < CARD_LIMIT; i++)
		DrawCard();

	playerIndex++; // Increment the indexer for the next player
}

void Player::Start() {
	Player* thePlayer = players[Human];
	Player* theComputer = players[Computer];

	// Determine who has first turn
	thePlayer->turnState = rand() % 2;
	theComputer->turnState = thePlayer->turnState == WaitPhase ? DrawPhase : WaitPhase;

	if (thePlayer->turnState != WaitPhase)
		message = "Your Turn!";
	else
		message = "Enemy Turn!";

	// For testing purposes when no AI is present
	//for (int i = 0; i < CARD_LIMIT; i++)
	//	if (theComputer->hand[i]->type == CardType::Army)
	//		theComputer->PlayCard(i);
}

void Player::Turn() {
	switch (turnState) {
	case WaitPhase: // Wait for the enemy to be done their turn
		if (GetOpponent()->turnState == EndPhase) {
			turnTimer.Stop();
			if (BoardHasCards()) ResetBoard();
			GetOpponent()->turnState = WaitPhase;
			turnState++;
		}
		break;
	case DrawPhase: // Draw a card
		message = playerNum == Human ? "Your Turn!" : "Enemy Turn!";
		DrawCard();
		energy = MAX_ENERGY; // Reset energy
		turnTimer.Start(TURN_TIME); // Start a countdown timer for the turn
		turnState++;
		break;
	case SelectPhase: // Select cards on your board or in your hand
		if (playerNum == Human) {
			// Playing cards from the hand
			// Army: Puts card on the board
			// Tactics: Begins target phase to use on another card
			for (int i = 0; i < CARD_LIMIT; i++) {
				Card* card = GetCardInHand(i);
				if (card)
					if (card->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT) && !card->isMoving) {
						if (PlayCard(i) == 1) // If PlayCard returns 1, it is a tactics card that was played
							turnState = TacticsPhase;
						break; // Break to avoid clicking on overlaying cards
					}
			}

			// Select an Army card already on your board
			// Enters target phase to select another card to attack
			for (int i = 0; i < CARD_LIMIT; i++) {
				Card* card = GetCardOnBoard(i);
				if (card)
					if (card->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT) && !card->isMoving && !card->used) {
						selected = i;
						card->SetMovement(positions[playerNum][PBoard0 + selected] + Vector2(0, playerNum == Human ? -50.0F : 50.0F), false);
						turnState = AttackPhase;
						break;
					}
			}

			// End your turn early by clicking on your deck
			if (entityCard->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT))
				turnState = EndPhase;
		} else {
			if (HandHasCards() || BoardHasCards()) {
				bool choice = rand() % 2 == 0; // What the computer will do, true = play a card, false = use a card
				bool canSelectFromBoard = true;

				if (!BoardHasCards()) // If the board has no cards, do not allow selection from the board
					choice = true;
				else { // Check if the cards on the board can be used
					for (int i = 0; i < CARD_LIMIT; i++) {
						if (board[i])
							if (!board[i]->used) { // Break the loop if an unused card is on the board
								canSelectFromBoard = false;
								break;
							}
					}
				}

				// Plays a card from the hand
				if (choice) {
					Card* card = nullptr;
					int i = -1;
					do {
						card = GetCardInHand(i = rand() % CARD_LIMIT);
					} while (!card);

					if (!(card->type == CardType::Tactics && !GetOpponent()->BoardHasCards()))
						if (PlayCard(i) == 1)
							turnState = TacticsPhase;
				} else { // Uses a card on the board
					if (canSelectFromBoard) {
						Card* card = nullptr;
						int i = -1;
						do { // Loop until a card is found on the board
							card = GetCardOnBoard(i = rand() % CARD_LIMIT);

							if (card) // Loop until an unused card is found
								if (!card->used)
									break;
						} while (true);

						selected = i;
						card->SetMovement(positions[playerNum][PBoard0 + selected] + Vector2(0, playerNum == Human ? -50.0F : 50.0F), false);
						turnState = AttackPhase;
					}
				}
			}
		}
		break;
	case AttackPhase: // Select a card to attack with your army card
		if (playerNum == Human) {
			// Exit attack phase without attacking by right clicking the selected card on your board
			if (GetCardOnBoard(selected)->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_RIGHT)) {
				GetCardOnBoard(selected)->SetMovement(positions[playerNum][PBoard0 + selected], false);
				selected = -1;
				turnState = SelectPhase;
			}

			// Attack a card on the enemy board
			for (int i = 0; i < CARD_LIMIT; i++) {
				Card* card = GetOpponent()->GetCardOnBoard(i);
				if (card)
					if (card->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT) && !card->isMoving) {
						UseCard(selected, i, 0);
						GetCardOnBoard(selected)->SetMovement(positions[playerNum][PBoard0 + selected] + Vector2(0, playerNum == Human ? 50.0F : -50.0F), false);
						selected = -1;
						turnState = SelectPhase;
						break;
					}
			}

			// Attack the enemy's player card
			if (GetOpponent()->entityCard->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT)) {
				AttackOpponent(selected);
				GetCardOnBoard(selected)->SetMovement(positions[playerNum][PBoard0 + selected] + Vector2(0, playerNum == Human ? 50.0F : -50.0F), false);
				selected = -1;
				turnState = SelectPhase;
			}
		} else {
			bool choice = !GetOpponent()->BoardHasCards(); // Cannot attack the enemy if the enemy board has cards

			// Attack the player
			if (choice) {
				AttackOpponent(selected);
				GetCardOnBoard(selected)->SetMovement(positions[playerNum][PBoard0 + selected] + Vector2(0, playerNum == Human ? 50.0F : -50.0F), false);
				selected = -1;
				turnState = SelectPhase;
			} else { // Attack a card on the enemy's board
				Card* card = nullptr;
				int i = -1;
				do {
					card = GetOpponent()->GetCardOnBoard(i = rand() % CARD_LIMIT);
				} while (!card);

				UseCard(selected, i, 0);
				GetCardOnBoard(selected)->SetMovement(positions[playerNum][PBoard0 + selected] + Vector2(0, playerNum == Human ? 50.0F : -50.0F), false);
				selected = -1;
				turnState = SelectPhase;
			}
		}
		break;
	case TacticsPhase: // Select a card to use your tactics card on
		if (playerNum == Human) {
			// Exit tactics phase without using the card by right clicking the selected card in your hand
			if (GetCardInHand(selected)->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_RIGHT)) {
				GetCardInHand(selected)->SetMovement(positions[playerNum][PHand0 + selected], false);
				energy += GetCardInHand(selected)->cost; // Refund energy cost when cancelled
				selected = -1;
				turnState = SelectPhase;
			}

			// Use on a card on the enemy's board
			for (int i = 0; i < CARD_LIMIT; i++) {
				Card* card = GetOpponent()->GetCardOnBoard(i);
				if (card)
					if (card->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT) && !card->isMoving) {
						UseCard(selected, i, 1);
						selected = -1;
						turnState = SelectPhase;
						break;
					}
			}

			// Use on a card on your board
			for (int i = 0; i < CARD_LIMIT; i++) {
				Card* card = GetCardOnBoard(i);
				if (card)
					if (card->CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()) && InputManager::GetInstance()->MouseButtonPress(SDL_BUTTON_LEFT) && !card->isMoving) {
						UseCard(selected, i, 2);
						selected = -1;
						turnState = SelectPhase;
						break;
					}
			}
		} else {
			unsigned short tactics = GetCardInHand(selected)->tactics;
			// Whether or not the card has offensive effects, to figure out where to use the card
			bool offensive = tactics & Tactics::LowerRanks || tactics & Tactics::LowerHealth || tactics & Tactics::LowerAttack || tactics & Tactics::LowerDefense || tactics & Tactics::MoveToDeck || tactics & Tactics::MoveToTrash;

			Card* card = nullptr;
			int i = -1;
			do {
				i = rand() % CARD_LIMIT;
				if (!offensive)
					card = GetCardOnBoard(i);
				else
					card = GetOpponent()->GetCardOnBoard(i);
			} while (!card);

			UseCard(selected, i, offensive == true ? 1 : 2);
			selected = -1;
			turnState = SelectPhase;
		}
		break;
	case EndPhase:
		break;
	}

	if (turnState != WaitPhase) {
		if (playerNum == Computer) { // End the computer's turn when there's nothing left to do
			if (energy == 0 || !HandHasCards()) { // If additional cards cannot be placed
				if (GetOpponent()->BoardHasCards()) { // If the enemy board has cards to attack
					if (!BoardHasCards()) // The board has no cards to be used
						turnState = EndPhase;
					else { // Check if the cards on the board can be used to attack the other cards
						for (int i = 0; i < CARD_LIMIT; i++) {
							if (board[i])
								if (!board[i]->used) // Break the loop if an unused card is on the board
									break;

							if (i == CARD_LIMIT - 1) { // If on the last index of the loop and all cards are used
								cout << "End." << endl;
								turnState = EndPhase;
							}
						}
					}
				}
			}
		}

		if (turnTimer.Remaining() == -1)
			turnState = EndPhase; // End the turn when time runs out
	}
}

int Player::DrawCard() {
	int spot = FindFreeSpot(hand);
	if (spot < 0) 
		return -1;

	Card* card = deck->DrawToHand();
	card->position = positions[playerNum][PDeck];
	card->SetMovement(positions[playerNum][PHand0 + spot], false);
	hand[spot] = card;

	return 0;
}

int Player::PlayCard(int cardIndex) {
	Card* card = hand[cardIndex];

	if (energy - card->cost < 0)
		return -1; // Insufficient energy, return failure

	if (card->type == CardType::Army) {
		int spot = FindFreeSpot(board);
		if (spot < 0)
			return -1; // No space on board, return failure

		card->SetMovement(positions[playerNum][PBoard0 + spot]);
		board[spot] = card;
		hand[cardIndex] = nullptr;
	} else {
		if (!GetOpponent()->BoardHasCards() && !BoardHasCards())
			return -1; // Cannot use the card because the board has no cards

		selected = cardIndex;
		card->SetMovement(positions[playerNum][PHand0 + selected] + Vector2(0, playerNum == Human ? -50.0F : 50.0F), false);
	}

	energy -= card->cost;

	// Return success. 0 = Army card, 1 = Tactics card (to initiate target phase for tactics card)
	return card->type == CardType::Army ? 0 : 1;
}

void Player::UseCard(int card1Index, int card2Index, int typeOfUse) {
	if (card1Index < 0 || card2Index < 0)
		return;

	Card* card1 = nullptr;
	Card* card2 = nullptr;

	switch (typeOfUse) {
	case 0: // (Army) Ally Board -> (Army) Enemy Board
		card1 = GetCardOnBoard(card1Index);
		card2 = GetOpponent()->GetCardOnBoard(card2Index);
		break;
	case 1: // (Tactics) Ally Hand -> (Army) Enemy Board
		card1 = GetCardInHand(card1Index);
		card2 = GetOpponent()->GetCardOnBoard(card2Index);
		break;
	case 2: // (Tactics) Ally Hand -> (Army) Ally Board
		card1 = GetCardInHand(card1Index);
		card2 = GetCardOnBoard(card2Index);
		break;
	}

	card1->used = true;

	if (card1->type == CardType::Army)
		card2->health -= (int)ceil(card1->attack * (1.0F / card2->defense));
	else {
		if (card1->sound != NULL_SOUND)
			Mix_PlayChannel(-1, card1->sound->second, 0);

		unsigned short tactics = card1->tactics;

		if (typeOfUse != 2) {
			if (tactics & Tactics::MoveToTrash)
				GetOpponent()->Discard(GetOpponent()->board[card2Index]);

			if (tactics & Tactics::MoveToDeck) {
				// TODO: Move the card to the bottom of the deck
				//GetOpponent()->board[card2Index] = nullptr;
			}
		}

		// Boost / Lower attributes
		card2->ranks += tactics & Tactics::BoostRanks ? card1->ranks : tactics & Tactics::LowerRanks ? -card1->ranks : 0;
		card2->health += tactics & Tactics::BoostHealth ? card1->health : tactics & Tactics::LowerHealth ? -card1->health : 0;
		card2->attack += tactics & Tactics::BoostAttack ? card1->attack : tactics & Tactics::LowerAttack ? -card1->attack : 0;
		card2->defense += tactics & Tactics::BoostDefense ? card1->defense : tactics & Tactics::LowerDefense ? -card1->defense : 0;

		// Move the tactics card to the discard pile
		Discard(hand[card1Index]);
	}

	if (typeOfUse != 2)
		if (card2->health <= 0)
			if (card2->ranks > 1) { // Reset health and deplete a rank
				card2->ranks--;
				card2->health = card2->maxHealth;
			} else // Send to discard if no ranks remaining
				GetOpponent()->Discard(GetOpponent()->board[card2Index]);
}

void Player::AttackOpponent(int cardIndex) {
	Card* card = GetCardOnBoard(cardIndex);
	Player* enemy = GetOpponent();

	card->used = true;

	// Damage the enemy
	enemy->health -= (int)ceil(card->attack * 0.75F);

	// If the enemy's health is 0 or less, end the game
	// If the player is human at this point, count it as a win
	if (enemy->health <= 0)
		War::GetInstance()->EndGame(playerNum == Human);
}

void Player::Discard(Card*& cardPointer) {
	cardPointer->SetMovement(positions[playerNum][PDiscard]); // Move to the discard pile
	cardPointer->discarded = true;

	discard[0] = discard[1]; // 0 is bottom, 1 is top; move top to bottom
	discard[1] = cardPointer; // Put discarded card on top of discard pile

	cardPointer = nullptr; // Empty the spot in the playfield
}

void Player::ResetBoard() {
	for (int i = 0; i < CARD_LIMIT; i++) {
		if (board[i]) {
			board[i]->SetMovement(positions[playerNum][PBoard0 + i]);
			board[i]->used = false;
		}
	}

	selected = -1;
}

int Player::FindFreeSpot(Card** arr) {
	int spot = -1;
	for (int i = 0; i < CARD_LIMIT; i++) {
		if (arr[i] == nullptr) {
			spot = i;
			break;
		}
	}

	return spot;
}

bool Player::BoardHasCards() {
	for (int i = 0; i < CARD_LIMIT; i++) {
		if (board[i])
			return true;
	}

	return false;
}

bool Player::HandHasCards() {
	for (int i = 0; i < CARD_LIMIT; i++) {
		if (hand[i])
			return true;
	}

	return false;
}

void Player::Update(const float deltaTime) {
	entityCard->Update(deltaTime); // Update the player card entity

	if (health <= 0) // If health is 0, end the game. Returns true (win) if computer has 0 health.
		War::GetInstance()->EndGame(playerNum == Computer);

	if (turnState != EndPhase)
		Turn();

	// Update the hand
	for each (Card* card in hand) {
		if (card)
			card->Update(deltaTime);
	}

	// Update the board
	for each (Card* card in board) {
		if (card)
			card->Update(deltaTime);
	}

	// Update the discard pile
	for each (Card* card in discard) {
		if (card)
			card->Update(deltaTime);
	}
}

void Player::Draw(ContentManager* content) {
	// Draw the player's card entity, aka the deck, and the player's health/energy
	entityCard->Draw(content, health, energy);

	// Draw the board
	for each (Card* card in board) {
		if (card)
			card->Draw(content);
	}

	// Draw the hand
	for each (Card* card in hand) {
		if (card) {
			if (playerNum == Computer) { // Display opponent's hand as face down and flipped
				card->faceDown = true;
				card->flip = (SDL_RendererFlip)(SDL_FLIP_VERTICAL | SDL_FLIP_HORIZONTAL);
			}

			card->Draw(content);
		}
	}

	// Draw the discard pile
	for each (Card* card in discard)
		if (card)
			card->Draw(content);
}

Player* Player::GetOpponent() {
	return players[playerNum == Human ? Computer : Human]; // Returns the opposite player in the players array
}

Player::~Player() {
	delete entityCard;
	delete deck;
}

void PlayerEntity::Update(const float deltaTime) {
	Sprite::Update(deltaTime);

	if (CollisionRect().Contains(InputManager::GetInstance()->GetMousePos()))
		scale = 0.6F;
	else
		scale = Card::CARD_SCALE;
}

void PlayerEntity::Draw(ContentManager* content, int health, int energy) {
	Vector2 drawPosition = position;
	if (scale != Card::CARD_SCALE) {
		drawPosition.X -= ((bounds.Width * scale) - (bounds.Width * Card::CARD_SCALE)) / 2;
		drawPosition.Y -= ((bounds.Height * scale) - (bounds.Height * Card::CARD_SCALE)) / 2;
	}

	content->DrawTexture(image, &drawPosition, &bounds, 0, scale, NULL, SDL_FLIP_NONE);
	content->DrawString(to_string(health), &(drawPosition + Vector2(10, 5) * scale), Player::font, CreateColor(255, 0, 0)); // Draw the player's health
	content->DrawString(to_string(energy), &(drawPosition + Vector2(10, bounds.Height - 100) * scale), Player::font, CreateColor(255, 255, 0)); // Draw the player's energy
}