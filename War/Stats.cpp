#include "Stats.h"

Stats::Stats(std::string xmlFilePath) {
	path = "Content/" + xmlFilePath;

	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());
	tinyxml2::XMLElement* root = doc.RootElement();

	wins = atoi(root->FirstChildElement("wins")->GetText());
	losses = atoi(root->FirstChildElement("losses")->GetText());
	ratio = (float)wins / (float)losses;
}

void Stats::EndGame(bool win) { 
	if (win)
		wins++;
	else
		losses++;

	Save();
}

void Stats::Save() {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(path.c_str());
	tinyxml2::XMLElement* root = doc.RootElement();

	root->FirstChildElement("wins")->SetText(std::to_string(wins).c_str());
	root->FirstChildElement("losses")->SetText(std::to_string(losses).c_str());

	doc.SaveFile(path.c_str());
}