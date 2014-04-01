#include "Deck.h"

Deck::Deck(string xmlFilePath, ContentManager* content) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile((string("Content/") + xmlFilePath).c_str());
	tinyxml2::XMLElement* root = doc.RootElement();

	nextCard = 0;
	for (tinyxml2::XMLElement* child = root->FirstChildElement(); child != NULL; child = child->NextSiblingElement()) {
		string img = child->Attribute("img");
		string snd = child->Attribute("snd");

		Card* card = new Card(content->LoadTexture("Cards/" + img), content->LoadSound(snd));

		card->name = child->Attribute("name");
		card->type = strcmp(child->Attribute("type"), "Army") == 0 ? CardType::Army : CardType::Tactics;
		card->ranks = atoi(child->Attribute("ranks"));
		card->health = card->maxHealth = atoi(child->Attribute("health"));
		card->attack = atoi(child->Attribute("attack"));
		card->defense = atoi(child->Attribute("defense"));
		card->cost = atoi(child->Attribute("cost"));
		card->tactics = (unsigned short)strtol(child->Attribute("tactics"), NULL, 16);

		cards[nextCard++] = card;
	}
	
	Shuffle();
}

void Deck::DrawToBottom() {
	Card* topCard = cards[nextCard - 1]; // Pointer to card being moved to bottom
	Card* movingCard = cards[0]; // Pointer to be used for the card being moved up

	for (int i = 1; i < nextCard; i++) {
		Card* tempCard = cards[i]; // Temporary pointer for next card to be moved
		cards[i] = movingCard; // Set card to the moving card (the card before it)
		movingCard = tempCard; // Set moving card to the next card to be moved
	}

	cards[0] = topCard; // Set cards[0] to the earlier defined topCard
}

void Deck::Shuffle(bool reset) {
	int shuffleLimit = reset ? DECK_SIZE : nextCard;

	int* randNums = new int[shuffleLimit]; // Create an array of random numbers to be used for sorting, equal to the number of undrawn cards, or 30 if resetting deck
	for (int i = 0; i < shuffleLimit; i++)
		randNums[i] = rand() % 100;

	for (int i = 0; i < shuffleLimit; i++) {
		for (int j = 0; j < shuffleLimit - 1; j++) {
			if (randNums[j] > randNums[j + 1]) { // Bubble sort the cards by their corresponding random number
				int temp = randNums[j];
				randNums[j] = randNums[j + 1];
				randNums[j + 1] = temp;

				Card* tempCard = cards[j];
				cards[j] = cards[j + 1];
				cards[j + 1] = tempCard;
			}
		}
	}

	if (reset) nextCard = DECK_SIZE;
	delete[] randNums;
}

Deck::~Deck() {
	for (int i = 0; i < DECK_SIZE; i++)
		delete cards[i];
}