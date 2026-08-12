#include "crow/app.h"
#include "../../includes/controllers/messageController.hpp"
#include "../../includes/utils.hpp"

void MessageController::registerRoutes(crow::SimpleApp& app)
{
    CROW_ROUTE(app, "/api/message").methods("POST"_method)(
            [this](const crow::request& req) { return sendMessage(req); }
            );
}

crow::response MessageController::sendMessage(const crow::request& req)
{
    if (!utils::isAuthenticated(req) || !utils::isAuthorized(req))
        return crow::response(400, "Unauthorized!");

    auto json = crow::json::load(req.body);
    crow::json::wvalue res;
    if (
            !json || 
            !json.has("recipient") || 
            !json.has("body") ||
            !json.has("sender") ||
            !json.has("time_sent")
            )
    {
        res["status"] = "error";
        res["message"] = "missing fields";
        return crow::response(400, res);
    }

    std::string sender = json["sender"].s();
    std::string recipient = json["recipient"].s();
    std::string body = json["body"].s();
    Instant timestamp = utils::parseIsoToTimeStamp(json["time_sent"].s());

    Message message {
        .sender = sender,
        .recipient = recipient,
        .body = body,
        .time_sent = timestamp
    };

    res["status"] = "success";
    return crow::response(200, res);
}

