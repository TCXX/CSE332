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



	int bet_in_turn(); 
	int discardCards(Player& p);
	int dealCard(Player& p, Visibility vis);
	int dealUntilFull(Player& p, Visibility vis);
	int dealAround(Visibility vis);
	int dealSameToAll(Visibility vis);

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

	virtual int before_round(); //pay ante + sth. different from subclass
	virtual int round() = 0; //sth. different for every subclass
	virtual int after_round(); //same for every subclass
};

#endif  //PokerGame_H_