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
	MAX_CARDS_IN_HAND = 5;
}

int FiveCardDraw::before_round(){
	PokerGame::before_round();

	int len = players.size();

	//players draw cards
	for (int i = 0; i < MAX_CARDS_IN_HAND; i++) {
		for (int j = 0; j < len; j++) {
			int index = (dealer + j + 1) % len;
			if (players[index]->hand.size() < MAX_CARDS_IN_HAND) players[index]->hand << deck;
		}
	}

	return 0;
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

//A public virtual before_turn method that asks the user cards to discard and then move them to discarDeck.
int FiveCardDraw::before_turn(Player& p) {
	discardCards(p);
	return 0;
}

//A public virtual turn method that deals as many as that they have discarded.
int FiveCardDraw::turn(Player& p) {
	dealUntilFull(p);
	return 0;
}

//A public virtual after_turn method that prints out the player's name and the contents of their hand.
int FiveCardDraw::after_turn(Player& p) {
	cout << p << endl;
	return 0;
}