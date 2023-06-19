/**
* Blackjack in Modern C++, using MS Visual Studio 2019.
*/

#include <cassert>
#include "Card.h"
#include "Deck.h"
#include "Game.h"
#include "Hand.h"
#include "House.h"
#include "BasePlayer.h"
#include "Player.h"

// Forward declration of the ostream operators.
ostream& operator << (ostream& os, const Card& aCard);
ostream& operator << (ostream& os, const BasePlayer& player);

int main() {
	cout << "\t\tWelcome To The BlackJack Mania!\n\n";

	int numPlayers = 0;
	while (numPlayers < 1 || numPlayers > 7) {
		cout << "Please enter number of players (1 to 7): ";
		cin >> numPlayers;
	}
	// Make sure that we have at least one player.
	assert(numPlayers >= 1);
	
	vector<string> names;
	string name;
	for (auto i = 0; i < numPlayers; ++i) {
		cout << "Enter player name: ";
		cin >> name;
		names.push_back(name);
	}
	cout << "\n";

	// The Game Loop.
	Game aGame(names);
	char again = 'y';
	
	while (again != 'n' && again != 'N') {
		aGame.PlayGame();
		cout << "\nDo you want to play again? (Y/N): ";
		
		cin >> again;
	}

	return 0;
}

/**
* Overloads << operator so Card object can be sent to cout
*/
ostream& operator << (ostream& os, const Card& aCard)
{
	const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9",
	"10", "J", "Q", "K" };
	const string SUITS[] = { "c", "d", "h", "s" };

	if (aCard.m_isFaceUp)
		os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
	else
		os << "XX";

	return os;
}

/**
* Overloads << operator so a Generic Playe object can be sent to cout
*/
ostream& operator<<(ostream& os, const BasePlayer& player)
{
	os << player.m_Name << ":\t";

	vector<Card*>::const_iterator pCard;
	if (!player.m_Cards.empty()) {
		for (pCard = player.m_Cards.begin();
			pCard != player.m_Cards.end(); ++pCard)
			os << *(*pCard) << "\t";
		if (player.GetTotal() != 0)
			cout << "(" << player.GetTotal() << ")";
	}
	else {
		os << "<empty";
	}

	return os;
}