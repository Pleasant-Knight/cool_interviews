#include <iostream>
#include "Deck.h"
#include "Card.h"

Deck::Deck()
{
	m_Cards.reserve(52);

	Populate();
}

Deck::~Deck() {
	m_Cards.clear();
}

void Deck::Populate() {
	Clear();

	// Let's make a deck:
	for (int s = Card::Clubs; s < Card::Spades; s++) {
		for (int r = Card::ACE; r <= Card::KING; r++) {
			m_Cards.push_back(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s)));
			//Add(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s)));
		}
	}
}

void Deck::Shuffle() {
	// std::random_shuffle() Reorders the elements in the given range [first, last) such that
	// possible permutation of those elements has equal probability of appearance.
	random_shuffle(m_Cards.begin(), m_Cards.end());
}

void Deck::Deal(Hand& hand) {
	if (!m_Cards.empty()) {
		Card *card = m_Cards.back();
		hand.Add(card);
		m_Cards.pop_back();
	}
	else {
		cout << "We are out of cards!!\n";
	}
}

void Deck::MoreCards(BasePlayer& player) {
	// Deal more cards till he is busted or he withdraw

	while (!player.IsBusted() && player.IsHitting()) {
		Deal(player); // hmm, this is from card?

		cout << player;

		if (player.IsBusted())
			player.Bust();
	}
}