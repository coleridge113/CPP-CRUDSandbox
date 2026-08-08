#include "crow/app.h"
#include <memory>
#include <string>
#include <jwt-cpp/jwt.h>
#include "../includes/utils.hpp"
#include "../includes/data/database.hpp"
#include "../includes/service/authService.hpp"
#include "./controllers/authController.cpp"

int main() 
{
    crow::SimpleApp app;
    auto sharedDb = std::make_shared<Database>("crud_db");
    auto authService = std::make_shared<AuthService>(sharedDb);
    AuthController authController(authService);

    authController.registerRoutes(app);

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


    std::cout << "Server listening on http://localhost:18080" << '\n';
    app.port(18080).multithreaded().run();

    return 0;
}
