#include "crow/app.h"
#include <ostream>
#include <string>
#include <jwt-cpp/jwt.h>

int main() 
{
    try 
    {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/api/greet")([]()
                {
                    return crow::response(200, "Hello world!");
                });


        CROW_ROUTE(app, "/api/parse").methods("POST"_method)([](const crow::request& req)
                {
                    auto json = crow::json::load(req.body);
                    if (!json || !json.has("jwt")) 
                    {
                        return crow::response(400, "Missing 'jwt' field in payload");
                    }

                    try 
                    {
                        std::string jwtToken = json["jwt"].s();
                        auto decoded = jwt::decode(jwtToken);
                        return crow::response(200, decoded.get_payload());
                    }
                    catch (const std::exception& e)
                    {
                        return crow::response(400, "Invalid JWT: " + std::string(e.what()));
                    }
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
