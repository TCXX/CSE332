/*
Player.cpp created by Cindy Le, Adrien Xie, and Yanni Yang
*/

#include "stdafx.h"
#include "Player.h"

using namespace std;

//A constructor that tries to read its member variables from a file that has the same name as the player.
Player::Player(string playername, bool aut):
		name(playername), won(0), lost(0), chip(20), bet(0), isFold(false), isAuto (aut) {
	hand = Hand();

	//name is stored without '*'

	string filename = name + ".txt";
	ifstream ifs(filename);
	if (ifs.is_open()) {
		string line;
		vector<string> temp = vector<string>(); //to store readins

		//readin strings
		while (!ifs.eof()) {
			getline(ifs, line);
			temp.push_back(line);
		}

		if (temp.size()>=4) { //valid
			//name = temp[0];
			won = stoi(temp[1]);
			lost = stoi(temp[2]);
			chip = stoi(temp[3]);
		}	
	}
}

string Player::toString() const {
	ostringstream out = ostringstream();

	//basic info
	out << "Player " << name;
	if (isAuto) out << "*";
	out << "(" << chip << "): ";

	//hand content
	if (isFold) {
		int len = hand.size();
		for (int i = 0; i < len; i++) out << "**  ";
	}
	else {
		out << hand;
	}

	return out.str();
}

int Player::reset() {
	chip = 20;
	return 0;
}

//A non-member insertion operator that prints out the player's name, number of wins, and number of losses.
ostream& operator<<(ostream& out, const Player& p) {
	out << p.toString();
	return out;
}

//
ofstream& operator<<(ofstream& out, const Player& p) {
	out << p.name << endl;
	out << p.won << endl;
	out << p.lost << endl;
	return out;
}

//
bool handCompare(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2) {
	if (p1.get() == NULL) return false;
	if (p2.get() == NULL) return true;
	return p1->hand < p2->hand;
}

