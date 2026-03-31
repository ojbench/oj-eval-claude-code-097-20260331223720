#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
#include <algorithm>

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

        reverseInorder(root->right, k);

        count++;
        if (count == k) {
            result = root->val;
            return;
        }

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

TreeNode* buildTreeFromLevelOrder(const vector<string>& tokens) {
    if (tokens.empty() || tokens[0] == "null" || tokens[0] == "None") {
        return nullptr;
    }

    TreeNode* root = new TreeNode(stoi(tokens[0]));
    queue<TreeNode*> q;
    q.push(root);
    int i = 1;

    while (!q.empty() && i < tokens.size()) {
        TreeNode* node = q.front();
        q.pop();

        // Left child
        if (i < tokens.size()) {
            if (tokens[i] != "null" && tokens[i] != "None" && tokens[i] != "NULL") {
                node->left = new TreeNode(stoi(tokens[i]));
                q.push(node->left);
            }
            i++;
        }

        // Right child
        if (i < tokens.size()) {
            if (tokens[i] != "null" && tokens[i] != "None" && tokens[i] != "NULL") {
                node->right = new TreeNode(stoi(tokens[i]));
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

// Parse tokens (can be numbers or "null")
vector<string> parseTokens(const string& str) {
    vector<string> result;
    stringstream ss(str);
    string token;
    bool inBracket = false;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];

        if (c == '[') {
            inBracket = true;
            continue;
        } else if (c == ']') {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
            break;
        } else if (c == ',' || c == ' ' || c == '\t') {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }

    if (!token.empty()) {
        result.push_back(token);
    }

    return result;
}

int extractInt(const string& str) {
    stringstream ss;
    for (char c : str) {
        if (isdigit(c) || c == '-') {
            ss << c;
        }
    }
    int result;
    if (ss >> result) {
        return result;
    }
    return 0;
}

int main() {
    string line;
    if (!getline(cin, line)) {
        return 0;
    }

    vector<string> tokens;
    int cnt = 0;

    // Try to find array and cnt in the line
    size_t bracketStart = line.find('[');
    size_t bracketEnd = line.find(']');

    if (bracketStart != string::npos && bracketEnd != string::npos) {
        // Extract array content
        string arrayContent = line.substr(bracketStart, bracketEnd - bracketStart + 1);
        tokens = parseTokens(arrayContent);

        // Extract cnt
        size_t cntPos = line.find("cnt", bracketEnd);
        if (cntPos != string::npos) {
            cnt = extractInt(line.substr(cntPos));
        } else {
            // Try to read cnt from next line
            string line2;
            if (getline(cin, line2)) {
                cnt = extractInt(line2);
            }
        }
    } else {
        // No brackets, try to read numbers and cnt differently
        tokens = parseTokens(line);
        string line2;
        if (getline(cin, line2)) {
            cnt = extractInt(line2);
        } else if (tokens.size() >= 2) {
            // Last token might be cnt
            try {
                cnt = stoi(tokens.back());
                tokens.pop_back();
            } catch (...) {}
        }
    }

    if (tokens.empty() || cnt <= 0) {
        return 0;
    }

    // Build tree
    TreeNode* root = buildTreeFromLevelOrder(tokens);
    if (root == nullptr) {
        return 0;
    }

    Solution solution;
    int result = solution.kthLargest(root, cnt);

    cout << result << endl;

    deleteTree(root);

    return 0;
}
