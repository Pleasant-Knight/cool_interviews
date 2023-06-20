#include <iostream>
#include "cache.h"


LRUCache::LRUCache(int capacity) {
    try {
        if (capacity <= 0) {
            throw capacity;
        }
    }
    catch (int e) {
        std::cout << "LRUCache: Zero or negative cache capacity occured: " << e << std::endl;
    }

    // Setup the linked list dummy nodes: head and tail
    head = new Node();
    tail = new Node();
    head->next = tail;
    tail->next = head->prev = nullptr;
    tail->prev = head;
    maxSize = capacity;
}

int LRUCache::get(std::string key) {
    int ret = -1;
    auto iter = Cache.find(key);
    if (iter != Cache.end()) {
        Node* node = iter->second;
        ret = node->val;
        // Udpate the node's position in the linked list
        removeNode(node);
        addNode(node);
    }
    return ret;
}

void LRUCache::put(std::string key, int val) {
    // try to see if the key already here:
    auto iter = Cache.find(key);
    if (iter != Cache.end()) {
        // Found the key
        Node* node = iter->second;
        // Remove node from current position, then add it to the head
        removeNode(node);
        // Just update the value
        node->val = val;
        addNode(node);
        goto end;
    } else {
        // In case the cache is at its max limit
        if (Cache.size() == maxSize) {
            // Remove the tail node from the actual cache hash table:
            Cache.erase(tail->prev->key);
            // Remove the node from the linked list:
            removeNode(tail->prev);
        }
        // Here we have space to add a node:
        Node* node = new Node();
        node->val = val;
        node->key = key;
        Cache.emplace(key, node);
        addNode(node);
    }
end:
    return;
}

void LRUCache::addNode(Node* n) {
    Node* old_head = head->next;
    n->next = head->next;
    old_head->prev = n;
    head->next = n;
    n->prev = head;
}

void LRUCache::removeNode(Node* n) {
    Node* next = n->next;
    Node* prev = n->prev;
    next->prev = prev;
    prev->next = next;
}
