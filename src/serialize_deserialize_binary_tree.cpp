/*
Serialize and Deserialize Binary Tree

Problem: Design serialize(root) to convert a binary tree to a string, and deserialize(data) to reconstruct the tree from the string.
Use preorder traversal.
Constraints: Tree nodes have int values (-1000 to 1000); up to 10^4 nodes.
C++ Focus: 
Define a TreeNode struct. Use std::string with delimiters (e.g., commas, "null" for missing nodes).
Parse with std::istringstream. Recursion for tree ops.
Time to Solve: 25-35 minutes.
Follow-up: Optimize for level-order traversal or handle larger trees with queues.

API:
    std::string serialize(TreeNode* root) → one string that fully captures the tree.
    TreeNode* deserialize(const std::string& data) → reconstructs an identical tree.
Constraints
    Nodes have int val, left, right.
Up to 10⁴ nodes, values -1000 … 1000.
No cycles, no parent pointers.


Sample Input/Output:cpp// Tree: [1,2,3,null,null,4,5]
serialize(root);  // "1,2,null,null,3,4,null,null,5,null,null"
deserialize reconstructs the tree
*/

#include<stringstream>
#include<string>

using namespace std;
/**
 * Definition for a binary tree node.
 */
 struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
  };
 
class Codec {
    ostringstream out;
    
    void serializeRec(TreeNode* root) {
        if (root) {
            out << root->val << ' ';
            serializeRec(root->left);
            serializeRec(root->right);
        } else {
            out << "# ";
        }
    }

    TreeNode* deserialize(istringstream& in) {
        string val;
        in >> val;
        if (val == "#")
            return nullptr;
        TreeNode* root = new TreeNode(stoi(val));
        root->left = deserialize(in);
        root->right = deserialize(in);
        
        return root;
    }

    public:
    string serialize(TreeNode* root) {        
        serializeRec(root);
        return out.str();
    }

    TreeNode* deserialize(string data) {
        istringstream in(data);
        return deserialize(in);
    }
};

int main() {
    // Your Codec object will be instantiated and called as such:
    Codec codec;
    TreeNode* tree = new TreeNode(1);
    tree->left = new TreeNode(2);
    tree->right = new TreeNode(3);
    tree->right->left = new TreeNode(4);
    tree->right->right = new TreeNode(5);

    string serialized = codec.serialize(tree);
    TreeNode* deserializedTree = codec.deserialize(serialized);

    return 0;
}