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

vector<int> parseArray(const string& str) {
    vector<int> result;
    stringstream ss(str);
    string token;

    while (getline(ss, token, ',')) {
        // Trim whitespace
        size_t start = token.find_first_not_of(" \t\n\r[]");
        size_t end = token.find_last_not_of(" \t\n\r[]");

        if (start != string::npos && end != string::npos) {
            token = token.substr(start, end - start + 1);

            if (token == "null" || token == "None" || token.empty()) {
                result.push_back(-1);
            } else {
                result.push_back(stoi(token));
            }
        }
    }

    return result;
}

int main() {
    string line;

    // Try to read all input
    if (!getline(cin, line)) {
        return 1;
    }

    // Try different input formats
    vector<int> nums;
    int cnt = 0;

    // Format 1: root = [12, 5, 18, 2, 9, 15, 20], cnt = 4
    size_t rootPos = line.find("root = [");
    size_t cntPos = line.find("], cnt = ");

    if (rootPos != string::npos && cntPos != string::npos) {
        size_t arrayStart = rootPos + 8;
        size_t arrayEnd = cntPos;
        string arrayStr = line.substr(arrayStart, arrayEnd - arrayStart);
        nums = parseArray(arrayStr);

        size_t cntStart = cntPos + 9;
        cnt = stoi(line.substr(cntStart));
    }
    // Format 2: Simple format - just array on first line, cnt on second
    else {
        // Check if there's a bracket
        size_t bracketStart = line.find('[');
        size_t bracketEnd = line.find(']');

        if (bracketStart != string::npos && bracketEnd != string::npos) {
            string arrayStr = line.substr(bracketStart + 1, bracketEnd - bracketStart - 1);
            nums = parseArray(arrayStr);

            // Look for cnt in the same line or next line
            size_t cntPos2 = line.find("cnt");
            if (cntPos2 != string::npos) {
                size_t numStart = line.find_first_of("0123456789", cntPos2);
                if (numStart != string::npos) {
                    cnt = stoi(line.substr(numStart));
                }
            } else {
                // Read cnt from next line
                string line2;
                if (getline(cin, line2)) {
                    stringstream ss(line2);
                    ss >> cnt;
                }
            }
        }
        // Format 3: Array elements on first line, cnt on second
        else {
            nums = parseArray(line);
            string line2;
            if (getline(cin, line2)) {
                stringstream ss(line2);
                ss >> cnt;
            }
        }
    }

    if (nums.empty() || cnt == 0) {
        return 1;
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
