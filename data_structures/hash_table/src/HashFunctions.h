#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <unistd.h> // for getopt()


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


// Forward decals for getopt()
//extern char* optarg;
//extern int optind, opterr, optopt;
//extern int getopt(int, char* [], char*);


//#include "HashTable.h"
constexpr size_t MH_WIDTH = (8 * sizeof(uint8_t));
const uint64_t MH_HASH_LONGKEY_MASK = (0xFFFFFFFFFFFFFFFF);
const uint64_t randomsource = 0xABC0235698742343;
const uint32_t MH_HASH_SHRTKEY_MASK = (0xFFFFFFFF);
constexpr size_t MH_DefhashTableSize = (1 << 10);
constexpr size_t MH_DefnumKeysGen = (1 << 7);
constexpr size_t MH_DefmaxBlockSize = (1 << 19);


// hash function pointers
typedef uint32_t(*hashLong)(uint64_t, size_t);
typedef uint32_t(*hashShrt)(uint32_t, size_t);

/**
 * 64b and 32b hash functions.
 */
uint32_t crc32Fast64(uint64_t key, size_t);
uint32_t verkHash64(uint64_t key, size_t sz);
uint32_t XORhash64(uint64_t key, size_t sz);
uint32_t hashDJB264(uint64_t key, size_t sz);
uint32_t hashSDBM64(uint64_t key, size_t sz);
uint32_t hashUxELF64(uint64_t key, size_t sz);
uint32_t hashPJW64(uint64_t key, size_t sz);

uint32_t crc32Fast32(uint32_t key, size_t);
uint32_t verkHash32(uint32_t key, size_t sz);
uint32_t XORhash32(uint32_t key, size_t sz);

struct HashFuncs {
    int numHashFuncs = 1;
    bool keyType32 = true;
    size_t hashTableSize = MH_DefhashTableSize;
    size_t numKeysGen = MH_DefnumKeysGen;
    size_t maxBlockSize = MH_DefmaxBlockSize;
    // generate comma separated value file
    std::string outFileName = "std_out.csv";
    uint64_t randomSeed = randomsource;
};

// store the hash functions and their names:
std::unordered_map<std::string, hashLong> HashFuncs64 = { 
                                                    { "verkHash64", verkHash64 },
                                                    { "crc32Fast64", crc32Fast64 },
                                                    { "XORhash64", XORhash64 },
                                                    { "hashDJB264", hashDJB264 },
                                                    { "hashSDBM64", hashSDBM64 },
                                                    { "hashUxELF64", hashUxELF64 },
                                                    { "hashPJW64", hashPJW64} };

std::unordered_map<std::string, hashShrt> HashFunctionNames32 = {
                                                    { "crc32Fast32", crc32Fast32 },
                                                    { "verkHash32", verkHash32 },
                                                    { "XORhash32", XORhash32 } };
