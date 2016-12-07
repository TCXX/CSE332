/*
Game.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Game.h" 
#include "FiveCardDraw.h"
#include "SevenCardStud.h"

using namespace std;

//allocate storage outside the class for the static member
shared_ptr<Game> Game::gamePtr;

//No destructor is needed because of shared_ptr

//A public static instance method that returns a copy of the static pointer member variable.
shared_ptr<Game> Game::instance() {
	if (gamePtr == nullptr) {
		throw INS_NOT_AVAIL;
	}
	else {
		return gamePtr; //passed by value
	}
}

//Returns the number of players.
size_t Game::size() {
	return players.size();
}

//A public static start_game method that dynamically allocates an instance of the FiveCardDraw class.
//The string parameter should contain "FiveCardDraw"
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
	else {
		throw UNKNOWN_GAME;
	}
}

//A public static stop_game method that calls delete on the static pointer and reset the static pointer.
void Game::stopGame() {

	if (gamePtr != nullptr) {
		gamePtr.reset();
	}
	else {
		throw NO_GAME_IN_PROCESS;
	}

}

//A public add_player method that dynamically allocates a Player and then push a pointer.
//The player's name cannot be duplicate.
void Game::addPlayer(const string &name) {
	string namePrefix = name;
	bool isAuto = false;
	if (namePrefix.back() == '*') {
		namePrefix = namePrefix.substr(0, namePrefix.size() - 1);
		isAuto = true;
	}
	auto ifHasPlayer = findPlayer(namePrefix);
	if (ifHasPlayer) {
		throw ALREADY_PLAYING;
	}
	else {
		players.push_back(make_shared<Player>(namePrefix, isAuto));
	}
	if (players[players.size()-1]->chip == 0) { //the newly added player has no chips
		string ans;
		cout << "Player " << namePrefix << " has no chips. Reset? (yes/no) " << endl;
		do {
			getline(cin, ans);
			transform(ans.begin(), ans.end(), ans.begin(), ::tolower);
		} while (ans!="yes" && ans !="no");
		if (ans == "no") {
			players.erase(players.begin() + players.size() - 1);
		}
		else {
			players[players.size() - 1]->reset();
		}
	}
}

//A public find_player method that returns either a shared_ptr to the matching Player or a singular shared_ptr.
shared_ptr<Player> Game::findPlayer(const string name) {
	for (vector<shared_ptr<Player>>::iterator p = players.begin(); p != players.end(); p++) {
		if ((*p)->name == name) {
			return *p;
		}
	}
	return nullptr;
}
