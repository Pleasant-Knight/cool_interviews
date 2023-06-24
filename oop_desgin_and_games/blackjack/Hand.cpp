#include "hand.h"

// Reserve 7 cards per hand.
Hand::Hand() {
	m_Cards.reserve(7);
}

// Virtual Dtor of Hand.
Hand::~Hand() {
	Clear();
}

// Add a card.
void Hand::Add(Card* pCard) {
	m_Cards.push_back(pCard);
}

// Clear the cards.
void Hand::Clear() {
	// Iterate through vector, freeing all memory.
	for (auto card : m_Cards) {
		delete card;
	}

	// Finally, Clear vector of pointers.
	m_Cards.clear();
}

// Get total of cards in the hand
int Hand::GetTotal() const {
	if (m_Cards.empty())
		return 0;

	// If a first can has value of 0, then card is face down; return 0.
	auto first = m_Cards[0];
	if (first->GetValue() == 0)
		return 0;

	int total = 0;
	
	// First, add all the cards values, treat each ace as 1.
	for (auto card : m_Cards)
		total += card->GetValue();

	/*
	=========================================
	Determine if hand contains an ace
	=========================================
	*/
	bool hasAce = false;
	for (auto card : m_Cards) {
		if (card->GetValue() == Card::ACE) {
			hasAce = true;
			break;
		}
	}

	// If hand contains ace and total is low enough, treat ace as 11.
	if (hasAce && total <= 11)
		// Add only 10 since we've already added 1 for the ace
		total += 10;

	return total;
}
