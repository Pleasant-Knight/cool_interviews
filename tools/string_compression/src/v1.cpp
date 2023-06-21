#include "Compress.h"
#include "Timer.h"
#include <queue>
#include <cstdlib>


using namespace std;

bool AddNode(Node** head, Node* n) {
	if (n == nullptr || (head == nullptr) || (*head == nullptr))
		return false;
	Node* h = *head;
	while (h->next != nullptr)
		h = h->next;
	// add the actual node.
	h->next = n;

	return true;
}

int main(int argc, char* argv[]) {
	int ret = 0;
	Node* head = new Node(0, nullptr);

	// Create 10 temp nodes.
	for (int i = 0; i < 10; i++) {
		Node n((rand() % 100), nullptr);
		AddNode(&head, &n);
	}

	for (const auto & i : CompressionMethod()) {
		Timer t1;
		string fileName = "./input.txt";

		ret = compressDecomress(true, i, fileName);

		cout << "Got file size: " << ret << " for compression method ";
		cout << CompressionMethod_toString(i)  << "\n";
	}
	
	return ret;
}
