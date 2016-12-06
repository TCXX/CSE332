/*
Game.h created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#pragma once
#ifndef GAME_H_
#define GAME_H_
#include<iostream>
#include<fstream>
#include<sstream>
#include <string>
#include <mutex>
#include "Player.h"
#include "Deck.h"
#include "Hand.h"
#include "lab.h"

using namespace std;
class Deck;
struct Player;

class FiveCardDraw;

class Game {

	protected:
		static shared_ptr<Game> gamePtr;
		Deck deck;
		vector<shared_ptr<Player>> players;
	public:
		static shared_ptr<Game> instance();
		size_t size(); //defined for convinience
		static void startGame(const string& sofgames);
		static void stopGame();
		void addPlayer(const string &name);
		shared_ptr<Player> findPlayer(const string name);

		virtual int before_round() = 0;
		virtual int round() = 0;
		virtual int after_round() = 0;

};
#endif  //GAME_H_