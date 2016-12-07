/*
lab.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
#ifndef LAB_H_
#define LAB_H_

//#include "stdafx.h"  
//#include "Game.h"
//#include "FiveCardDraw.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;

enum ProgramError {
	SUCCESS,
	UNKNOWN_ERR,
	TOO_FEW_ARG, TOO_MANY_ARG, //cmd args
	HAND_OVERFLOW, EMPTY_HAND, HAND_OUT_OF_RANGE, HAND_NOT_COMPLETE, TOO_MANY_CARDS, SAME_CARD, //hand
	INS_NOT_AVAIL, GAME_ALREADY_STARTED, UNKNOWN_GAME, NO_GAME_IN_PROCESS, //game
	ALREADY_PLAYING, NO_ACTIVES, NO_WINNERS, NO_PLAYERS, //player
	NO_CARD_TO_DEAL //deck
};

const vector<string> errorMsg {
	"Successful run. ",
	"An unknown error has occurred. ",
	//cmd args
	"No Sufficient command line arguments are given. ",
	"Too many command line arguments are given. ",
	//hand
	"More than five cards are in the hand. ",
	"The deck has nothing to pop up. ",
	"Hand's index is out of range. ",
	"The hand contains less than five cards. ",
	"The function only applies to five cards. ",
	"The hand contains two cards of same suit.  ",
	//game
	"Instance is not available! ",
	"Game is already started! ",
	"Unknown game! ",
	"No game is currently in process! ",
	//player
	"The player is already playing! ",
	"No player is unfolded. ",
	"No player wins this round. ",
	"No player is currently in this round. ",
	//deck
	"The decks have no cards to deal. "
};

//main
int usageMsg(char* filename, int errorCode);

#endif //LAB_H_