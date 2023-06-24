#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>

/******************************************************************/
// Define few debug prints.
// If you want to show all the prints in debug mode, set DEBUG to 1 or more
#define DEBUG 0

#if defined(DEBUG) && (DEBUG > 0)
#define DBGVAR(os, var) \
        (os) << "DBG: " << __func__ << ":(" << __LINE__ << ") "\
             << #var << " = [" << (var) << "]" << std::endl
#define DBGMSG(os, msg) \
        (os) << "DBG: " << __func__ << ":(" << __LINE__ << ") "\
             << msg << std::endl
#else
#define DBGVAR(os, var)
#define DBGMSG(os, msg)
#endif
/******************************************************************/

// Forward declration for the timer class in timer.cpp.
class Timer;

// default values for a file
constexpr int Default_randSeed = 0x0ABEBAB1;
constexpr int Default_numBridges = 4;
constexpr int Default_maxNumHikers = 5;
constexpr int Default_maxBridgeLen = 500;
constexpr int Default_maxHikerSpeed = 25;

// Tempalte for generating config file
struct configGenFile {
	int randSeed = Default_randSeed;
	int numBridges = Default_numBridges;
	int maxNumHikers = Default_maxNumHikers;
	int maxBridgeLen = Default_maxBridgeLen;
	int maxHikerSpeed = Default_maxHikerSpeed;
	std::string fileName = "";
};

// Use this to keep track of crossing time, and bridge length
using BLD = long double;	// Bridge long double


// Based on the given information in the question, we allow 2 hikers
// to cross the bridge, and only one torsh.
constexpr int MAX_NUM_HICKERS = 2;
constexpr int NUM_TORSHES = MAX_NUM_HICKERS - 1;
constexpr BLD DEFAULT_BRIDGE_LENGHT = 1;

// API to generate YAML file
bool generateYamlFile(configGenFile& cfg);

/**
 * Class to keep the bridge properties.
 * The desgin will use a singlton (one object of this class only).
 * Everytime we parse new bridge, we use addBridge()
 */

class Bridge {
private:
	// Length of each bridge in feet
	std::vector<BLD> bridge_length;
	
	// Keep track of of how many hikers were added to each bridge
	std::vector<size_t> numHikers;
	
	// Each hikers speed 
	std::vector<BLD> hiker_speed;
	
	// Parametrize the constriants so that if they change, algorithm can still work
	// Maximum number of hikers can go over the bridge
	int maxNHikers = MAX_NUM_HICKERS;
	int numTorshes = NUM_TORSHES;

	// Keep track of the total cross time for all the brdiges so far
	BLD totalCrossTime = 0;
	// Keep track of each bridge cross time
	std::vector<BLD> bridgesCrossTimes;

public:	
	// The move constructor with length modifier
	Bridge(std::vector<BLD> && hikers_speed, BLD length) {
		DBGMSG(std::cout, "Move constructor ...");
		this->numHikers.push_back(hikers_speed.size());
		this->hiker_speed = std::move(hiker_speed);
		this->bridge_length.push_back(length);
	}

	Bridge(std::vector<std::vector<BLD>> hSpeeds, std::vector<BLD> length);

	// Add one more bridge with its hikers and their speeds to the 
	// singlton bridge.
	// Also adjust the length of the bridge.
	void addBridgePri(const std::vector<BLD> speeds, BLD length);
	void addBridgeSTL(const std::vector<BLD> speeds, BLD length);
	
	// return the current number of hikers to cross the bridge
	size_t getHikers() {
		return this->hiker_speed.size();
	}

	// return the vector for all bridge cross time
	std::vector<BLD> getBridgesCrossTimes() {
		return this->bridgesCrossTimes;
	}

	// return the total cross time of all the bridges so far
	BLD getTotalCrossTime() {
		return this->totalCrossTime;
	}

	/**
	 * Calculate the bridge cross time during the night for multiple scenarios.
	 *  - First scenario: maximum two can cross the bridge with one torsh.
	 *    This means that the fastest total time to cross will be when we 
	 *    find the fastest hiker and make him cross with each hiker then 
	 *	  come back to pick the next hiker. 
	 *	  The order to pick which hiker to cross with the fastest one is 
	 *	  not important as long as the fastest one is going back and forth
	 *	  to keep the torsh needed to light the bridge.
	 *		  
	 */
	BLD getCrossTimePri();
	
	/**
	 * Same like getCrossTimePri() except that we are not using 
	 * priority queue, instead we are calculating the top speed 
	 * hiker using stl api: std::max_element()
	 */
	BLD getCrossTimeSTL();
};
