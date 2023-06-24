/**
* Blackjack in C++
* Made with MS Visual Studio 2019.
*/

#include "Card.h"
#include "Game.h"
#include "Player.h"

Game::Game(const vector<string>& names) {
	cout << "\nGot " << names.size() << " players plus the house\n";
	// Create the players' vector of names.
	for (auto name : names)
		m_Players.push_back(Player(name));

	// Seed The random number generator.
	srand(static_cast<unsigned int>(time(0)));
	
	m_Deck.Populate();
	m_Deck.Shuffle();
}

Game::~Game() {}

/**
* Let's play the game.
*/
void Game::PlayGame() {
	for (int i = 0; i < m_num_players; ++i) {
		for (auto player : m_Players)
			m_Deck.Deal(player);
		m_Deck.Deal(m_House);
	}

	// Hide the house's first card.
	m_House.FlipFirstCard();

	// Display everyone's hand
	for (auto player : m_Players)
		cout << player << endl;
	cout << m_House << endl;

	//Deal additional cards to players
	for (auto player : m_Players)
		m_Deck.MoreCards(player);

	// Reveal house's first card.
	m_House.FlipFirstCard();
	cout << endl << m_House;

	// Deal additional cards to house.
	m_Deck.MoreCards(m_House);

	if (m_House.IsBusted()) {
		// Everyone who is still playing wins.
		for (auto player : m_Players)
			if (!(player.IsBusted()))
				player.Win();
	} else 	{
		// Compare each player still playing to house.

		for (auto player : m_Players) {
			if (!(player.IsBusted())) {
				if (player.GetTotal() > m_House.GetTotal())
					player.Win();
				else if (player.GetTotal() < m_House.GetTotal())
					player.Lose();
				else
					player.Push();
			}
		}
	}

	// Remove everyone's cards.
	for (auto player : m_Players)
		player.Clear();
	
	m_House.Clear();
}