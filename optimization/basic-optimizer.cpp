#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

// Three Address Code instruction
struct TAC {
    string result;
    string op1;
    string operator_;
    string op2;
    
    TAC(string r, string o1, string op, string o2) 
        : result(r), op1(o1), operator_(op), op2(o2) {}
    
    void print() {
        cout << result << " = " << op1 << " " << operator_ << " " << op2 << endl;
    }
};

class BasicOptimizer {
private:
    vector<TAC> instructions;
    
public:
    void addInstruction(string result, string op1, string op, string op2) {
        instructions.push_back(TAC(result, op1, op, op2));
    }
    
    // Constant folding optimization
    void constantFolding() {
        cout << "\n--- Constant Folding ---" << endl;
        
        for (auto& instr : instructions) {
            if (isNumber(instr.op1) && isNumber(instr.op2)) {
                int val1 = stoi(instr.op1);
                int val2 = stoi(instr.op2);
                int result = 0;
                
                if (instr.operator_ == "+") result = val1 + val2;
                else if (instr.operator_ == "-") result = val1 - val2;
                else if (instr.operator_ == "*") result = val1 * val2;
                else if (instr.operator_ == "/") result = val1 / val2;
                
                cout << "Optimized: " << instr.result << " = " << instr.op1 
                     << " " << instr.operator_ << " " << instr.op2 
                     << " -> " << instr.result << " = " << result << endl;
                
                // Update instruction
                instr.op1 = to_string(result);
                instr.operator_ = "";
                instr.op2 = "";
            }
        }
    }
    
    // Dead code elimination (simple version)
    void deadCodeElimination() {
        cout << "\n--- Dead Code Elimination ---" << endl;
        
        vector<TAC> optimized;
        
        for (auto& instr : instructions) {
            // Check if result is used later
            bool isUsed = false;
            for (int i = 0; i < instructions.size(); i++) {
                if (instructions[i].op1 == instr.result || 
                    instructions[i].op2 == instr.result) {
                    isUsed = true;
                    break;
                }
            }
            
            if (isUsed || instr.result == "output") { // Keep if used or is output
                optimized.push_back(instr);
            } else {
                cout << "Removed dead code: ";
                instr.print();
            }
        }
        
        instructions = optimized;
    }
    
    // Algebraic simplification
    void algebraicSimplification() {
        cout << "\n--- Algebraic Simplification ---" << endl;
        
        for (auto& instr : instructions) {
            // x + 0 = x or 0 + x = x
            if (instr.operator_ == "+" && (instr.op1 == "0" || instr.op2 == "0")) {
                string nonZero = (instr.op1 == "0") ? instr.op2 : instr.op1;
                cout << "Optimized: " << instr.result << " = " << instr.op1 
                     << " + " << instr.op2 << " -> " << instr.result << " = " << nonZero << endl;
                instr.op1 = nonZero;
                instr.operator_ = "";
                instr.op2 = "";
            }
            // x * 1 = x or 1 * x = x
            else if (instr.operator_ == "*" && (instr.op1 == "1" || instr.op2 == "1")) {
                string nonOne = (instr.op1 == "1") ? instr.op2 : instr.op1;
                cout << "Optimized: " << instr.result << " = " << instr.op1 
                     << " * " << instr.op2 << " -> " << instr.result << " = " << nonOne << endl;
                instr.op1 = nonOne;
                instr.operator_ = "";
                instr.op2 = "";
            }
            // x * 0 = 0 or 0 * x = 0
            else if (instr.operator_ == "*" && (instr.op1 == "0" || instr.op2 == "0")) {
                cout << "Optimized: " << instr.result << " = " << instr.op1 
                     << " * " << instr.op2 << " -> " << instr.result << " = 0" << endl;
                instr.op1 = "0";
                instr.operator_ = "";
                instr.op2 = "";
            }
        }
    }
    
    void displayInstructions(string title) {
        cout << "\n--- " << title << " ---" << endl;
        for (auto& instr : instructions) {
            instr.print();
        }
    }
    
private:
    bool isNumber(const string& str) {
        return !str.empty() && str.find_first_not_of("0123456789") == string::npos;
    }
};

int main() {
    BasicOptimizer optimizer;
    
    // Sample TAC instructions
    optimizer.addInstruction("t1", "3", "+", "5");     // Constant folding
    optimizer.addInstruction("t2", "x", "+", "0");     // Algebraic simplification
    optimizer.addInstruction("t3", "y", "*", "1");     // Algebraic simplification
    optimizer.addInstruction("t4", "z", "*", "0");     // Algebraic simplification
    optimizer.addInstruction("t5", "a", "+", "b");     // Normal operation
    optimizer.addInstruction("output", "t5", "", "");   // Used result
    
    optimizer.displayInstructions("Original Code");
    
    // Apply optimizations
    optimizer.constantFolding();
    optimizer.algebraicSimplification();
    optimizer.deadCodeElimination();
    
    optimizer.displayInstructions("Optimized Code");
    
    return 0;
}