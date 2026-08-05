#pragma once

#include <memory>
#include <sqlite3.h>
#include "database.hpp"

class AuthService
{
    private:
        std::shared_ptr<Database> db_;

    public:
        explicit AuthService(std::shared_ptr<Database> db)
            : db_(std::move(db)) {}
};
