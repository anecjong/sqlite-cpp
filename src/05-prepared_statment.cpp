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

    // Making a template for the insert statement
    // Use placeholder `?` or `:name` to bind the values later
    std::string sql = "INSERT INTO Cars(Name, Price) VALUES(?, ?);";

    /**
     * sqlite3_prepare_v2(db, sql, nByte, prepared_statement, tail)
     * @param db - The database to prepare the statement on
     * @param sql - The SQL statement to prepare
     * @param nByte - The number of bytes in the SQL statement
     * @param prepared_statement - The prepared statement to return
     * @param tail - pointer to the unused portion of the SQL statement
     *
     * @return int - SQLITE_OK on success, otherwise an error code
     */
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc == SQLITE_OK)
    {
        /**
         * sqlite3_bind_text(stmt, index, value, length, destructor)
         * @param stmt - The prepared statement to bind the value to
         * @param index - The index of the parameter to bind the value to (starts from 1)
         * @param value - The text value to bind
         * @param length - Number of bytes in the text (or -1 to use strlen)
         * @param destructor - Memory management function (SQLITE_STATIC or SQLITE_TRANSIENT)
         *
         */
        rc = sqlite3_bind_text(stmt, 1, "BMW", -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to bind value: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }

        /**
         * sqlite3_bind_int(stmt, index, value)
         * @param stmt - The prepared statement to bind the value to
         * @param index - The index of the parameter to bind the value to (starts from 1)
         * @param value - The integer value to bind
         *
         * @return int - SQLITE_OK on success, otherwise an error code
         */
        rc = sqlite3_bind_int(stmt, 2, 36000);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to bind value: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
    }
    else
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    // Execute the statement
    // Return SQLITE_DONE when the statement is executed successfully
    // Return SQLITE_ROW when the statement is a SELECT statement
    // Otherwise, return an error code
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    std::cout << "Record inserted successfully" << std::endl;

    // Finalize the statement - free the resources allocated to the statement
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
