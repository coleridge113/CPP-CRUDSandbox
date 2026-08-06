#include "crow/app.h"
#include <memory>
#include <ostream>
#include <string>
#include <jwt-cpp/jwt.h>
#include "../includes/jwtService.hpp"
#include "../includes/utils.hpp"
#include "../includes/database.hpp"
#include "../includes/authService.hpp"

int main() 
{
    crow::SimpleApp app;
    auto sharedDb = std::make_shared<Database>("crud_db");
    AuthService authService(sharedDb);

    CROW_ROUTE(app, "/api/greet")([](const crow::request& req)
            {
                if (utils::isAuthenticated(req) && utils::isAuthenticated(req))
                {
                    return crow::response(200, "Hello world!");
                } 
                else 
                {
                    return crow::response(401, "Unauthorized");
                }
            });


    CROW_ROUTE(app, "/api/parse").methods("POST"_method)([](const crow::request& req)
            {
                auto json = crow::json::load(req.body);
                if (!json || !json.has("jwt")) 
                {
                    return crow::response(400, "Missing 'jwt' field in payload");
                }

                std::string jwtToken = json["jwt"].s();
                auto decoded = jwt::decode(jwtToken);
                return crow::response(200, decoded.get_payload());
            });

    CROW_ROUTE(app, "/api/login").methods("POST"_method)([](const crow::request& req)
            {
                auto json = crow::json::load(req.body);
                if (!json || !json.has("username") || !json.has("password"))
                {
                    return crow::response(400, "Missing fields");
                }

                std::string username = json["username"].s();
                std::string password = json["password"].s();

                auto token = JwtService::authenticateUser(username, password);

                return crow::response(200, token);

            });

    CROW_ROUTE(app, "/api/signup").methods("POST"_method)([&authService](const crow::request& req)
            {
                auto json = crow::json::load(req.body);
                if (!json) return crow::response(400, "Missing payload");

                std::string username = json["username"].s();
                std::string password = json["password"].s();

                if (authService.registerUser(username, password, "user"))
                {
                    return crow::response(200, "User has been registered");
                }
                else 
                {
                    return crow::response(500, "Failed to register user");
                }
            });

    std::cout << "Server listening on http://localhost:18080" << '\n';
    app.port(18080).multithreaded().run();

    return 0;
}
