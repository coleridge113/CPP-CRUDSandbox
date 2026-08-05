#pragma once

#include <exception>
#include <sqlite3.h>
#include <stdexcept>
#include <string>

inline constexpr int TIMEOUT = 5000;

class Database
{
    private:
        sqlite3* db_ = nullptr;

    public:
        Database(const std::string& dbPath)
        {
            if (sqlite3_open(dbPath.c_str(), &db_) != SQLITE_OK)
            {
                throw std::runtime_error(
                        "Failed to open SQLite Datbase: " + std::string(sqlite3_errmsg(db_))
                        );
            }

            sqlite3_busy_timeout(db_, TIMEOUT);
            execute("PRAGMA journal_mode = WAL");
            execute("PRAGMA synchronous = NORMAL");
        }

        const char* schema = R"(
            CREATE TABLE IF NOT EXISTS user_credentials (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL UNIQUE,
                password_hash TEXT NOT NULL,
                role TEXT NOT NULL DEFAULT 'user',
                created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
                updated_at DATETIME DEFAULT CURRENT_TIMESTAMP
            );
        )";

        ~Database()
        {
            if (db_) sqlite3_close_v2(db_);
        }

        void execute(const std::string& sql)
        {
            char* errMsg = nullptr;
            if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
            {
                std::string error = errMsg ? errMsg : "Unknown error occurred...";
                sqlite3_free(errMsg);
                throw std::runtime_error("SQL Execution Error: " + error);
            }
        }
};
