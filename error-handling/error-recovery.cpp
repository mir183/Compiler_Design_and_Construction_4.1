#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Error types
enum ErrorType {
    LEXICAL_ERROR,
    SYNTAX_ERROR,
    SEMANTIC_ERROR
};

// Error structure
struct CompilerError {
    ErrorType type;
    string message;
    int line;
    int column;
};

class ErrorHandler {
private:
    vector<CompilerError> errors;
    int errorCount;
    
public:
    ErrorHandler() : errorCount(0) {}
    
    void reportError(ErrorType type, string message, int line = 1, int column = 1) {
        CompilerError error;
        error.type = type;
        error.message = message;
        error.line = line;
        error.column = column;
        
        errors.push_back(error);
        errorCount++;
        
        string typeStr;
        switch(type) {
            case LEXICAL_ERROR: typeStr = "Lexical"; break;
            case SYNTAX_ERROR: typeStr = "Syntax"; break;
            case SEMANTIC_ERROR: typeStr = "Semantic"; break;
        }
        
        cout << typeStr << " Error at line " << line << ", column " << column 
             << ": " << message << endl;
    }
    
    bool hasErrors() {
        return errorCount > 0;
    }
    
    void displaySummary() {
        cout << "\n--- Error Summary ---" << endl;
        cout << "Total errors: " << errorCount << endl;
        
        if (errorCount == 0) {
            cout << "Compilation successful!" << endl;
        } else {
            cout << "Compilation failed with " << errorCount << " error(s)" << endl;
        }
    }
    
    // Simple error recovery - skip to next semicolon
    void recoverToSemicolon(string& input, int& pos) {
        while (pos < input.length() && input[pos] != ';') {
            pos++;
        }
        if (pos < input.length()) pos++; // Skip the semicolon
        cout << "Recovered to next statement" << endl;
    }
};

// Simple parser with error handling
class Parser {
private:
    ErrorHandler& errorHandler;
    string input;
    int pos;
    
public:
    Parser(ErrorHandler& eh) : errorHandler(eh), pos(0) {}
    
    void parse(string code) {
        input = code;
        pos = 0;
        
        while (pos < input.length()) {
            if (!parseStatement()) {
                // Error recovery
                errorHandler.recoverToSemicolon(input, pos);
            }
        }
    }
    
private:
    bool parseStatement() {
        skipWhitespace();
        
        if (pos >= input.length()) return true;
        
        // Simple statement: identifier = number;
        if (!parseIdentifier()) {
            errorHandler.reportError(SYNTAX_ERROR, "Expected identifier", 1, pos);
            return false;
        }
        
        skipWhitespace();
        
        if (pos >= input.length() || input[pos] != '=') {
            errorHandler.reportError(SYNTAX_ERROR, "Expected '='", 1, pos);
            return false;
        }
        pos++; // Skip '='
        
        skipWhitespace();
        
        if (!parseNumber()) {
            errorHandler.reportError(SYNTAX_ERROR, "Expected number", 1, pos);
            return false;
        }
        
        skipWhitespace();
        
        if (pos >= input.length() || input[pos] != ';') {
            errorHandler.reportError(SYNTAX_ERROR, "Expected ';'", 1, pos);
            return false;
        }
        pos++; // Skip ';'
        
        return true;
    }
    
    bool parseIdentifier() {
        if (pos >= input.length() || !isalpha(input[pos])) {
            return false;
        }
        
        while (pos < input.length() && (isalnum(input[pos]) || input[pos] == '_')) {
            pos++;
        }
        return true;
    }
    
    bool parseNumber() {
        if (pos >= input.length() || !isdigit(input[pos])) {
            return false;
        }
        
        while (pos < input.length() && isdigit(input[pos])) {
            pos++;
        }
        return true;
    }
    
    void skipWhitespace() {
        while (pos < input.length() && isspace(input[pos])) {
            pos++;
        }
    }
};

int main() {
    ErrorHandler errorHandler;
    Parser parser(errorHandler);
    
    // Test code with errors
    string code = "x = 5; y = ; z = 10;"; // Missing number after second '='
    
    cout << "Parsing: " << code << endl;
    parser.parse(code);
    
    errorHandler.displaySummary();
    
    return 0;
}