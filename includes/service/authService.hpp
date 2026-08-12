#pragma once

#include <cassert>
#include <memory>
#include <optional>
#include <sqlite3.h>
#include "../data/database.hpp"
#include "../models.hpp"
#include "../utils.hpp"

class AuthService
{
private:
    std::shared_ptr<Database> db_;

public:
    explicit AuthService(std::shared_ptr<Database> db)
        : db_(std::move(db)) {}

    bool registerUser(
            const std::string& username,
            const std::string& password,
            const std::string& role
            ) const
    {
        std::string passwordHash = utils::hashPassword(password);
        return db_->insertCredentials(username, passwordHash, role);
    }

    bool authenticateUser(
            const std::string& username,
            const std::string& password

            ) const
    {
        std::optional<UserCredential> userOpt = db_->getCredentials(username);
        if (!userOpt) return false;

        std::string passwordHash = utils::hashPassword(password);
        return passwordHash == userOpt->passwordHash;
    }
};
