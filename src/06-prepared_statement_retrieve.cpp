#include <iostream>
#include <string>

#include <sqlite3.h>

int main(void)
{
    sqlite3* db;
    char* err_msg = nullptr;
    sqlite3_stmt* stmt;

    auto rc = sqlite3_open("test.db", &db);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::string sql = "SELECT Id, Name, Price FROM Cars WHERE Id >= ? AND Id <= ?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_bind_int(stmt, 1, 1); // find id >= 1
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to bind value: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_bind_int(stmt, 2, 3); // find id <= 3
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to bind value: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    while (true)
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            std::cout << sqlite3_column_int(stmt, 0) << " " << sqlite3_column_text(stmt, 1) << " "
                      << sqlite3_column_int(stmt, 2) << std::endl;
        }
        else if (rc == SQLITE_DONE)
        {
            std::cout << "Search completed" << std::endl;
            break;
        }
        else
        {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
