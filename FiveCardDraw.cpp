/*
FiveCardDraw.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/


#include "stdafx.h"
#include "FiveCardDraw.h"
#include "stdlib.h"

//#define cout std::cout //cout is not ambiguous

using namespace std;

//A default constructor for fiveCardDraw that initializes dealer to be the first person and discard to be empty. 
FiveCardDraw::FiveCardDraw() : PokerGame() {
	//initialize as in superclass
}

//A public virtual round method that iterates through the players calling their turn method and then their after_turn method.
int FiveCardDraw::round() {
	int len = players.size();

	//players decide whether to bet some chips or not - first phase
	bet_in_turn();

	//change some cards in hand
	if (countActive() > 1) {
		//players discard some of the five cards
		for (int i = 0; i <len; i++) before_turn(*players[i]);

		//deal cards until each player has five cards
		for (int i = 0; i < len; i++) turn(*players[i]);

		//players decide whether to bet some chips or not - second phase
		bet_in_turn();
	}

	//print player ranks before sorting
	cout << endl;
	for (int i = 0; i < len; i++) {
		after_turn(*players[i]);
	}

	return 0;
}