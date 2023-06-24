#include <iostream>
#include <fstream>
#include <cstdlib> // for rand()
#include <ctime>
#include "hikers.h"

/**
 *
 * Sample file to be generated:
	---
# First configuration file for Scala
# We will start our config file with the total number of bridges.
# This way we can add (potentially remove) hikers.
# Next, we describe each bridge with its length. Next, we describe each

# in this config file, we have three bridges
hikerbridges:
	- bridge:
		length: 100.00      # length of 1st bridge is 100 feet
		hikers: [ 50.00, 20, 10.00 ]   # Initial hikers' speed: A to C
	- bridge:
		length: 250.00      # Length of the 2nd bridge is 250 feet
		hikers: [ 2.5 ]     # hiker D
	- bridge:
		length: 150.00      # Length of the 3rd bridge is 150 feet
		hikers: [ 25.00, 15.00 ]    # hikers E & F
 *
 */

// API to generate YAML file
bool generateYamlFile(configGenFile& cfg) {
	int randSeed = cfg.randSeed;
	if (randSeed == 0) {
		randSeed = (int)time(NULL);
	}
	DBGVAR(std::cout, randSeed);
	srand(randSeed);
	if (cfg.fileName.empty() || (cfg.fileName == "")) {
		std::cerr << "Error: cannot create outfile\n";
		return false;
	}
	// generate the fields for the file:
	std::ofstream out(cfg.fileName, std::ofstream::out);
	out << "---\n";
	out << "# Config YAML file for the hikers-bridges\n";
	int numBridges = (rand() % cfg.numBridges + 1); // range between [1, numBridges]
	DBGVAR(std::cout, numBridges);
	out << "# in this file " << cfg.fileName << " we have " << numBridges
		<< "bridges:\n";
	double t = 0.00;
	int d = 0;
	out << "hikerbridges:\n";
	for (int i = 0; i < numBridges; i++) {
		d = rand() % cfg.maxBridgeLen + 1;
		t = (double)d;
		out << "    - bridge:\n";
		out << "        length: " << t << "\n";
		
		int numHikers = (rand() % cfg.maxNumHikers + 1);
		DBGVAR(std::cout, numHikers);
		out << "        hikers: [ ";
		for (int j = 0; j < numHikers; j++) {
			d = rand() % cfg.maxHikerSpeed + 1;
			t = (double)d;
			out << t;
			if (j != (numHikers - 1))
				out << ", ";
		}
		out << " ] # got " << numHikers << " Hikers for bridge: " 
			<< i + 1 <<".\n";
	}
	out.close();

	return true;
}