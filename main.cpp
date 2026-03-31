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
        if (i < nums.size()) {
            if (nums[i] != -1) {
                node->left = new TreeNode(nums[i]);
                q.push(node->left);
            }
            i++;
        }

        // Right child
        if (i < nums.size()) {
            if (nums[i] != -1) {
                node->right = new TreeNode(nums[i]);
                q.push(node->right);
            }
            i++;
        }
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

// Parse integers from a string, handling various formats
vector<int> parseIntegers(const string& str) {
    vector<int> result;
    stringstream ss;
    string cleaned;

    // Remove brackets and other non-numeric characters (except minus, comma, space)
    for (char c : str) {
        if (isdigit(c) || c == '-' || c == ' ' || c == ',') {
            cleaned += c;
        }
    }

    ss.str(cleaned);
    string token;

    // Try comma-separated first
    if (cleaned.find(',') != string::npos) {
        while (getline(ss, token, ',')) {
            size_t start = token.find_first_not_of(" \t\n\r");
            if (start != string::npos) {
                size_t end = token.find_last_not_of(" \t\n\r");
                token = token.substr(start, end - start + 1);
                try {
                    result.push_back(stoi(token));
                } catch (...) {}
            }
        }
    } else {
        // Space-separated
        int num;
        while (ss >> num) {
            result.push_back(num);
        }
    }

    return result;
}

int main() {
    string line;

    // Read first line
    if (!getline(cin, line)) {
        return 0;
    }

    vector<int> nums;
    int cnt = 0;

    // Check for "root = [...]" format
    size_t rootPos = line.find("root");
    size_t cntTextPos = line.find("cnt");

    if (rootPos != string::npos && cntTextPos != string::npos) {
        // Extract array part (between root and cnt)
        string arrayPart = line.substr(rootPos, cntTextPos - rootPos);
        nums = parseIntegers(arrayPart);

        // Extract cnt value
        string cntPart = line.substr(cntTextPos);
        vector<int> cntVec = parseIntegers(cntPart);
        if (!cntVec.empty()) {
            cnt = cntVec[0];
        }
    } else {
        // Try to parse as array
        nums = parseIntegers(line);

        // If we found numbers and there's at least 2, last one might be cnt
        if (nums.size() >= 2) {
            // Check if there's a second line
            string line2;
            if (getline(cin, line2)) {
                vector<int> cntVec = parseIntegers(line2);
                if (!cntVec.empty()) {
                    cnt = cntVec[0];
                } else {
                    // Maybe cnt is on same line, separated somehow
                    // Last number is cnt, rest is array
                    cnt = nums.back();
                    nums.pop_back();
                }
            } else {
                // No second line, assume last number is cnt
                cnt = nums.back();
                nums.pop_back();
            }
        } else if (nums.size() == 1) {
            // Only one number on first line, must be array start or complete
            string line2;
            if (getline(cin, line2)) {
                vector<int> secondLine = parseIntegers(line2);
                if (secondLine.size() == 1) {
                    // First line: single element array, second line: cnt
                    cnt = secondLine[0];
                } else {
                    // Append to array
                    nums.insert(nums.end(), secondLine.begin(), secondLine.end());
                    // Try to read cnt
                    string line3;
                    if (getline(cin, line3)) {
                        vector<int> thirdLine = parseIntegers(line3);
                        if (!thirdLine.empty()) {
                            cnt = thirdLine[0];
                        }
                    }
                }
            }
        }
    }

    if (nums.empty() || cnt <= 0) {
        return 0;
    }

    // Build tree and find kth largest
    TreeNode* root = buildTreeFromLevelOrder(nums);
    if (root == nullptr) {
        return 0;
    }

    Solution solution;
    int result = solution.kthLargest(root, cnt);

    cout << result << endl;

    // Clean up
    deleteTree(root);

    return 0;
}
