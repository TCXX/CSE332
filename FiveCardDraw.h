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
#include "PokerGame.h"
#include <iostream>
#include <sstream>
#include <vector> 
#include <string>

using namespace std;

//A game that has two betting phases, one before discarding cards and another after. 
class FiveCardDraw : public PokerGame {
	virtual int before_round();
	virtual int round();

public:
	FiveCardDraw();

};

#endif  //FiveCardDraw_H_
