#include "War.h"

int main(int argc, char* args[]) {
	War* game = new War();
	game->Run();
	delete game;

	return 0;
}