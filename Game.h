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
		virtual int before_round() = 0;
		virtual int round() = 0;
		virtual int after_round() = 0;


	protected:
		static shared_ptr<Game> gamePtr;
		Deck deck;
		vector<shared_ptr<Player>> players;


	public:
		static shared_ptr<Game> instance();
		size_t size() const; //number of players
		static void startGame(const string& sofgames);
		static void playGame();
		static void stopGame();
		void addPlayer(const string &name);
		void addPlayers(vector<string> list);
		vector<string> Game::getPlayers();
		shared_ptr<Player> findPlayer(const string name);
		void CheckChips();

		int autoPlayerLeave();
		int saveToFile();
		vector<size_t> findAuto();
		size_t countActive();
		int printPlayers(AccessMode mode);


};
#endif  //GAME_H_