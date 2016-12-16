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

//A game that has four turns of dealing cards and four betting phases after each one of them.
class TexasHoldEm : public PokerGame {
	int before_round();
	int round();

public:
	TexasHoldEm();

};

#endif  //TexasHoldEm_H_