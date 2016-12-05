/*
PokerGame.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
#ifndef PokerGame_H_
#define PokerGame_H_


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

class PokerGame : public Game {

public:
	PokerGame();
	virtual int before_turn(Player& p);
	virtual int turn(Player& p);
	virtual int after_turn(Player& p);
	virtual int before_round();
	virtual int round() = 0;
	virtual int after_round();

	int bet_in_turn();

	/*Never subtract chips directly!!! Use payChips instead to avoid negative values*/
	unsigned int payChips(Player& p, unsigned int amount);
	unsigned int betChips(Player& p);

protected:
	size_t dealer;
	Deck discardDeck;
	unsigned int ante;
	unsigned int pot;
	unsigned int bet; // the cumulative amount players have bet during that phase
	int autoPlayerLeave();
	vector<int> findAuto();
	int countActive();
};

#endif  //PokerGame_H_