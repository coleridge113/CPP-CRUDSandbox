#pragma once

#include "crow/app.h"
#include <memory>
#include "../service/authService.hpp"

class AuthController
{
private:
    std::shared_ptr<AuthService> authService_;

    crow::response handleLogin(const crow::request& req);
    crow::response handleSignup(const crow::request& req);

public:
    explicit AuthController(std::shared_ptr<AuthService> authService);

    void registerRoutes(crow::SimpleApp& app);
};
