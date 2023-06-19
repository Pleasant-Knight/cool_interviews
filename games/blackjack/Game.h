#pragma once


#ifndef GAME_H
#define GAME_H

#include "Deck.h"
#include "House.h"
#include "Player.h"

class Game {
public:
	Game(const vector<string>& names);

	~Game();

	void PlayGame();

private:
	Deck m_Deck;
	House m_House;
	vector<Player> m_Players;
	int m_num_players = 2;
};

#endif // GAME_H
