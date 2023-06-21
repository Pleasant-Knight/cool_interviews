/**
 * Hikers and bridges solution.
 * Michel Hanna: 2020
 */

#include "hikers.h"

// Add a new bridge the singlton bridge with its length
// use heap (prirotiy queue)
void Bridge::addBridgePri(const std::vector<BLD> hikerSpeeds, BLD length) {
	for (const auto& h : hikerSpeeds)
		this->hiker_speed.push_back(h);
	this->numHikers.push_back(hikerSpeeds.size());
	this->bridge_length.push_back(length);
	// call getCrossTime()
	std::cout << "Current bridge cross time: " << this->getCrossTimePri() << "\n";
}


// Add a new bridge the singlton bridge with its length:
// Use std::max_element()
void Bridge::addBridgeSTL(const std::vector<BLD> hikerSpeeds, BLD length) {
	for (const auto& h : hikerSpeeds)
		this->hiker_speed.push_back(h);
	this->numHikers.push_back(hikerSpeeds.size());
	this->bridge_length.push_back(length);
	// call getCrossTime()
	std::cout << "Current bridge cross time: " << this->getCrossTimeSTL() << "\n";
}

 /**
  * 
  * Calculate the bridge cross time during the night for multiple scenarios.
  *  - First scenario: maximum two can cross the bridge with one torsh.
  *   This means that the fastest total time to cross will be when we
  *   find the fastest hiker and make him cross with each hiker then
  *	  come back to pick the next hiker.
  *	  The order to pick which hiker to cross with the fastest one is
  *	  not important as long as the fastest one is going back and forth
  *	  to keep the torsh needed to light the bridge.
  *
  * This API is using priority queue (heap) to keep track of the 
  * top speed hiker.
  *
  */
long double Bridge::getCrossTimePri() {
	if (hiker_speed.empty())
		return (0.0);

	//TODO: provide different calculation for more 
	// than two hikers and one torsh.
	if ((this->maxNHikers != MAX_NUM_HICKERS) ||
		(this->numTorshes != NUM_TORSHES)) {
		DBGMSG(std::cout, "Cannot calculate time to cross this bridge");
		return 0.0;
	}
	int idx = (int)(bridge_length.size()) - 1;
	BLD length = bridge_length[idx];
	int hikers = hiker_speed.size();
	// create a priority queue
	std::priority_queue<float> hSpeeds(hiker_speed.begin(), hiker_speed.end());

	BLD crossTime = 0.0, topSpeed = hSpeeds.top();
	hSpeeds.pop();
	crossTime = (((BLD)hikers - 1) * topSpeed / length);
	// Get the rest of the hikers
	while (!hSpeeds.empty()) {
		crossTime += (length / hSpeeds.top());
		hSpeeds.pop();
	}
	// record this bridge cross time
	this->bridgesCrossTimes.push_back(crossTime);
	// update the total cross time
	this->totalCrossTime += crossTime;

	// This is the fastest time in minutes
	return crossTime;
}

/**
 * Same like getCrossTimePri() except that we are not using
 * priority queue, instead we are calculating the top speed
 * hiker using stl api: std::max_element()
 */
long double Bridge::getCrossTimeSTL() {
	if (hiker_speed.empty())
		return (0.0);

	//TODO: provide different calculation for more 
	// than two hikers and one torsh.
	if ((this->maxNHikers != MAX_NUM_HICKERS) ||
		(this->numTorshes != NUM_TORSHES)) {
		DBGMSG(std::cout, "Cannot calculate time to cross this bridge");
		return 0.0;
	}

	// Get the topSpeed hiker:
	BLD topSpeed = *std::max_element(hiker_speed.begin(), hiker_speed.end());
	BLD length = bridge_length[bridge_length.size() - 1];
	BLD crossTime = (((BLD)(hiker_speed.size()) - 2) * topSpeed / length);
	// Get the rest of the hikers time
	for (const auto& h : hiker_speed) {
		crossTime += (length / h);
	}
	// record this bridge cross time
	this->bridgesCrossTimes.push_back(crossTime);
	// update the total cross time
	this->totalCrossTime += crossTime;
	
	// This is the fastest time in minutes
	return crossTime;
}

// The default constructor that we are using:
Bridge::Bridge(std::vector<std::vector<BLD>> hSpeeds, std::vector<BLD> length) {
	DBGMSG(std::cout, "The constructor ...");
	BLD t = 0.00;
	for (int idx = 0; idx < ((int)hSpeeds.size()); idx++) {
		this->numHikers.push_back(hSpeeds[idx].size());
		for (auto j = 0; j < ((int)hSpeeds[idx].size()); j++) {
			t = hSpeeds[idx][j];
			DBGVAR(std::cout, t);
			this->hiker_speed.push_back(t);
		}
		this->bridge_length.push_back(length[idx]);
		// call eval getCrossTimeSTL() or this->getCrossTimePri();
		BLD tmp = this->getCrossTimeSTL(); 
		std::cout << "\n\nBridge: " << (idx + 1) << " Cross time: " 
			<< tmp << " minutes, Total Cross time so far: "<< this->totalCrossTime << " minutes\n\n";
	}
	
	return;
}
