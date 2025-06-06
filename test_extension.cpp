#include <iostream>
#include <duckdb.hpp>

using namespace duckdb;
using namespace std;

int main() {
    DuckDB db(nullptr);
    Connection con(db);
    
    try {
        // Load the extension
        con.Query("LOAD 'rapidhash.duckdb_extension'");
        cout << "Extension loaded successfully!" << endl;
        
        // Test rapidhash function
        auto result = con.Query("SELECT rapidhash('Hello World') as hash");
        if (result->HasError()) {
            cerr << "Error: " << result->GetError() << endl;
            return 1;
        }
        
        result->Print();
        
        // Test with seed
        result = con.Query("SELECT rapidhash_with_seed('Hello World', 12345) as seeded_hash");
        if (result->HasError()) {
            cerr << "Error: " << result->GetError() << endl;
            return 1;
        }
        
        result->Print();
        
    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}