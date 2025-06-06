#include "rapidhash_functions.h"
#include "duckdb_extension.h"
#include "../rapidhash.h"

DUCKDB_EXTENSION_EXTERN

// Scalar function that computes rapid hash of a string
static void RapidHashFunction(duckdb_function_info info, duckdb_data_chunk input, duckdb_vector output) {
    // get the total number of rows in this chunk
    idx_t input_size = duckdb_data_chunk_get_size(input);
    
    // extract the input vector (string)
    duckdb_vector input_vector = duckdb_data_chunk_get_vector(input, 0);
    
    // get the data pointers
    auto string_data = (duckdb_string_t *)duckdb_vector_get_data(input_vector);
    auto result_data = (int64_t *)duckdb_vector_get_data(output);
    
    // get validity vectors
    auto input_validity = duckdb_vector_get_validity(input_vector);
    
    if (input_validity) {
        // there might be NULL values
        duckdb_vector_ensure_validity_writable(output);
        auto result_validity = duckdb_vector_get_validity(output);
        
        for (idx_t row = 0; row < input_size; row++) {
            if (duckdb_validity_row_is_valid(input_validity, row)) {
                // get the string data for this row
                duckdb_string_t str = string_data[row];
                const char* data = duckdb_string_t_data(&str);
                uint32_t length = duckdb_string_t_length(str);
                
                // compute rapid hash
                uint64_t hash = rapidhash(data, length);
                result_data[row] = (int64_t)hash;
            } else {
                // input is NULL - set the result row to NULL
                duckdb_validity_set_row_invalid(result_validity, row);
            }
        }
    } else {
        // no NULL values - iterate and do the operation directly
        for (idx_t row = 0; row < input_size; row++) {
            // get the string data for this row
            duckdb_string_t str = string_data[row];
            const char* data = duckdb_string_t_data(&str);
            uint32_t length = duckdb_string_t_length(str);
            
            // compute rapid hash
            uint64_t hash = rapidhash(data, length);
            result_data[row] = (int64_t)hash;
        }
    }
}

// Scalar function that computes rapid hash of a string with seed
static void RapidHashWithSeedFunction(duckdb_function_info info, duckdb_data_chunk input, duckdb_vector output) {
    // get the total number of rows in this chunk
    idx_t input_size = duckdb_data_chunk_get_size(input);
    
    // extract the input vectors
    duckdb_vector input_vector = duckdb_data_chunk_get_vector(input, 0);
    duckdb_vector seed_vector = duckdb_data_chunk_get_vector(input, 1);
    
    // get the data pointers
    auto string_data = (duckdb_string_t *)duckdb_vector_get_data(input_vector);
    auto result_data = (int64_t *)duckdb_vector_get_data(output);
    auto seed_data = (int64_t *)duckdb_vector_get_data(seed_vector);
    
    // get validity vectors
    auto input_validity = duckdb_vector_get_validity(input_vector);
    auto seed_validity = duckdb_vector_get_validity(seed_vector);
    
    if (input_validity || seed_validity) {
        // there might be NULL values
        duckdb_vector_ensure_validity_writable(output);
        auto result_validity = duckdb_vector_get_validity(output);
        
        for (idx_t row = 0; row < input_size; row++) {
            if (duckdb_validity_row_is_valid(input_validity, row) && 
                duckdb_validity_row_is_valid(seed_validity, row)) {
                // get the string data for this row
                duckdb_string_t str = string_data[row];
                const char* data = duckdb_string_t_data(&str);
                uint32_t length = duckdb_string_t_length(str);
                uint64_t seed = (uint64_t)seed_data[row];
                
                // compute rapid hash with seed
                uint64_t hash = rapidhash_withSeed(data, length, seed);
                result_data[row] = (int64_t)hash;
            } else {
                // either input or seed is NULL - set the result row to NULL
                duckdb_validity_set_row_invalid(result_validity, row);
            }
        }
    } else {
        // no NULL values - iterate and do the operation directly
        for (idx_t row = 0; row < input_size; row++) {
            // get the string data for this row
            duckdb_string_t str = string_data[row];
            const char* data = duckdb_string_t_data(&str);
            uint32_t length = duckdb_string_t_length(str);
            uint64_t seed = (uint64_t)seed_data[row];
            
            // compute rapid hash with seed
            uint64_t hash = rapidhash_withSeed(data, length, seed);
            result_data[row] = (int64_t)hash;
        }
    }
}

// Register the RapidHash functions
void RegisterRapidHashFunctions(duckdb_connection connection) {
    // Create rapidhash() function
    auto function = duckdb_create_scalar_function();
    duckdb_scalar_function_set_name(function, "rapidhash");
    
    // add a string parameter
    duckdb_logical_type string_type = duckdb_create_logical_type(DUCKDB_TYPE_VARCHAR);
    duckdb_scalar_function_add_parameter(function, string_type);
    
    // set the return type to bigint
    duckdb_logical_type bigint_type = duckdb_create_logical_type(DUCKDB_TYPE_BIGINT);
    duckdb_scalar_function_set_return_type(function, bigint_type);
    
    // set up the function
    duckdb_scalar_function_set_function(function, RapidHashFunction);
    
    // register and cleanup
    duckdb_register_scalar_function(connection, function);
    duckdb_destroy_scalar_function(&function);
    
    // Create rapidhash_with_seed() function
    function = duckdb_create_scalar_function();
    duckdb_scalar_function_set_name(function, "rapidhash_with_seed");
    
    // add string and bigint parameters
    duckdb_scalar_function_add_parameter(function, string_type);
    duckdb_scalar_function_add_parameter(function, bigint_type);
    
    // set the return type to bigint
    duckdb_scalar_function_set_return_type(function, bigint_type);
    
    // set up the function
    duckdb_scalar_function_set_function(function, RapidHashWithSeedFunction);
    
    // register and cleanup
    duckdb_register_scalar_function(connection, function);
    duckdb_destroy_scalar_function(&function);
    
    // cleanup types
    duckdb_destroy_logical_type(&string_type);
    duckdb_destroy_logical_type(&bigint_type);
}