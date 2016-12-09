/*
Card.cpp created by Cindy Le (lex@wustl.edu)
*/

#include "stdafx.h"
#include "Card.h"

using namespace std;

//struct



bool Card::operator<(const Card& c) const {
	//compares for by rank then suit
	return rank * 4 + suit < c.rank * 4 + c.suit;
}

bool Card::operator==(const Card& c) const {
	//compares for by rank then suit
	return rank * 4 + suit == c.rank * 4 + c.suit;
}

string Card::toString(AccessMode mode) const {
	if (!isVisible(mode)) return "** ";
	string str = rankName[rank - 1] + suitName[suit - 1];
	return str;
}

bool Card::isVisible(AccessMode mode) const {
	int intMode = (int)mode;
	int intVis = (int)visible;
	if (intVis > intMode) return false;
	return true;
}

