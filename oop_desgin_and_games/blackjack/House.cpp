#include "House.h"


House::House(const string& name) : BasePlayer(name) {}

House::~House() {}

/*
===========================================
If the House is hitting
===========================================
*/
bool House::IsHitting() const
{
	return (GetTotal() <= Critical_Threshold);
}

/*
======================================
Flip the first card
======================================
*/

void House::FlipFirstCard()
{
	if (!(m_Cards.empty())) {
		cout << "House is flipping frist card\n";
		m_Cards.front()->Flip();
	}
	else {
		cout << "No more cards to flip!\n";
	}

}

