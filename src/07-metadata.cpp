#include <iostream>
#include <string>
#include <vector>

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

    std::string sql = "SELECT name FROM sqlite_master WHERE type=?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // Table names
    rc = sqlite3_bind_text(stmt, 1, "table", -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to bind value: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    std::vector<std::string> tables;
    std::cout << "Tables: " << std::endl;
    while (true)
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            tables.push_back(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0))));
            std::cout << "\t" << tables.back() << std::endl;
        }
        else if (rc == SQLITE_DONE)
        {
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

    // For each table, get the column names
    sqlite3_finalize(stmt);

    for (const auto& table : tables)
    {
        std::string pragma_sql = "PRAGMA table_info(" + table + ");";
        rc = sqlite3_prepare_v2(db, pragma_sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return 1;
        }

        std::cout << "\nColumns for table " << table << ":" << std::endl;
        while (true)
        {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW)
            {
                int cid = sqlite3_column_int(stmt, 0);
                const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
                int notnull = sqlite3_column_int(stmt, 3);
                const char* dflt_value = nullptr;
                if (sqlite3_column_type(stmt, 4) != SQLITE_NULL)
                {
                    dflt_value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                }
                int pk = sqlite3_column_int(stmt, 5);

                std::cout << "\t" << cid << ": " << name << " (" << type << ")" << (pk ? " PRIMARY KEY" : "")
                          << (notnull ? " NOT NULL" : "") << (dflt_value ? " DEFAULT " + std::string(dflt_value) : "")
                          << std::endl;
            }
            else if (rc == SQLITE_DONE)
            {
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
    }

    sqlite3_close(db);
    return 0;
}