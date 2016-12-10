/*
Game.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Game.h" 
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include "TexasHoldEm.h"

using namespace std;

//Allocate storage outside the class for the static member.
shared_ptr<Game> Game::gamePtr;

//No destructor is needed because of shared_ptr.

//Return a copy of the static pointer member variable.
shared_ptr<Game> Game::instance() {
	if (gamePtr == nullptr) {
		throw INS_NOT_AVAIL;
	}
	else {
		return gamePtr;
	}
}

//Return the number of players.
size_t Game::size() const {
	return players.size();
}

//Return a list of all players in a vector.
vector<string> Game::getPlayers() {
	size_t len = players.size();
	vector<string> list = vector<string>();
	for (size_t i = 0; i < len; i++) {
		if (players[i]->isAuto) list.push_back(players[i]->name + "*");
		else list.push_back(players[i]->name);
	}
	return list;
}

//Add a given list of players to the game.
void Game::addPlayers(vector<string> list) {
	size_t len = list.size();
	for (size_t i = 0; i < len; i++) {
		const string name = list[i];
		addPlayer(name);
	}
}

//Dynamically allocates an game instance.
void Game::startGame(const string& sofgames) {
	if (gamePtr!= nullptr) {
		throw GAME_ALREADY_STARTED;
	}
	else if (sofgames.find("FiveCardDraw") != string::npos) {
		shared_ptr<FiveCardDraw> temp(new FiveCardDraw);
		gamePtr = temp;
	}
	else if (sofgames.find("SevenCardStud") != string::npos) {
		shared_ptr<SevenCardStud> temp(new SevenCardStud);
		gamePtr = temp;
	}
	else if (sofgames.find("TexasHoldEm") != string::npos) {
		shared_ptr<TexasHoldEm> temp(new TexasHoldEm);
		gamePtr = temp;
	}
	else {
		throw UNKNOWN_GAME;
	}
}

//Go through a round of the current game.
void Game::playGame() {
	if (gamePtr == nullptr) {
		throw NO_GAME_IN_PROCESS;
	}
	else if (gamePtr->size()<2) {
		throw NO_PLAYERS;
	}
	else {
		gamePtr->before_round();
		gamePtr->round();
		gamePtr->after_round();
	}
}

//Delete and reset the static pointer.
void Game::stopGame() {
	if (gamePtr != nullptr) {
		gamePtr.reset();
	}
	else {
		throw NO_GAME_IN_PROCESS;
	}

}

//Dynamically allocates a Player and then push a pointer.
void Game::addPlayer(const string &name) {
	string namePrefix = name;
	bool isAuto = false;
	if (namePrefix.back() == '*') {
		namePrefix = namePrefix.substr(0, namePrefix.size() - 1);
		isAuto = true;
	}
	//check for duplicates
	auto ifHasPlayer = findPlayer(namePrefix);
	if (ifHasPlayer) {
		throw ALREADY_PLAYING;
	}
	else {
		Player temp(namePrefix, isAuto);
		players.push_back(make_shared<Player>(temp));
	}
}

//Returns either a shared_ptr to the matching Player or a singular shared_ptr.
shared_ptr<Player> Game::findPlayer(const string name) {
	for (vector<shared_ptr<Player>>::iterator p = players.begin(); p != players.end(); p++) {
		if ((*p)->name == name) {
			return *p;
		}
	}
	return nullptr;
}

//Decide for autoplayers whether each of them leaves the game.
int Game::autoPlayerLeave() {
	vector<size_t> autoPlayers = findAuto();
	size_t numAuto = autoPlayers.size();
	size_t numPlayers = players.size();
	unsigned int leaveNum;

	//auto players leave with probablity
	for (size_t i = numAuto; i > 0; i--) {
		leaveNum = rand() % 100;
		if (autoPlayers[i - 1] == 0) { //the last place
			if (leaveNum < 90) {
				players.erase(players.begin() + autoPlayers[i - 1]);
			}
		}
		else if (autoPlayers[i - 1] == numPlayers) { //the first place
			if (leaveNum < 10) {
				players.erase(players.begin() + autoPlayers[i - 1]);
			}
		}
		else {
			if (leaveNum < 50) { //otherwise
				players.erase(players.begin() + autoPlayers[i - 1]);
			}
		}

	}

	return 0;
}

//Return a list of all auto players.
vector<size_t> Game::findAuto() {
	size_t len = players.size();
	vector<size_t> autoPlayers;
	for (size_t i = 0; i < len; i++) {
		char last = (*players[i]).name.back();
		if (last == '*') {
			autoPlayers.push_back(i);
		}
	}
	return autoPlayers;
}

//Count number of active players who have not folded.
size_t Game::countActive() {
	size_t numPlayers = players.size();
	size_t active = 0;
	for (size_t i = 0; i < numPlayers; i++) {
		if (players[i]->isFold == false) active++;
	}
	return active;
}

//Save all player data into corresponding txt files.
int Game::saveToFile() {
	size_t len = players.size();
	vector<size_t> autoPlayers = findAuto();
	ofstream output;
	for (size_t i = 0; i < len; i++) {
		string name = (*players[i]).name;
		//get rid of star
		if (find(autoPlayers.begin(), autoPlayers.end(), i) != autoPlayers.end()) {
			name = name.substr(0, name.size() - 1);
		}
		output.close();
		output.open(name + ".txt");
		output << (*players[i]).name << endl;
		output << (*players[i]).won << endl;
		output << (*players[i]).lost << endl;
		output << (*players[i]).chip << endl;
	}

	return 0;
}

