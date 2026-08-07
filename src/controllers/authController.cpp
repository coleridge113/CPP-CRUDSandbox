#include "../../includes/controllers/authController.hpp"
#include "crow/app.h"
#include <memory>

AuthController::AuthController(std::shared_ptr<AuthService> authService)
    : authService_(std::move(authService)) {}

void AuthController::registerRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/api/login").methods("POST"_method)(
            [this](const crow::request& req) { return handleLogin(req); }
            );

    CROW_ROUTE(app, "/api/signup").methods("POST"_method)(
            [this](const crow::request& req) { return handleSignup(req); }
            );
}

crow::response AuthController::handleLogin(const crow::request& req) 
{
    auto json = crow::json::load(req.body);
    if (!json || !json.has("username") || !json.has("password"))
    {
        return crow::response(400, "Missing fields");
    }

    std::string username = json["username"].s();
    std::string password = json["password"].s();


    bool isValidated = authService_->authenticateUser(username, password);
    crow::json::wvalue res;
    if (isValidated)
    {
        std::string token = JwtService::authenticateUser(username);
        res["token"] = token;
        res["status"] = "success";
        return crow::response(200, res);
    }
    else
    {
        res["status"] = "error";
        return crow::response(401, res);
    }
}

crow::response AuthController::handleSignup(const crow::request& req)
{

    auto json = crow::json::load(req.body);
    if (!json) return crow::response(400, "Missing payload");

    std::string username = json["username"].s();
    std::string password = json["password"].s();

    if (authService_->registerUser(username, password, "user"))
    {
        return crow::response(200, "User has been registered");
    }
    else 
    {
        return crow::response(500, "Failed to register user");
    }

}
