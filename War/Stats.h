#ifndef STATS_H
#define STATS_H
#include <string>
#include "tinyxml2.h"

class Stats {
public:
	void EndGame(bool); // Reports the win/loss to the stats object

	int GetWins() { return wins; }
	int GetLosses() { return losses; }
	float WLRatio() { return ratio; }

	Stats(std::string); // Loads the stats from the XML file
	Stats() {}
private:
	std::string path;
	int wins;
	int losses;
	float ratio;

	void Save(); // Saves the stats to the XML file
};
#endif