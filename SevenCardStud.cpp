/*
SevenCardStud.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/


#include "stdafx.h"
#include "SevenCardStud.h"
#include "stdlib.h"

//#define cout std::cout //cout is not ambiguous

using namespace std;

//A default constructor for fiveCardDraw that initializes dealer to be the first person and discard to be empty. 
SevenCardStud::SevenCardStud() : PokerGame() {
	MAX_CARDS_IN_HAND = 7;
}

int SevenCardStud::before_round() {
	PokerGame::before_round();
	//do nothing other than paying ante
	return 0;
}

//A public virtual round method that iterates through the players calling their turn method and then their after_turn method.
int SevenCardStud::round() {
	size_t len = players.size();

	//specify number of cards to deal for each type
	vector<int> faceUp = { 2, 1, 1, 1, 0 };
	vector<int> faceDown = { 1, 0, 0, 0, 1 };

	
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
