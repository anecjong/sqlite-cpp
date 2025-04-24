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

    std::string sql = "DROP TABLE IF EXISTS Cars;"
                      "CREATE TABLE Cars(Id INTEGER PRIMARY KEY UNIQUE, Name TEXT, Price INT);"
                      "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                      "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                      "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                      "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                      "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                      "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                      "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                      "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    // sqlite3_exec(database, sql, callback, callback_arg, err_msg)
    // sqlite3_exec: prepare, step, finalize at once
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
