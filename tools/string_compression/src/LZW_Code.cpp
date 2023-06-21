#include "Compress.h"

CompressionMethod operator++(CompressionMethod& x) {
	return x = (CompressionMethod)(underlying_type<CompressionMethod>::type(x) + 1);
}

CompressionMethod operator*(CompressionMethod c) {
	return c;
}

CompressionMethod begin(CompressionMethod r) {
	return CompressionMethod::First;
}

CompressionMethod end(CompressionMethod r) {
	CompressionMethod l = CompressionMethod::Last;
	return ++l;
}

string CompressionMethod_toString(const CompressionMethod c) {
	string str;
	switch (c) {
		case CompressionMethod::CLZ77: str = "LZ77";		break;
		case CompressionMethod::MHLZ77: str = "MHLZ77"; break;
		case CompressionMethod::CLZ78: str = "LZ78";		break;
		case CompressionMethod::MHLZ78: str = "MHLZ78";	break;
		case CompressionMethod::CLZW: str = "LZW";		break;
		case CompressionMethod::MHLZW: str = "MHLZW";	break;
	default: str = "Invalid Compressoin Method";
	}
	return str;
}
ostream & operator<<(ostream &os, const CompressionMethod c) {
	return os << CompressionMethod_toString(c);
}

void st_Node(Node* head, int index, string data) {
	head->index = index;
	head->data = data;
	head->next = nullptr;
}

size_t compressDecomress(bool compress, CompressionMethod method, string fileName) {
	// open the input file
	string input = "", result, line = "";
	// open the input file
	ifstream myfile("in.txt");

	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			input += line;
		}
		myfile.close();
	} else {
		cerr << "Err: Unable to open file" << fileName << endl;
		return INT_MAX;
	}

	auto org_file_size = input.length();
	cout << "Opened file: " << fileName << " with size: " << org_file_size << "\n\n";
	
	if (compress) {
		// choose the right function
		switch (method) {
		case CompressionMethod::MHLZ77:
			result = MHLZ77Compress(input);
			break;

		case CompressionMethod::CLZ77:
			result = LZ77(input, 1);
			break;

		case CompressionMethod::CLZ78:
			result = LZ78(input, 1);
			break;

		case CompressionMethod::CLZW:
			result = LZW(input, 1);
			break;

		case CompressionMethod::MHLZW:
			result = MHLZWCompress(input);
			break;
		}
	} else { // Decompress
		switch (method) {
		case CompressionMethod::MHLZ77:
			result = MHLZ77DeCompress(input);
			break;

		case CompressionMethod::CLZ77:
			result = LZ77(input, 2);
			break;

		case CompressionMethod::CLZ78:
			result = LZ78(input, 2);
			break;

		case CompressionMethod::CLZW:
			result = LZW(input, 2);
			break;

		case CompressionMethod::MHLZW:
			result = MHLZWDeCompress(input);
			break;
		}
	}
	auto final_file_size = result.length();

	return final_file_size;
}

void insert_Node(Node* head, int index, string data) {
	Node* new_Node = new Node(index, data, nullptr);

	Node* curr = head;
	while (curr != nullptr) {
		if (curr->next == nullptr) {
			curr->next = new_Node;
			return;
		}
		curr = curr->next;
	}
}

Node* search_Node(Node* head, string data) {
	Node* curr = head;
	while (curr != nullptr) {
		if (data.compare(curr->data) == 0)
			return curr;
		else
			curr = curr->next;
	}

	return nullptr;
}

Node* search_Node(Node* head, int index) {
	Node* curr = head;
	while (curr != nullptr) {
		if (index == curr->index)
			return curr;
		else
			curr = curr->next;
	}

	return nullptr;
}

bool delete_Node(Node* head, Node* to_delete) {
	if (to_delete == nullptr) {
		return false;
	} else if (to_delete == head) {
		head = to_delete->next;
		delete to_delete;
		return true;
	} else {
		Node* curr = head;
		while (curr) {
			if (curr->next == to_delete) {
				curr->next = to_delete->next;
				delete to_delete;
				return true;
			}
			curr = curr->next;
		}
		return false;
	}
}

vector<string> splitString(string str, char delimiter) {
	vector<string> internal;
	stringstream ss(str);
	string tok;

	while (getline(ss, tok, delimiter)) {
		internal.push_back(tok);
	}

	return internal;
}

string LZ77(string input, int option) {
	// Initialized variables
	string result;
	int length, char_info_selc = 0;

	if (option == 1) {
	check_char:		// Length checker pointer
		length = (int)input.length();	// Calculate input string length
		// Check input line length is less than 3
		if (length <= 2) {
			cerr << "Err: enter at leaset 3 characters \n";
			getline(cin, input);		// Read input string
			goto check_char;
		}

		// Declare an arry for final result called 'result_ary'
		int** result_ary = new int* [3];
		for (int i = 0; i < 3; ++i)
			result_ary[i] = new int[length];
		// Set result_ary elements value to 0 to prevent previous values
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < length; j++)
				result_ary[i][j] = 0;
		}

		// Declare an arry to store every char info in input string called 'char_info'
		int** char_info = new int* [3];
		for (int i = 0; i < 3; ++i)
			char_info[i] = new int[length];
		// Set char_info elements value to 0 to prevent previous values
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < length; j++)
				char_info[i][j] = 0;
		}

		// Set first char info to (0,0,'<first char>')
		result_ary[0][0] = 0;
		result_ary[1][0] = 0;
		result_ary[2][0] = input[0];

		// Set result counter
		int result_count = 1;

		// A loop to perform some operations in every char in input string
		for (int i = 1; i < length; i++) {
			// A loop to check input char in position i is equal to any of
			// previous char in input and save this info in char_info array
			for (int j = 0; j < i; j++) {
				// Check position of previous view of element i
				if (input[i] == input[j]) {
					// Set position pointer
					char_info[0][char_info_selc] = i - j;

					// Increase char info array selector by 1
					char_info_selc++;
				}

			}

			// A loop to check length for every char position
			for (int j = 0; j < length; j++) {
				// Check current char info array position is not equal to 0
				if (char_info[0][j] != 0) {
					// Set start point
					auto start = i - char_info[0][j];

					// Set count to calculate length for this char position
					auto count = 1;

					// A loop to check length for this char position
					for (int k = 0; k < length; k++) {
						// Check next element of start by next element of input
						if (input[start + count] == input[i + count])
							count++;	// Increase count by 1
						else {
							char_info[1][j] = count;	// Store count value in length 

							// Check if this input char is the last char
							if (i != (length - 1)) {
								// Store next char to char info
								// Check this postion is equal to length
								if (char_info[0][j] + count == length)
									char_info[2][j] = 0;	// Set 0 in next char field
								else
									char_info[2][j] = input[char_info[0][j] + count];	// Set the next char
							} else {
								char_info[2][j] = 0;		// Set NULL in next char field
							}
							break;	// Stop loop
						}
					}
				}
			}

			// Set large selector
			int large = 0;	// large selector equal 0

			// Loop to check the largest length for every char info
			for (int k = 1; k < length; k++) {
				// Check largest
				if (char_info[1][large] == char_info[1][k])
					large = k;	// Set largest
				else if (char_info[1][large] < char_info[1][k])
					large = k;	// Set largest
			}

			// Check largest length is equal to 0
			if (char_info[1][large] == 0)
				char_info[2][large] = input[i];		// Set char info
			else {
				i += char_info[1][large];		// increase loop counter by length of the largest char info element
				char_info[2][large] = input[i];		// Set char info
			}

			// Set final result info
			result_ary[0][result_count] = char_info[0][large];
			result_ary[1][result_count] = char_info[1][large];
			result_ary[2][result_count] = char_info[2][large];

			// Increase result counter
			result_count++;

			// Prepare char info array for next char by set it to 0
			for (int z = 0; z < 2; z++)
			{
				for (int j = 0; j < length; j++)
					char_info[z][j] = 0;	// Set every element in char info to 0
			}

			// Prepare char info selector for next char by set it to 0
			char_info_selc = 0;
		}

		// Display final results
		for (int j = 0; j < length; j++) {
			if (result_ary[0][j] == 0 && result_ary[1][j] == 0) {
				if (result_ary[2][j] != 0) {
					char z = result_ary[2][j];
					result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + "," + z + " ";
				}
			} else {
				//char z = result_ary[2][j];
				result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + ",0 ";
			}
		}

		return result;
	} else if (option == 2) {
		vector<string> s_input = splitString(input, ' ');

		for (int i = 0; i < s_input.size(); ++i) {
			vector<string> ss_input = splitString(s_input[i], ',');

			int p = stoi(ss_input[0]),
				l = stoi(ss_input[1]);
			string ch;
			if (ss_input[2][0] == '0')
				ch = ' ';
			else
				ch = ss_input[2];

			if (p != 0)
			{
				int result_len = (int)result.length();
				for (int x = 0; x < l; x++)
					result += result[result_len - p + x];
			}

			if (ch[0] != '0' || ch[0] != 0)
				result += ch;
		}
		return result;
	}
	return result;
}

string LZ78(string input, int option) {
	string result;
	if (option == 1) {
		string word;
		int length, last_seen = 0, index = 1;

		length = (int)input.length();
		word = input[0];
		Node* dictionary = new Node(1, word, nullptr);
		result += "0," + word;

		for (int i = 1; i < length; i++) {
			string data;
			data = input[i];

		re_check:
			Node* search = search_Node(dictionary, data);

			if (search) {
				i++;
				data += input[i];
				last_seen = search->index;
				goto re_check;
			} else {
				char zero;
				if (input[i] == ' ')
					zero = '0';
				else
					zero = input[i];

				if ((int)data.length() < 2)
					result += " " + to_string(0) + "," + zero;
				else
					result += " " + to_string(last_seen) + "," + zero;

				index++;
				if (i != length)
					insert_Node(dictionary, index, data);
			}
		}

		return result;
	}
	
	if (option == 2) {
		Node* dictionary = new Node();
		vector <string> s_input = splitString(input, ' ');
		int zz = 2;
		for (int i = 0; i < s_input.size(); i++) {
			vector <string> ss_input = splitString(s_input[i], ',');

			if (i == 0)
			{
				st_Node(dictionary, 1, ss_input[1]);
				result += ss_input[1];
			} else {
				Node* serched;
				string get_search = ss_input[1];
				serched = search_Node(dictionary, stoi(ss_input[0]));
				if (serched) {
					result += serched->data + get_search;
					get_search = serched->data + splitString(s_input[i], ',')[1];
					insert_Node(dictionary, zz, get_search);
				} else {
					if (stoi(ss_input[0]) == 0)
						insert_Node(dictionary, zz, get_search);
					else
						insert_Node(dictionary, zz, get_search);

					result += get_search;
				}
				zz++;
			}
		}

		if (result[(int)result.length() - 1] == '0')
			result = result.substr(0, result.size() - 1);

		return result;
	}
	return result;
}

string LZW(string input, int option) {
	string result;
	if (option == 1) {
		int length, last_seen = 0, index = 128;
		Node * dictionary = new Node(32, " ", nullptr);
		for (int i = 33; i < 128; i++) {
			string data;
			data = i;
			insert_Node(dictionary, i, data);
		}

		length = (int)input.length();
		Node* searched = nullptr;
		string search;
		search.clear();
		for (int i = 0; i < length; i++) {
			search = input[i];

		re_search:
			searched = search_Node(dictionary, search);
			if (searched) {
				i++;
				search += input[i];
				last_seen = searched->index;
				if (i != length)
					goto re_search;
				else
					goto print;
			} else {
				insert_Node(dictionary, index, search);
				index++;
			print:
				result += to_string(last_seen) + " ";
				i--;
			}
		}

		return result;
	}
	if (option == 2) {
		int index = 128;

		Node* dictionary = new Node(32, " ", nullptr);
		for (int i = 33; i < 128; i++) {
			string data;
			data = i;
			insert_Node(dictionary, i, data);
		}

		vector <string> s_input = splitString(input, ' ');
		for (int i = 0; i < s_input.size(); i++) {
			Node* searched;
			int search;
			search = stoi(s_input[i]);

			searched = search_Node(dictionary, search);

			string cur, prev, data;
			if (searched)
				cur = search_Node(dictionary, stoi(s_input[i]))->data;
			if (i != 0)
				prev = search_Node(dictionary, stoi(s_input[i - 1]))->data;
			else
				prev = cur;

			int show = 0;
			if (searched) {
				result += searched->data;

				if (i != 0) {
					data = prev + cur[0];
					if (show != 1)
					{
						insert_Node(dictionary, index, data);
						index++;
					}
				}
				show = 0;
			} else {
				data = prev + prev[0];
				insert_Node(dictionary, index, data);
				index++;
				show = 1;
				result += data;
			}
		}

		return result;
	}
	return result;
}

string MHLZWCompress(string input) {
	string result;
	int length, last_seen = 0, index = MHLZW_IDX;
	unordered_map<string, uint32_t> ht; // ht for all words
	int i;
	for (i = 0; i < MHLZW_IDX; i++) {
		ht[to_string(i)] = i;
	}

	length = (int)input.length();
	string srch;
	for (i = 0; i < length; i++) {
		srch = input[i];
	search:
		auto it = ht.find(srch);
		if (it != ht.end()) {
			srch += input[++i];
			last_seen = it->second;
			if (i != length) {
				goto search;
			} else {
				result += to_string(last_seen) + " ";
				i--;
			}
		} else { //insert new code into dictionary;
			ht[srch] = index++;
			result += to_string(last_seen) + " ";
			i--;
		}
	}

	return result;
}


string MHLZWDeCompress(string input) {
	string result;
	int index = MHLZW_IDX;

	unordered_map<uint32_t, string> ht; // ht for all words
	int i;
	for (i = 0; i < MHLZW_IDX; i++) {
		ht[i] = to_string(i);
	}

	vector <string> s_input = splitString(input, ' ');
	for (int i = 0; i < s_input.size(); i++) {
		int srch = stoi(s_input[i]);
		auto it = ht.find(srch);
		string cur = "", prev, data;
		bool found = (it != ht.end());
		if (found) {
			cur = it->second;
		}

		prev = cur;
		if (i != 0) {
			auto idx = ht.find(stoi(s_input[i - 1]));
			prev = idx->second;
		}

		bool show = false;
		if (found) {
			result += it->second;
			if (i != 0) {
				data = prev + cur[0];
				if (!show) {
					//insert_Node(dictionary, index, data);
					//index++;
					ht[index++] = data;
				}
			}
			show = false;
		} else {
			data = prev + prev[0];
			ht[index++] = data;
			show = true;
			result += data;
		}
	}

	return result;
}

string MHLZ77Compress(string input) {
	// Initialized variables
	string result;
	int length = 0, char_info_selc = 0, start = 0, count = 0;
	length = (int)input.length();	// Calculate input string length
	// Declare an arry for final result called 'result_ary'
	vector<vector<int>> result_ary(3, vector<int>(length, 0));
	// Set first char info to (0,0,'<first char>')
	result_ary[2][0] = input[0];

	// Declare an arry to store every char info in input string called 'char_info'
	vector<vector<int>> char_info(3, vector<int>(length, 0));

	// Set result counter
	int result_count = 1;

	// A loop to perform some operations in every char in input string
	for (int i = 1; i < length; i++) {
		// A loop to check input char in position i is equal to any of
		// previous char in input and save this info in char_info array
		for (int j = 0; j < i; j++) { 
			if (input[i] == input[j]) { // Check position of previous view of element i
				// Set position pointer
				char_info[0][char_info_selc++] = i - j; // Increase char info array selector by 1
			}
		}

		// A loop to check length for every char position
		for (int j = 0; j < length; j++) {
			// Check current char info array position is not equal to 0
			if (char_info[0][j] != 0) {
				start = i - char_info[0][j]; // Set start point
				count = 1; // Set count to calculate length for this char position

				// A loop to check length for this char position
				for (int k = 0; k < length; k++) {
					// Check next element of start by next element of input
					if (input[start + count] == input[i + count]) {
						count++;	// Increase count by 1
					} else {
						char_info[1][j] = count;	// Store count value in length 
						if (i != (length - 1)) { // Check if this input char is the last char
							// Store next char to char info
							// Check this postion is equal to length
							char_info[2][j] = (char_info[0][j] + count != length) ? input[char_info[0][j] + count] : 0;	// Set the next char
						} else {
							char_info[2][j] = 0;		// Set NULL in next char field
						}

						break;	// Stop loop
					}
				}
			}
		}

		// Set large selector
		int large = 0;	// large selector equal 0

		// Loop to check the largest length for every char info
		for (int k = 1; k < length; k++) {
			if (char_info[1][large] <= char_info[1][k]) // Check largest
				large = k;	// Set largest
		}

		// Check largest length is equal to 0
		i = (char_info[1][large] != 0) ? (i + char_info[1][large]) : i;
		char_info[2][large] = input[i];		// Set char info

		// Set final result info
		result_ary[0][result_count] = char_info[0][large];
		result_ary[1][result_count] = char_info[1][large];
		result_ary[2][result_count] = char_info[2][large];

		// Increase result counter
		result_count++;

		// Prepare char info array for next char by set it to 0
		// Set every element in char info to 0
		for (int j = 0; j < length; j++)
			char_info[0][j] = char_info[1][j] = 0;

		// Prepare char info selector for next char by set it to 0
		char_info_selc = 0;
	}

	// Display final results
	for (int j = 0; j < length; j++) {
		if (result_ary[0][j] == 0 && result_ary[1][j] == 0) {
			if (result_ary[2][j] != 0) {
				result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + "," + to_string(result_ary[2][j]) + " ";
			}
		} else {
			result += to_string(result_ary[0][j]) + "," + to_string(result_ary[1][j]) + ",0 ";
		}
	}

	return result;
}


string MHLZ77DeCompress(string input) {
	string result;
	vector<string> s_input = splitString(input, ' ');

	for (int i = 0; i < s_input.size(); ++i) {
		vector<string> ss_input = splitString(s_input[i], ',');

		int p = stoi(ss_input[0]);
		int l = stoi(ss_input[1]);
		string ch = " ";
		
		if (ss_input[2][0] != '0')
			ch = ss_input[2];

		if (p != 0) {
			int result_len = (int)result.length();
			for (int x = 0; x < l; x++)
				result += result[result_len - p + x];
		}

		if (ch[0] != '0' || ch[0] != 0)
			result += ch;
	}
	return result;
}