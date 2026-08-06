#pragma once

#include "models.hpp"
#include <exception>
#include <iostream>
#include <optional>
#include <sqlite3.h>
#include <stdexcept>
#include <string>

inline constexpr int TIMEOUT = 5000;

class Database
{
    private:
        sqlite3* db_ = nullptr;

        void initSchema()
        {
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

            execute(schema);
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

            initSchema();
        }


        ~Database()
        {
            if (db_) sqlite3_close_v2(db_);
        }

        bool insertCredentials(
                const std::string& username, 
                const std::string& passwordHash,
                const std::string& role
                )
        {
            const char* sql = R"( 
                INSERT INTO user_credentials (username, password_hash, role)
                VALUES (?, ?, ?);
            )";
            sqlite3_stmt* stmt = nullptr;

            if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
            {
                std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db_) << std::endl;
                return false;
            }

            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, passwordHash.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, role.c_str(), -1, SQLITE_STATIC);

            int result = sqlite3_step(stmt);
            bool success = false;

            if (result == SQLITE_DONE)
            {
                success = true;
            }
            else
            {
                std::cerr << "Insert failed: " << sqlite3_errmsg(db_) << std::endl;
            }

            sqlite3_finalize(stmt);

            return success;
        }

        const std::optional<UserCredential> getCredentials(const std::string& username)
        {
            const char* sql = R"(
                SELECT * FROM user_credentials WHERE username = ?;
            )";

            sqlite3_stmt* stmt = nullptr;
            if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK)
            {
                throw std::runtime_error("Invalid credentials!");
            }
            sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

            int result = sqlite3_step(stmt);
            bool success = false;
            UserCredential user;

            if (result == SQLITE_ROW)
            {
                success = true;
                const unsigned char* username = sqlite3_column_text(stmt, 1);
                const unsigned char* passwordHash = sqlite3_column_text(stmt, 2);

                user.username = username 
                    ? reinterpret_cast<const char*>(username) 
                    : "";
                user.passwordHash = passwordHash 
                    ? reinterpret_cast<const char*>(passwordHash) 
                    : "";
            }
            else
            {
                success = false;
                std::cerr << "Failed to fetch credentials: " << sqlite3_errmsg(db_) << std::endl;
            }

            sqlite3_finalize(stmt);

            if (success) return user;
            return std::nullopt;
        }
};
