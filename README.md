# SQLite C++ API Practice

A collection of C++ examples demonstrating how to use SQLite's C/C++ API. This repository contains practical examples showcasing various SQLite features including basic operations, prepared statements, transactions, R*Tree spatial indexing, and more.

## Project Overview

This project demonstrates the usage of SQLite's C++ API through a series of progressive examples:

- **Basic SQLite Operations**:
  - Checking SQLite version
  - Creating databases and tables
  - Inserting data
  - Executing transactions
  
- **Advanced Features**:
  - Prepared statements
  - Retrieving data
  - Working with metadata
  - R*Tree spatial indexing

## Prerequisites

- C++17 compatible compiler
- SQLite3 development libraries
- CMake (version 3.10 or higher)

## Building the Project

```bash
# Create and navigate to build directory
mkdir -p build && cd build

# Configure the project
cmake ..

# Build the examples
cmake --build .
```

## Examples

1. **Version Check** (`00-version.cpp`): Display SQLite library version
2. **Database Creation** (`01-create_db.cpp`): Create SQLite database and tables
3. **Data Insertion** (`02-insert_db.cpp`): Insert data into tables
4. **Transactions** (`03-transaction.cpp`): Use transactions for better performance
5. **Data Retrieval** (`04-retrieve_data.cpp`): Fetch data from tables
6. **Prepared Statements** (`05-prepared_statment.cpp`): Use prepared statements for safe queries
7. **Data Retrieval with Prepared Statements** (`06-prepared_statement_retrieve.cpp`): Combine prepared statements with data retrieval
8. **Metadata** (`07-metadata.cpp`): Access database metadata
9. **R*Tree Spatial Index** (`08-r_star_tree.cpp`): Implement spatial indexing with R*Tree

## Future Improvements

- **BLOB Support for Images**: Implementation for storing and retrieving binary objects like images will be added in future updates.

## References

- [SQLite C/C++ Tutorial](https://zetcode.com/db/sqlitec/) - ZetCode's comprehensive guide to SQLite C/C++ programming
- [SQLite R*Tree Module](https://www.sqlite.org/rtree.html) - Official documentation on using SQLite's R*Tree module for spatial indexing

## License

This project is intended for educational purposes.
