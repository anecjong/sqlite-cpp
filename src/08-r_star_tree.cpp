#include <iostream>
#include <string>
#include <vector>

#include <sqlite3.h>

int main()
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

    std::string sql = "BEGIN TRANSACTION;"
                      "DROP TABLE IF EXISTS spatial_index;"
                      "DROP TABLE IF EXISTS spatial_index_node;"
                      "DROP TABLE IF EXISTS spatial_index_rowid;"
                      "DROP TABLE IF EXISTS spatial_index_parent;"
                      "CREATE VIRTUAL TABLE spatial_index USING rtree"
                      "(id INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "minX REAL,"
                      "maxX REAL,"
                      "minY REAL,"
                      "maxY REAL);"
                      "COMMIT;";

    // Use sqlite3_exec for multiple statements instead of prepared statements
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to create tables: " << err_msg << std::endl;
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    std::string insert_sql = "INSERT INTO spatial_index (id, minX, maxX, minY, maxY) VALUES "
                             "(28215, -80.781227, -80.604706, 35.208813, 35.297367),"
                             "(28216, -80.957283, -80.840599, 35.235920, 35.367825),"
                             "(28217, -80.960869, -80.869431, 35.133682, 35.208233),"
                             "(28226, -80.878983, -80.778275, 35.060287, 35.154446),"
                             "(28227, -80.745544, -80.555382, 35.130215, 35.236916),"
                             "(28244, -80.844208, -80.841988, 35.223728, 35.225471),"
                             "(28262, -80.809074, -80.682938, 35.276207, 35.377747),"
                             "(28269, -80.851471, -80.735718, 35.272560, 35.407925),"
                             "(28270, -80.794983, -80.728966, 35.059872, 35.161823),"
                             "(28273, -80.994766, -80.875259, 35.074734, 35.172836),"
                             "(28277, -80.876793, -80.767586, 35.001709, 35.101063),"
                             "(28278, -81.058029, -80.956375, 35.044701, 35.223812),"
                             "(28280, -80.844208, -80.841972, 35.225468, 35.227203),"
                             "(28282, -80.846382, -80.844193, 35.223972, 35.225655);";

    rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_finalize(stmt);

    // Query the spatial index
    std::string query_sql = "SELECT id FROM spatial_index WHERE minX <= ? AND maxX >= ? AND minY <= ? AND maxY >= ?;";
    rc = sqlite3_prepare_v2(db, query_sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_bind_double(stmt, 1, -80.774707);
    sqlite3_bind_double(stmt, 2, -80.774707);
    sqlite3_bind_double(stmt, 3, 35.223728);
    sqlite3_bind_double(stmt, 4, 35.223728);

    while (true)
    {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW)
        {
            std::cout << sqlite3_column_int(stmt, 0) << "\n"
                      << "  minX: " << sqlite3_column_double(stmt, 1) << "\n"
                      << "  maxX: " << sqlite3_column_double(stmt, 2) << "\n"
                      << "  minY: " << sqlite3_column_double(stmt, 3) << "\n"
                      << "  maxY: " << sqlite3_column_double(stmt, 4) << std::endl;
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
    sqlite3_close(db);
    return 0;
}