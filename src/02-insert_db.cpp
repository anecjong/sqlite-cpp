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

    std::string sql = "INSERT INTO Cars(Id, Name, Price) VALUES(9, 'Mazda', 31000);"
                      "INSERT INTO Cars(Id, Name, Price) VALUES(10, 'Mini', 28500);"
                      "INSERT INTO Cars(Id, Name, Price) VALUES(11, 'Lexus', 45000);";

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
