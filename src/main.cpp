#include "crow.h"
#include "crow/app.h"
#include <ostream>

int main() 
{
    try 
    {
        crow::SimpleApp app;

        CROW_ROUTE(app, "/api/greet")([](){
                return crow::response(200, "Hello world!");
                });

        std::cout << "Server listening on http://localhost:18080" << '\n';
        app.port(18080).multithreaded().run();
    } 
    catch (const std::exception& err) 
    {
        std::cerr << "Error occurred: " << err.what() << std::endl;
        return 1;
    }

    return 0;
}
