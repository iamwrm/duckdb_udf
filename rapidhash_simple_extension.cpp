#define DUCKDB_EXTENSION_MAIN

#include "duckdb/function/scalar_function.hpp"
#include "duckdb/main/extension_util.hpp"
#include "rapidhash.h"

#include <duckdb.h>
#include <duckdb/common/exception.hpp>
#include <duckdb/common/string_util.hpp>
#include <duckdb/function/scalar_function.hpp>
#include <duckdb/main/extension_util.hpp>

namespace duckdb {

static void RapidHashScalarFunction(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &input = args.data[0];
    UnaryExecutor::Execute<string_t, int64_t>(
        input, result, args.size(),
        [&](string_t input) {
            auto data = input.GetData();
            auto len = input.GetSize();
            uint64_t hash = rapidhash(data, len);
            return (int64_t)hash;
        });
}

static void RapidHashWithSeedScalarFunction(DataChunk &args, ExpressionState &state, Vector &result) {
    auto &input = args.data[0];
    auto &seed_input = args.data[1];
    BinaryExecutor::Execute<string_t, int64_t, int64_t>(
        input, seed_input, result, args.size(),
        [&](string_t input, int64_t seed) {
            auto data = input.GetData();
            auto len = input.GetSize();
            uint64_t hash = rapidhash_withSeed(data, len, (uint64_t)seed);
            return (int64_t)hash;
        });
}

static void LoadInternal(DatabaseInstance &instance) {
    // Register rapidhash() function
    auto rapidhash_function = ScalarFunction("rapidhash", {LogicalType::VARCHAR}, LogicalType::BIGINT, RapidHashScalarFunction);
    ExtensionUtil::RegisterFunction(instance, rapidhash_function);
    
    // Register rapidhash_with_seed() function  
    auto rapidhash_seed_function = ScalarFunction("rapidhash_with_seed", {LogicalType::VARCHAR, LogicalType::BIGINT}, LogicalType::BIGINT, RapidHashWithSeedScalarFunction);
    ExtensionUtil::RegisterFunction(instance, rapidhash_seed_function);
}

void RapidhashExtension::Load(DuckDB &db) {
    LoadInternal(*db.instance);
}

std::string RapidhashExtension::Name() {
    return "rapidhash";
}

} // namespace duckdb

extern "C" {

DUCKDB_EXTENSION_API void rapidhash_init(duckdb::DatabaseInstance &db) {
    LoadInternal(db);
}

DUCKDB_EXTENSION_API const char *rapidhash_version() {
    return DuckDB::LibraryVersion();
}

}