/*
TexasHoldEm.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
#ifndef TexasHoldEm_H_
#define TexasHoldEm_H_


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

class TexasHoldEm : public PokerGame {
	virtual int before_round();
	virtual int round();

public:
	TexasHoldEm();

};

#endif  //TexasHoldEm_H_