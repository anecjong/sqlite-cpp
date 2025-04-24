cmake_minimum_required(VERSION 3.10)

project(SQLite3-CPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SQLite3 REQUIRED)
link_libraries(SQLite::SQLite3)

add_executable(00-version src/00-version.cpp)
add_executable(01-create_db src/01-create_db.cpp)
add_executable(02-insert_db src/02-insert_db.cpp)
add_executable(03-transaction src/03-transaction.cpp)
add_executable(04-retrieve_data src/04-retrieve_data.cpp)
add_executable(05-prepared_statement src/05-prepared_statment.cpp)
add_executable(06-prepared_statement_retrieve src/06-prepared_statement_retrieve.cpp)
add_executable(07-metadata src/07-metadata.cpp)
add_executable(08-r_star_tree src/08-r_star_tree.cpp)