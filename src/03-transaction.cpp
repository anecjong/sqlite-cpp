#include <iostream>
#include <string>

#include <sqlite3.h>

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

    // Transaction:
    // Start with "BEGIN TRANSACTION;"
    // End with "COMMIT;"
    // The operations between "BEGIN TRANSACTION;" and "COMMIT;" are executed as a single unit.
    // If any of the operations fail, the entire transaction is rolled back to the state before the transaction was
    // started. Generally, it is faster to execute a transaction than to execute a series of operations.
    std::string sql = "DROP TABLE IF EXISTS Friends;"
                      "BEGIN TRANSACTION;"
                      "CREATE TABLE Friends(Id INTEGER PRIMARY KEY, Name TEXT);"
                      "INSERT INTO Friends(Name) VALUES ('Tom');"
                      "INSERT INTO Friends(Name) VALUES ('Rebecca');"
                      "INSERT INTO Friends(Name) VALUES ('Jim');"
                      "INSERT INTO Friends(Name) VALUES ('Roger');"
                      "INSERT INTO Friends(Name) VALUES ('Robert');"
                      "COMMIT;";

    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
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
