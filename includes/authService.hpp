#pragma once

#include <memory>
#include <sqlite3.h>
#include "database.hpp"
#include "utils.hpp"

class AuthService
{
    private:
        std::shared_ptr<Database> db_;

    public:
        explicit AuthService(std::shared_ptr<Database> db)
            : db_(std::move(db)) {}

        const bool registerUser(
                const std::string& username,
                const std::string& password,
                const std::string& role
                )
        {
            std::string passwordHash = utils::hashPassword(password);
            return db_->insertCredentials(username, passwordHash, role);
        }
};
