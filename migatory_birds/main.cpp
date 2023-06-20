#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Prototype for split func.
vector<string> split(const string& str);

int migratoryBirds(vector<int>& arr) {
    if (arr.empty())
        return 0;
    vector<long long> ht(6, 0UL);

    for (auto elem : arr) {
        if (elem > 5)
            continue;
        ht[elem]++;
    }

    int Max = -1, maxIdx = 0;
    for (size_t i = 1; i < ht.size(); i++) {
        if (ht[i] > Max) {
            Max = ht[i];
            maxIdx = i;
        }
    }

    return maxIdx;
}


int main() {

    cout << "\nSolvig Hacker Rank https://www.hackerrank.com/challenges/migratory-birds/problem\n\n";
    cout << "\nSplit input file: ./input.txt file ...\n";

    string arr_temp_temp;
    ifstream myfile("input.txt");

    int arr_count = 124992;

    getline(myfile, arr_temp_temp);

    vector<string> arr_temp = split(arr_temp_temp);

    vector<int> arr(arr_count);

    for (int i = 0; i < arr_count; i++) {
        int arr_item = stoi(arr_temp[i]);
        arr[i] = arr_item;
    }

    int result = migratoryBirds(arr);

    cout << "\nMax Results: " << result << endl;
    return 0;
}

vector<string> split(const string& str) {
    vector<string> tokens;
    string::size_type start = 0, end = 0;

    while ((end = str.find(" ", start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(str.substr(start));

    return tokens;
}
