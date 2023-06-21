#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

constexpr size_t MH_WIDTH = (8 * sizeof(uint8_t));
const uint64_t MH_HASH_LONGKEY_MASK = (0xFFFFFFFFFFFFFFFF);
const uint64_t randomsource = 0xABC0235698742343;
const uint32_t MH_HASH_SHRTKEY_MASK = (0xFFFFFFFF);
const size_t DEFAULT_NUM_HASH_FUNCS = 1;
const size_t DEFAULT_HASH_TABLE_SIZE = 1000;

// hash function pointers
uint32_t (*hashLong)(uint64_t, size_t);
uint32_t (*hashShrt)(uint32_t, size_t);

template <class KeyType>
class MH_HashTable {
	typedef uint32_t (*hf)(KeyType, size_t);
private:
	size_t size = DEFAULT_HASH_TABLE_SIZE;
	size_t used = 0;
	size_t numHashFunc = DEFAULT_NUM_HASH_FUNCS;

	/**hf hash1;	hf hash2;*/

	unsigned iterator;

	Bucket<KeyType, InfoType>* buckets;

	void startout(unsigned sz);
	void kill() { if (buckets != NULL) delete[] buckets; buckets = NULL; }
	void rehash();

	void pack() { if (ignores > (size >> 2)) rehash(); }
	void loosen() { if (empty < (size >> 2)) rehash(); }
public:
	HashTable(hf h1, hf h2) : hash1(h1), hash2(h2) { startout(minSize); }
	~HashTable() { kill(); }

	int insert(KeyType, InfoType); // returns 0 on failure
	int lookup(KeyType, InfoType&) const; // puts value into i on success
	int assign(KeyType, InfoType);
	int remove(KeyType);
	void restart() { startout(minSize); }
	int NumElts() const { return used; }
	void seek();
	unsigned startIterator();
	InfoType operator *(); // a destructive dereference
	unsigned operator ++() { seek(); return (iterator < size); }
};
