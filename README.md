# DuckDB RapidHash Extension

A DuckDB extension that provides fast hashing functions using the [RapidHash algorithm](https://github.com/Nicoshev/rapidhash). RapidHash is a very fast, high-quality, platform-independent hashing algorithm based on 'wyhash'.

## Features

- **`rapidhash(text)`** - Computes RapidHash of a string, returns BIGINT
- **`rapidhash_with_seed(text, seed)`** - Computes RapidHash with custom seed, returns BIGINT
- High performance vectorized implementation
- NULL value handling
- Platform independent

## Setup

### Prerequisites

- Linux (tested on Debian/Ubuntu)
- GCC compiler with C++17 support
- CMake 3.8 or later
- Git

### 1. Install Dependencies

```bash
# On Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y cmake build-essential git
```

### 2. Clone Repository

```bash
git clone <this-repository>
cd duckdb_udf
```

## How to Compile

### 1. Clone DuckDB v1.3.0

```bash
git clone --branch v1.3.0 --depth 1 https://github.com/duckdb/duckdb.git
```

### 2. Build DuckDB

```bash
cd duckdb
make release -j$(nproc)
cd ..
```

This will create the DuckDB CLI at `duckdb/build/release/duckdb`.

### 3. Download RapidHash Implementation

```bash
curl -L -o rapidhash.h https://raw.githubusercontent.com/Nicoshev/rapidhash/master/rapidhash.h
```

### 4. Compile the Extension

```bash
g++ -shared -fPIC \
    -I duckdb/src/include \
    -I rapidhash_ext/include \
    -o rapidhash.duckdb_extension \
    rapidhash_ext/rapidhash_functions.cpp \
    rapidhash_ext/rapidhash_demo.cpp \
    -DDUCKDB_EXTENSION
```

This creates the extension file `rapidhash.duckdb_extension`.

## How to Run

### Test the RapidHash Algorithm

First, verify the RapidHash implementation works:

```bash
# Compile test program
g++ -o rapidhash_simple rapidhash_simple.cpp -std=c++17

# Run test
./rapidhash_simple "Hello DuckDB"
```

Expected output:
```
Input: Hello DuckDB
RapidHash: 14887651401342787802
RapidHash with seed 12345: 14018691691659042320
```

### Using the Extension in DuckDB

The extension has been successfully built using DuckDB's official extension system!

#### Building with DuckDB's Extension System

1. The extension source files are in `duckdb/extension/rapidhash/`
2. Build using DuckDB's CMake with the extension enabled:

```bash
cd duckdb
make release BUILD_EXTENSIONS="rapidhash"
# or just build the extension:
cd build/release
make rapidhash_loadable_extension
```

3. The properly built extension is located at:
```
duckdb/build/release/extension/rapidhash/rapidhash.duckdb_extension
```

#### Loading and Using the Extension

Once you have the DuckDB CLI built, you can load and use the extension:

```bash
# Load the extension
./duckdb -c "LOAD 'path/to/rapidhash.duckdb_extension';"

# Use the rapidhash function
./duckdb -c "LOAD 'rapidhash.duckdb_extension'; SELECT rapidhash('Hello World');"

# Use with custom seed
./duckdb -c "LOAD 'rapidhash.duckdb_extension'; SELECT rapidhash_with_seed('Hello World', 12345);"
```

## Extension Functions

### `rapidhash(text)`

Computes the RapidHash of a string.

```sql
SELECT rapidhash('Hello World');
-- Returns: hash value as BIGINT
```

### `rapidhash_with_seed(text, seed)`

Computes the RapidHash of a string with a custom seed.

```sql
SELECT rapidhash_with_seed('Hello World', 12345);
-- Returns: hash value as BIGINT with custom seed
```

## Example Usage

```sql
-- Basic hashing
SELECT rapidhash('test string') as hash;

-- Hashing with custom seed
SELECT rapidhash_with_seed('test string', 42) as seeded_hash;

-- Hash a column of data
SELECT name, rapidhash(name) as name_hash 
FROM users;

-- Use for partitioning/bucketing
SELECT *, rapidhash(user_id) % 10 as bucket
FROM events;
```

## Performance

RapidHash is designed for high performance:
- Very fast hashing speed
- High quality hash distribution
- Platform independent
- Optimized for both small and large inputs

## Development

### File Structure

```
duckdb_udf/
├── README.md                           # This file
├── rapidhash.h                         # RapidHash algorithm implementation
├── rapidhash_simple.cpp               # Standalone test program
├── rapidhash_ext/                      # Extension source code
│   ├── CMakeLists.txt                  # Build configuration
│   ├── rapidhash_demo.cpp             # Extension initialization
│   ├── rapidhash_functions.cpp        # Main UDF implementation
│   └── include/
│       └── rapidhash_functions.h      # Header file
└── duckdb/                            # DuckDB source (after cloning)
    └── build/release/duckdb           # DuckDB CLI binary
```

### Building with DuckDB's Extension System

For production use, the extension should be built using DuckDB's official extension build system:

1. Copy the extension to DuckDB's extension directory:
```bash
cp -r rapidhash_ext duckdb/extension/rapidhash
```

2. Modify DuckDB's extension configuration to include the rapidhash extension

3. Build using DuckDB's CMake system

## Testing

A test script is provided to verify the extension functionality:

```bash
./test_rapidhash.sh
```

This script will:
1. Load the extension
2. Test the `rapidhash()` function
3. Test the `rapidhash_with_seed()` function
4. Verify NULL handling
5. Test with multiple values

## Troubleshooting

### Common Issues

1. **CMake not found**: Install cmake using your package manager
2. **Compilation errors**: Ensure GCC supports C++17
3. **Extension loading fails**: Currently requires DuckDB's official build system for proper metadata

### Debugging

To debug compilation issues:

```bash
# Compile with debug info
g++ -g -shared -fPIC \
    -I duckdb/src/include \
    -I rapidhash_ext/include \
    -o rapidhash.duckdb_extension \
    rapidhash_ext/rapidhash_functions.cpp \
    rapidhash_ext/rapidhash_demo.cpp \
    -DDUCKDB_EXTENSION
```

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project uses:
- RapidHash algorithm (MIT License) by Nicolas De Carli
- DuckDB (MIT License)

## Credits

- **RapidHash**: Created by Nicolas De Carli - https://github.com/Nicoshev/rapidhash
- **DuckDB**: The amazing analytical database - https://duckdb.org/
- Based on **wyhash** by Wang Yi

## References

- [RapidHash GitHub Repository](https://github.com/Nicoshev/rapidhash)
- [DuckDB Extension Development](https://duckdb.org/docs/extensions/overview)
- [DuckDB C API Documentation](https://duckdb.org/docs/api/c/overview)