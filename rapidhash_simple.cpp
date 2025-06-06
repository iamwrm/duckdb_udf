#include <iostream>
#include <string>
#include <cstring>
#include "rapidhash.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <string_to_hash>" << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    uint64_t hash = rapidhash(input.c_str(), input.length());
    
    std::cout << "Input: " << input << std::endl;
    std::cout << "RapidHash: " << hash << std::endl;
    
    // Test with seed
    uint64_t hash_with_seed = rapidhash_withSeed(input.c_str(), input.length(), 12345);
    std::cout << "RapidHash with seed 12345: " << hash_with_seed << std::endl;
    
    return 0;
}