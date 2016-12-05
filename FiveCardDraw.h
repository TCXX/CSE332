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

class FiveCardDraw : public PokerGame {

	public:
		FiveCardDraw();
		virtual int round();
};

#endif  //FiveCardDraw_H_
