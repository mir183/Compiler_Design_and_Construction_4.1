#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <vector>
#include <unordered_set>
using namespace std;

// List of keywords
unordered_set<string> keywords = {"int", "float", "if", "else", "while", "return", "for"};

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '<' || ch == '>';
}

bool isSeparator(char ch) {
    return ch == ';' || ch == ',' || ch == '(' || ch == ')' || ch == '{' || ch == '}';
}

void tokenize(string line) {
    string token;
    for (int i = 0; i < line.length(); i++) {
        char ch = line[i];
        
        if (isspace(ch)) {
            continue;
        }
        
        if (isalpha(ch) || ch == '_') {
            token = "";
            while (isalnum(line[i]) || line[i] == '_') {
                token += line[i++];
            }
            i--;
            
            if (keywords.count(token))
                cout << token << " : Keyword" << endl;
            else
                cout << token << " : Identifier" << endl;
        }
        else if (isdigit(ch)) {
            token = "";
            while (isdigit(line[i])) {
                token += line[i++];
            }
            i--;
            cout << token << " : Number" << endl;
        }
        else if (isOperator(ch)) {
            cout << ch << " : Operator" << endl;
        }
        else if (isSeparator(ch)) {
            cout << ch << " : Separator" << endl;
        }
        else {
            cout << ch << " : Unknown Token" << endl;
        }
    }
}

int main() {
    string code = "int a = 5;\nfloat b = 3.5;\nif (a > b) { return a; }";
    stringstream ss(code);
    string line;
    
    while (getline(ss, line)) {
        tokenize(line);
    }
    
    return 0;
}