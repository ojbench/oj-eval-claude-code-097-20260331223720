#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
private:
    int result;
    int count;

    void reverseInorder(TreeNode* root, int k) {
        if (root == nullptr || count >= k) {
            return;
        }

        // Visit right subtree first (larger values)
        reverseInorder(root->right, k);

        // Visit current node
        count++;
        if (count == k) {
            result = root->val;
            return;
        }

        // Visit left subtree (smaller values)
        reverseInorder(root->left, k);
    }

public:
    int kthLargest(TreeNode* root, int k) {
        result = 0;
        count = 0;
        reverseInorder(root, k);
        return result;
    }
};

TreeNode* buildTreeFromLevelOrder(const vector<int>& nums) {
    if (nums.empty() || nums[0] == -1) {
        return nullptr;
    }

    TreeNode* root = new TreeNode(nums[0]);
    queue<TreeNode*> q;
    q.push(root);
    int i = 1;

    while (!q.empty() && i < nums.size()) {
        TreeNode* node = q.front();
        q.pop();

        // Left child
        if (i < nums.size() && nums[i] != -1) {
            node->left = new TreeNode(nums[i]);
            q.push(node->left);
        }
        i++;

        // Right child
        if (i < nums.size() && nums[i] != -1) {
            node->right = new TreeNode(nums[i]);
            q.push(node->right);
        }
        i++;
    }

    return root;
}

void deleteTree(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    string line;
    getline(cin, line);

    // Parse the input
    // Format: root = [12, 5, 18, 2, 9, 15, 20], cnt = 4
    size_t rootPos = line.find("root = [");
    size_t cntPos = line.find("], cnt = ");

    if (rootPos == string::npos || cntPos == string::npos) {
        cerr << "Invalid input format" << endl;
        return 1;
    }

    // Extract the array part
    size_t arrayStart = rootPos + 8; // length of "root = ["
    size_t arrayEnd = cntPos;
    string arrayStr = line.substr(arrayStart, arrayEnd - arrayStart);

    // Extract cnt
    size_t cntStart = cntPos + 9; // length of "], cnt = "
    int cnt = stoi(line.substr(cntStart));

    // Parse the array
    vector<int> nums;
    stringstream ss(arrayStr);
    string token;

    while (getline(ss, token, ',')) {
        // Trim whitespace
        size_t start = token.find_first_not_of(" \t\n\r");
        size_t end = token.find_last_not_of(" \t\n\r");
        if (start != string::npos && end != string::npos) {
            token = token.substr(start, end - start + 1);
        }

        if (token == "null" || token == "None") {
            nums.push_back(-1); // Use -1 to represent null
        } else {
            nums.push_back(stoi(token));
        }
    }

    // Build tree and find kth largest
    TreeNode* root = buildTreeFromLevelOrder(nums);
    Solution solution;
    int result = solution.kthLargest(root, cnt);

    cout << result << endl;

    // Clean up
    deleteTree(root);

    return 0;
}
