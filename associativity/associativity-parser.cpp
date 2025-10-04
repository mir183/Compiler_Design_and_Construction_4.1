#include <iostream>     // For input and output (cin, cout)
#include <vector>       // For using vector (dynamic array) to store tokens
#include <string>       // For handling strings
using namespace std;    // To avoid using std:: prefix everywhere

// Function to break (tokenize) an expression like "a - b - c" into parts: ["a", "-", "b", "-", "c"]
vector<string> tokenize(string expr) {
    vector<string> tokens;  // To store the separated tokens (operands and operators)
    string token = "";      // Temporary string to build a word or number
    
    // Loop through each character in the expression
    for (char ch : expr) {
        if (ch == ' ') continue; // Skip spaces
        
        // If the character is an operator like '-' or '='
        if (ch == '-' || ch == '=') {
            // If we have built an operand before, add it to the token list
            if (!token.empty()) {
                tokens.push_back(token); // e.g., "a"
                token = "";              // Reset token for next one
            }
            // Add the operator as a separate token (convert char to string)
            tokens.push_back(string(1, ch));
        } else {
            // It's part of an operand (e.g., 'a', 'b', 'x', '1')
            token += ch;
        }
    }
    
    // After the loop, add the last token if it's not empty
    if (!token.empty())
        tokens.push_back(token);
    
    return tokens; // Return the list of tokens
}

// Function to simulate left-associative parsing
// Example: a - b - c → ((a - b) - c)
string parseLeftAssociative(const vector<string>& tokens) {
    string result = tokens[0]; // Start with the first operand
    
    // Loop through the tokens: operator is at index i, next operand at i+1
    for (int i = 1; i < tokens.size(); i += 2) {
        string op = tokens[i];       // Operator like "-"
        string next = tokens[i + 1]; // Next operand like "b" or "c"
        
        // Group the current expression with the next part using parentheses
        result = "(" + result + " " + op + " " + next + ")";
    }
    
    return result; // Return the final expression string
}

// Function to simulate right-associative parsing using recursion
// Example: a = b = c → a = (b = c)
string parseRightAssociative(const vector<string>& tokens, int i = 0) {
    // Base case: if no more operator-operand pairs left
    if (i + 2 >= tokens.size())
        return tokens[i]; // Return the final operand
    
    // Recursively parse the right side first
    string right = parseRightAssociative(tokens, i + 2);
    
    // Combine the current operand, operator, and the right result
    return tokens[i] + " " + tokens[i + 1] + " (" + right + ")";
}

// Main function: Entry point of the program
int main() {
    // Define a left-associative expression
    string leftExpr = "a - b - c";
    // Define a right-associative expression
    string rightExpr = "a = b = c";
    
    // Tokenize both expressions
    vector<string> leftTokens = tokenize(leftExpr);
    vector<string> rightTokens = tokenize(rightExpr);
    
    // Show original left-associative expression and its parsed result
    cout << "Left-Associative Expression: " << leftExpr << endl;
    cout << "Parsed (Left-Associative): " << parseLeftAssociative(leftTokens) << endl;
    
    // Print a line separator
    cout << "--------------------------------------------\n";
    
    // Show original right-associative expression and its parsed result
    cout << "Right-Associative Expression: " << rightExpr << endl;
    cout << "Parsed (Right-Associative): " << parseRightAssociative(rightTokens) << endl;
    
    return 0; // End of program
}