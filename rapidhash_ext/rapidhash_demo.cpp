#include "rapidhash_functions.h"

extern "C" {

DUCKDB_EXTENSION_API void rapidhash_init(duckdb_database database) {
    // Initialize extension
}

DUCKDB_EXTENSION_API void rapidhash_load(duckdb_connection connection) {
    RegisterRapidHashFunctions(connection);
}

DUCKDB_EXTENSION_API const char* rapidhash_version() {
    return "1.0.0";
}

}