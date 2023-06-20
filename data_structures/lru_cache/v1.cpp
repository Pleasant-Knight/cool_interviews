#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "cache.h"

using namespace std;


int main() {
	ifstream myfile("input.txt");
	string line;

    // Read the size of the cache first.
    getline(myfile, line);
    int cacheSize = stoi(line);
    
    // instantiate LRU cache here
    LRUCache lruCache(cacheSize);

    int urlSize;
    string url;

    // read the rest of file
    while (std::getline(myfile, line)) {
        // skip empty lines
        if (line.empty())
            continue;
        // read the url
        stringstream ss(line);
        ss >> url >> urlSize;

        lruCache.put(url, urlSize);
    }

    cout << "Testing LRU Cache: Current size = " << lruCache.getSize() << " of Max: " << lruCache.getMaxSize() << endl;

	return 0;
}