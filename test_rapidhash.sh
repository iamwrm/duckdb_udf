#!/bin/bash

# Test script for RapidHash DuckDB extension

echo "=== RapidHash DuckDB Extension Test ==="
echo

# Find DuckDB executable
DUCKDB="/home/wr/.duckdb/cli/latest/duckdb -unsigned"

echo "Using DuckDB: $DUCKDB"

# Find extension
EXTENSION=""
if [ -f "./duckdb/build/release/extension/rapidhash/rapidhash.duckdb_extension" ]; then
    EXTENSION="./duckdb/build/release/extension/rapidhash/rapidhash.duckdb_extension"
elif [ -f "./rapidhash.duckdb_extension" ]; then
    EXTENSION="./rapidhash.duckdb_extension"
else
    echo "Error: rapidhash.duckdb_extension not found!"
    exit 1
fi

echo "Using extension: $EXTENSION"
echo

# Test 1: Load extension
echo "Test 1: Loading extension..."
$DUCKDB -c "LOAD '$EXTENSION';" 2>&1
if [ $? -eq 0 ]; then
    echo "✓ Extension loaded successfully"
else
    echo "✗ Failed to load extension"
    exit 1
fi
echo

# Test 2: Basic rapidhash function
echo "Test 2: Testing rapidhash('Hello World')..."
RESULT=$($DUCKDB -c "LOAD '$EXTENSION'; SELECT rapidhash('Hello World') as hash;" 2>&1)
if [ $? -eq 0 ]; then
    echo "✓ rapidhash function works"
    echo "$RESULT"
else
    echo "✗ rapidhash function failed"
    echo "$RESULT"
    exit 1
fi
echo

# Test 3: RapidHash with seed
echo "Test 3: Testing rapidhash_with_seed('Hello World', 12345)..."
RESULT=$($DUCKDB -c "LOAD '$EXTENSION'; SELECT rapidhash_with_seed('Hello World', 12345) as seeded_hash;" 2>&1)
if [ $? -eq 0 ]; then
    echo "✓ rapidhash_with_seed function works"
    echo "$RESULT"
else
    echo "✗ rapidhash_with_seed function failed"
    echo "$RESULT"
    exit 1
fi
echo

# Test 4: NULL handling
echo "Test 4: Testing NULL handling..."
RESULT=$($DUCKDB -c "LOAD '$EXTENSION'; SELECT rapidhash(NULL) as null_hash;" 2>&1)
if [ $? -eq 0 ]; then
    echo "✓ NULL handling works"
    echo "$RESULT"
else
    echo "✗ NULL handling failed"
    echo "$RESULT"
    exit 1
fi
echo

# Test 5: Multiple values
echo "Test 5: Testing with multiple values..."
$DUCKDB -c "LOAD '$EXTENSION'; 
    SELECT 
        value,
        rapidhash(value) as hash,
        rapidhash_with_seed(value, 42) as seeded_hash
    FROM (VALUES 
        ('test'), 
        ('DuckDB'), 
        ('RapidHash'), 
        (''), 
        (NULL)
    ) t(value);" 2>&1
echo

echo "=== All tests completed ==="