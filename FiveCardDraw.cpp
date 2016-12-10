/*
FiveCardDraw.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "FiveCardDraw.h"
#include "stdlib.h"

using namespace std;

//Constructor 
FiveCardDraw::FiveCardDraw() : PokerGame() {
	MAX_CARDS_IN_HAND = 5;
}

//Deal five private cards to every player.
int FiveCardDraw::before_round(){
	PokerGame::before_round();

	for (size_t i = 0; i < MAX_CARDS_IN_HAND; i++) {
		dealAround(OWNER_CAN_SEE);
	}

	return 0;
}

//Iterate through the players calling their turn method and then their after_turn method.
int FiveCardDraw::round() {
	size_t len = players.size();

	//first betting phase
	bet_in_turn();

	if (countActive() > 1) {
		//before_turn
		for (size_t i = 0; i <len; i++) discardCards(*players[i]);

		//turn
		for (size_t i = 0; i < len; i++) dealUntilFull(*players[i], OWNER_CAN_SEE);

		//second betting phase
		bet_in_turn();
	}

	//after turn
	printPlayers(ADMIN);

	return 0;
}