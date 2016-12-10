/*
SevenCardStud.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "SevenCardStud.h"
#include "stdlib.h"

using namespace std;

//Constructor
SevenCardStud::SevenCardStud() : PokerGame() {
	MAX_CARDS_IN_HAND = 7;
}

//Do nothing other than paying ante.
int SevenCardStud::before_round() {
	PokerGame::before_round();
	return 0;
}

//Iterate through the players to take a turn.
int SevenCardStud::round() {
	size_t len = players.size();

	//specify number of cards to deal for each type
	vector<int> faceUp = { 1, 1, 1, 1, 0 };
	vector<int> faceDown = { 2, 0, 0, 0, 1 };

	
	for (int turn = 1; turn <= 5; turn++) {
		//deal seven cards to each player
		int up = faceUp[turn - 1];
		int down = faceDown[turn - 1];
		for (int i = 0; i < down; i++) dealAround(OWNER_CAN_SEE);
		for (int i = 0; i < up; i++) dealAround(SEEN_BY_ALL);

		//bet
		bet_in_turn();

		printPlayers(OTHER);

		if (countActive() < 2) break;
	}

	//after turn
	cout << endl;
	printPlayers(ADMIN);

	return 0;
}
