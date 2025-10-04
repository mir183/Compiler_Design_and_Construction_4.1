#include <iostream>   // For input and output
#include <stack>      // For using stack data structure
#include <string>     // For using strings
#include <cctype>     // For character checking (like isalnum)
using namespace std;

// Global counter to generate temporary variable names like t1, t2, ...
int tempCount = 1;

// Function to get the precedence of operators
int precedence(char op) {
    if (op == '*' || op == '/') return 2;  // Higher precedence
    if (op == '+' || op == '-') return 1;  // Lower precedence
    return 0;                              // Lowest precedence (like '=')
}

// Function to convert infix expression to postfix
string infixToPostfix(string infix) {
    string postfix = "";           // Result postfix expression
    stack<char> st;                // Stack to hold operators and parentheses
    
    // Loop through each character in the infix expression
    for (char ch : infix) {
        if (ch == ' ') continue;   // Skip spaces
        
        if (isalnum(ch)) {
            // If it's a letter or number (operand), add to postfix
            postfix += ch;
        }
        else if (ch == '(') {
            // If opening parenthesis, push to stack
            st.push(ch);
        }
        else if (ch == ')') {
            // Pop everything until matching '('
            while (!st.empty() && st.top() != '(') {
                postfix += st.top();
                st.pop();
            }
            if (!st.empty()) st.pop();  // Remove '(' from stack
        }
        else {
            // Operator case (+, -, *, /)
            while (!st.empty() && precedence(st.top()) >= precedence(ch)) {
                // Pop operators from stack with higher or equal precedence
                postfix += st.top();
                st.pop();
            }
            st.push(ch);  // Push current operator to stack
        }
    }
    
    // Pop any remaining operators from the stack
    while (!st.empty()) {
        postfix += st.top();
        st.pop();
    }
    
    return postfix;  // Return the postfix expression
}

// Function to generate a new temporary variable (t1, t2, t3, ...)
string newTemp() {
    return "t" + to_string(tempCount++);  // e.g., returns "t1", "t2", ...
}

// Function to generate Three Address Code from postfix expression
string generateTAC(string postfix) {
    stack<string> operands;  // Stack to hold operands and temporary variables
    
    // Loop through each character in the postfix expression
    for (char ch : postfix) {
        if (isalnum(ch)) {
            // If it's an operand, push it as a string to the stack
            operands.push(string(1, ch));
        }
        else {
            // If it's an operator, pop two operands from the stack
            string op2 = operands.top(); operands.pop();  // Second operand
            string op1 = operands.top(); operands.pop();  // First operand
            
            string temp = newTemp();  // Create new temporary variable
            
            // Print the TAC instruction
            cout << temp << " = " << op1 << " " << ch << " " << op2 << endl;
            
            // Push the result (temp) back to the stack
            operands.push(temp);
        }
    }
    
    // Return the final temporary variable holding the result
    return operands.top();
}

// Main function
int main() {
    string infix;  // To store the full input expression (e.g., a = b + c * d)
    
    // Prompt the user to enter the expression
    cout << "Enter an infix expression (e.g., a = b + c * d): ";
    getline(cin, infix);  // Read full line including spaces
    
    // Find the '=' sign position to separate LHS and RHS
    size_t pos = infix.find('=');
    
    // Basic check: ensure '=' is found and LHS & RHS are valid
    if (pos == string::npos || pos == 0 || pos == infix.size() - 1) {
        cout << "Invalid assignment expression!" << endl;
        return 1;  // Exit with error
    }
    
    // Extract the left-hand side variable (e.g., 'a')
    string lhs = string(1, infix[0]);
    
    // Extract the right-hand side expression (e.g., 'b + c * d')
    string rhs = infix.substr(pos + 1);
    
    // Convert RHS from infix to postfix
    string postfix = infixToPostfix(rhs);
    
    // Generate TAC for the postfix expression
    string result = generateTAC(postfix);
    
    // Print the final assignment statement
    cout << lhs << " = " << result << endl;
    
    return 0;  // End of program
}