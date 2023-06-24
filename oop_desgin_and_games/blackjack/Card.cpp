#include "Card.h"

Card::Card(rank r, suit s, bool ifu) : m_Rank(r), m_Suit(s), m_isFaceUp(ifu) {}

int Card::GetValue() const {
	int val = 0;
	if (m_isFaceUp) {
		val = m_Rank;
	}

	if (val > 10)
		val = 10;

	return val;
}


void Card::Flip() {
	m_isFaceUp = !m_isFaceUp;
}
