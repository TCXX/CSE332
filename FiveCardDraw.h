/*
FiveCardDraw.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
#ifndef FiveCardDraw_H_
#define FiveCardDraw_H_


#include "Card.h"
#include "Deck.h"
#include "Player.h"
#include "Hand.h"
#include "Game.h"
#include <iostream>
#include <sstream>
#include <vector> 
#include <string>

using namespace std;

class FiveCardDraw : public Game {
		int autoPlayerLeave();
		vector<int> findAuto();

	public:
		FiveCardDraw();
		virtual int before_turn(Player& p);
	    virtual int turn(Player& p);
		virtual int after_turn(Player& p);
		virtual int before_round();
		virtual int round();
		virtual int after_round();

		/*Never subtract chips directly!!! Use payChips instead to avoid negative values*/
		unsigned int payChips(Player& p, unsigned int amount);
		unsigned int betChips(Player& p);
		
	protected:
		size_t dealer;
		Deck discardDeck;
		unsigned int ante;
		unsigned int pot;
};

#endif  //FiveCardDraw_H_
