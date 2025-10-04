#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

// Symbol table entry structure
struct Symbol {
    string name;
    string type;
    string scope;
    int address;
    bool initialized;
};

class SymbolTable {
private:
    unordered_map<string, Symbol> table;
    int nextAddress;
    
public:
    SymbolTable() : nextAddress(1000) {}
    
    // Insert a new symbol
    bool insert(string name, string type, string scope = "global") {
        if (table.find(name) != table.end()) {
            cout << "Error: Variable '" << name << "' already declared!" << endl;
            return false;
        }
        
        Symbol sym;
        sym.name = name;
        sym.type = type;
        sym.scope = scope;
        sym.address = nextAddress++;
        sym.initialized = false;
        
        table[name] = sym;
        cout << "Symbol '" << name << "' added to table" << endl;
        return true;
    }
    
    // Lookup a symbol
    Symbol* lookup(string name) {
        auto it = table.find(name);
        if (it != table.end()) {
            return &(it->second);
        }
        return nullptr;
    }
    
    // Display symbol table
    void display() {
        cout << "\n--- Symbol Table ---" << endl;
        cout << "Name\tType\tScope\tAddress\tInitialized" << endl;
        cout << "----\t----\t-----\t-------\t-----------" << endl;
        
        for (auto& pair : table) {
            Symbol& sym = pair.second;
            cout << sym.name << "\t" << sym.type << "\t" << sym.scope 
                 << "\t" << sym.address << "\t" << (sym.initialized ? "Yes" : "No") << endl;
        }
    }
};

int main() {
    SymbolTable st;
    
    // Sample symbol insertions
    st.insert("x", "int");
    st.insert("y", "float");
    st.insert("result", "int");
    st.insert("x", "double"); // This should show error
    
    // Lookup example
    Symbol* sym = st.lookup("x");
    if (sym) {
        cout << "Found symbol: " << sym->name << " of type " << sym->type << endl;
    }
    
    st.display();
    
    return 0;
}