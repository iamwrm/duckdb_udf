#!/bin/bash

# Build script for rapidhash DuckDB extension

echo "Building rapidhash extension for DuckDB..."

# Create build directory
cd duckdb/extension/rapidhash
mkdir -p build
cd build

# Configure with DuckDB's CMake
cmake -DDUCKDB_ROOT_DIR=../../.. ..

# Build the extension
make -j$(nproc)

# Copy the extension to the main directory
cp *.duckdb_extension ../../../../

echo "Extension built successfully!"
echo "To use: LOAD 'rapidhash.duckdb_extension';"