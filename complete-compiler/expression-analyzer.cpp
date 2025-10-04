#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cctype>
using namespace std;

// Token structure
struct Token {
    string value;
    string type;
};

// Tokenizer function
vector<Token> tokenize(string expression) {
    vector<Token> tokens;
    string current = "";
    
    for (int i = 0; i < expression.length(); i++) {
        char ch = expression[i];
        
        if (ch == ' ') continue;
        
        if (isalnum(ch)) {
            current += ch;
        } else {
            if (!current.empty()) {
                Token token;
                token.value = current;
                token.type = isdigit(current[0]) ? "NUMBER" : "IDENTIFIER";
                tokens.push_back(token);
                current = "";
            }
            
            if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                Token token;
                token.value = ch;
                token.type = "OPERATOR";
                tokens.push_back(token);
            } else if (ch == '(' || ch == ')') {
                Token token;
                token.value = ch;
                token.type = "PARENTHESIS";
                tokens.push_back(token);
            }
        }
    }
    
    if (!current.empty()) {
        Token token;
        token.value = current;
        token.type = isdigit(current[0]) ? "NUMBER" : "IDENTIFIER";
        tokens.push_back(token);
    }
    
    return tokens;
}

// Syntax checker using simple recursive descent
bool isValidExpression(const vector<Token>& tokens) {
    if (tokens.empty()) return false;
    
    // Simple validation: alternating operands and operators
    bool expectOperand = true;
    int parenthesesCount = 0;
    
    for (const Token& token : tokens) {
        if (token.type == "IDENTIFIER" || token.type == "NUMBER") {
            if (!expectOperand) return false;
            expectOperand = false;
        } else if (token.type == "OPERATOR") {
            if (expectOperand) return false;
            expectOperand = true;
        } else if (token.value == "(") {
            parenthesesCount++;
            expectOperand = true;
        } else if (token.value == ")") {
            parenthesesCount--;
            if (parenthesesCount < 0) return false;
            expectOperand = false;
        }
    }
    
    return !expectOperand && parenthesesCount == 0;
}

// Precedence function
int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Convert to postfix notation
string infixToPostfix(const vector<Token>& tokens) {
    string postfix = "";
    stack<char> operators;
    
    for (const Token& token : tokens) {
        if (token.type == "IDENTIFIER" || token.type == "NUMBER") {
            postfix += token.value + " ";
        } else if (token.value == "(") {
            operators.push('(');
        } else if (token.value == ")") {
            while (!operators.empty() && operators.top() != '(') {
                postfix += operators.top();
                postfix += " ";
                operators.pop();
            }
            if (!operators.empty()) operators.pop(); // Remove '('
        } else if (token.type == "OPERATOR") {
            char op = token.value[0];
            while (!operators.empty() && operators.top() != '(' && 
                   precedence(operators.top()) >= precedence(op)) {
                postfix += operators.top();
                postfix += " ";
                operators.pop();
            }
            operators.push(op);
        }
    }
    
    while (!operators.empty()) {
        postfix += operators.top();
        postfix += " ";
        operators.pop();
    }
    
    return postfix;
}

int main() {
    string expression;
    cout << "Enter an arithmetic expression: ";
    getline(cin, expression);
    
    // Step 1: Tokenization
    vector<Token> tokens = tokenize(expression);
    
    cout << "\nTokens: [";
    for (int i = 0; i < tokens.size(); i++) {
        cout << tokens[i].value;
        if (i < tokens.size() - 1) cout << ", ";
    }
    cout << "]" << endl;
    
    // Step 2: Syntax Analysis
    bool isValid = isValidExpression(tokens);
    cout << "Syntax: " << (isValid ? "Valid" : "Invalid") << endl;
    
    // Step 3: Convert to Postfix (if valid)
    if (isValid) {
        string postfix = infixToPostfix(tokens);
        cout << "Postfix: " << postfix << endl;
    }
    
    return 0;
}