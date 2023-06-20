#pragma once
#include <string>
#include <unordered_map>

constexpr auto MAX_CACHE_SIZE = (10);

struct Node {
public:
    std::string key;
    int val = 0; // size of the item
    Node* next = nullptr;
    Node* prev = nullptr;
};

class Cache {
protected:
    // cache hash table
    std::unordered_map<std::string, Node*> Cache;
    // The linked list members
    Node* head = nullptr;
    Node* tail = nullptr;
    // The maximum capacity of the cache in Bytes.
    int maxSize = MAX_CACHE_SIZE;
public:
    int getSize() { return Cache.size(); }
    int getMaxSize() { return this->maxSize; }
};

class LRUCache : public Cache {
public:
    LRUCache(int capacity);
    int get(std::string key);
    void put(std::string key, int val);
    void addNode(Node* n);
    void removeNode(Node* n);
};

/**
class LFUCache : public Cache {
public:
    LFUCache(int capacity);
    int get(int key);
    void addNode(Node* n);
    void removeNode(Node* n);
    void put(int key, int val);
}; */

/**
#ifndef _MY_CACHE_H
#define  _MY_CACHE_H
#endif  //_MY_CACHE_H
*/