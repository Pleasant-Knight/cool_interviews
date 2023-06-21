#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "windows.h"

using namespace std;

constexpr int MHLZW_IDX = 256;

enum class CompressionMethod {
	CLZ77 = 0, MHLZ77 = 1, CLZW = 2, MHLZW = 3, CLZ78 = 4, MHLZ78 = 5,
	First = CLZ77, Last = MHLZW
};

struct Node {
	int index;
	string data;
	Node* next;
	
	Node() {
		index = 0;
		data = "";
		next = nullptr;
	}
	Node(int idx, string str, Node* nxt = nullptr) : index(idx), data(str), next(nxt) {}
};


/** Compression API's. */
string CompressionMethod_toString(CompressionMethod);
CompressionMethod operator++(CompressionMethod& x);
CompressionMethod operator*(CompressionMethod c);
CompressionMethod begin(CompressionMethod r);
CompressionMethod end(CompressionMethod r);
size_t compressDecomress(bool, CompressionMethod, string fileName);

Node* search_Node(Node* head, int index);
Node* search_Node(Node* head, string data);
void insert_Node(Node* head, int index, string data);
void st_Node(Node* head, int index, string data);
vector <string> splitString(string str, char delimiter);

string LZ77(string input, int option);
string LZ78(string input, int option);
string LZW(string input, int option);

string MHLZWCompress(string input);
string MHLZWDeCompress(string input);
string MHLZ77Compress(string input);
string MHLZ77DeCompress(string input);