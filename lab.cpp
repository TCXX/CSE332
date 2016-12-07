/*
lab.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "lab.h"
#include "Game.h"
#include "FiveCardDraw.h"

using namespace std;

//To print usage message.
int usageMsg(const char* filename, int errorCode) {
	cout << "Program's name: " << filename << endl;
	cout << "Error " << errorCode <<": "<<errorMsg[errorCode]<< endl;
	return errorCode;
}

//Main
int main(int argc, char* argv[])
{
	const int MIN_CMD_ARG = 3;
	const char* filename = argv[0];

	try {
		if (argc < MIN_CMD_ARG) throw TOO_FEW_ARG;
		
		Game::startGame("SevenCardStud");
		shared_ptr<Game> game = Game::instance();

		string playerstemp;
		string checktemp;

		//add players
		for (int i = 1; i < argc; i++) {
			checktemp = argv[i];
			transform(checktemp.begin(), checktemp.end(), checktemp.begin(),::tolower);
			// player cannot be named 'no' 
			if (checktemp=="no" || checktemp=="no*") {
				cout << "Player with name "<< checktemp <<" is not allowed!" << endl; 
			}
			else {
				playerstemp = argv[i];
				game->addPlayer(playerstemp);
			}
		}

		if (game->size() < 2) throw TOO_FEW_ARG; // the number of valid player names less than two

		//play the game
		while (game->size()>=2) {
			game->before_round();
			game->round();
			game->after_round();
			if (game->size()<2) {
				Game::stopGame();
				cout << "Game ends with " <<game->size() << " players. " << endl;
				return 0;
			} else {
				cout << "Game continues with " << game->size() << " players. " << endl;
				cout << endl;
			}
		}
	}
	catch (ProgramError errorCode) {
		return usageMsg(filename, errorCode);
	}
	catch (...) {
		return usageMsg(filename, UNKNOWN_ERR);
	}

	return 0;
}


