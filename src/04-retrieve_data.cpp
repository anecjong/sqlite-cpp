#include <iostream>
#include <string>

#include <sqlite3.h>

/**
 * SQLite callback function signature:
 * int callback(void *data, int argc, char **argv, char **azColName)
 *
 * Parameters:
 * @param data      - Pointer to user-provided data passed to sqlite3_exec()
 * @param argc      - Number of columns in the result row
 * @param argv      - Array of strings containing column values for the current row
 * @param azColName - Array of strings containing column names
 *
 * @return int      - Return 0 to continue processing rows
 *                  - Return non-zero to abort execution
 *
 * The callback is invoked once for each row returned by the SQL query.
 * All values are provided as strings, even numeric data.
 */

int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    static int number_of_calls = 0;
    NotUsed = nullptr; // To avoid unused parameter warning in compiler

    std::cout << "Number of calls: " << ++number_of_calls << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << " = " << argv[i] << std::endl;
    }

    std::cout << std::endl;
    return 0;
}

int main(void)
{
    sqlite3* db;
    char* err_msg = nullptr;

    auto rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::string sql = "SELECT * FROM Cars;";

    rc = sqlite3_exec(db, sql.c_str(), callback, nullptr, &err_msg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
