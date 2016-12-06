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

	virtual int before_turn(Player& p) = 0;
	virtual int turn(Player& p) = 0;
	virtual int after_turn(Player& p) = 0;

	virtual int before_round(); //pay ante + sth. different from subclass
	virtual int round() = 0; //sth. different for every subclass
	virtual int after_round(); //same for every subclass

	int bet_in_turn(); 
	int discardCards(Player& p);
	int dealUntilFull(Player& p);

	/*Never subtract chips directly!!! Use payChips instead to avoid negative values*/
	unsigned int payChips(Player& p, unsigned int amount);
	unsigned int betChips(Player& p);

protected:
    size_t MAX_CARDS_IN_HAND;

	size_t dealer;
	Deck discardDeck;
	unsigned int ante;
	unsigned int pot;
	unsigned int bet; // the cumulative amount players have bet during that phase

	int autoPlayerLeave();
	vector<size_t> findAuto();
	size_t countActive();
};

#endif  //PokerGame_H_