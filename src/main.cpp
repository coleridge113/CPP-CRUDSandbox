#include "crow/app.h"
#include <ostream>
#include <string>
#include <jwt-cpp/jwt.h>
#include <string_view>
#include "../includes/jwtService.hpp"

bool isAuthenticated(const crow::request& req)
{
    const std::string authHeader = req.get_header_value("Authorization");
    constexpr std::string_view prefix = "Bearer ";
    if (authHeader.rfind(prefix, 0) != 0)
    {
        return false;
    }

    std::string token = authHeader.substr(prefix.length());

    return JwtService::validateJwt(token);
}

int main() 
{
    try 
    {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/api/greet")([](const crow::request& req)
                {
                    if (isAuthenticated(req))
                    {
                        return crow::response(401, "Unauthorized");
                    } 
                    else 
                    {
                        return crow::response(200, "Hello world!");
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

        std::cout << "Server listening on http://localhost:18080" << '\n';
        app.port(18080).multithreaded().run();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
