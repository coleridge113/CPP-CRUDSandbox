#pragma once

#include "crow/app.h"
#include <memory>
#include "../models.hpp"

class MessageController
{
private:
//     std::shared_ptr<MessageService> messageService_;
    crow::response sendMessage(const crow::request& req);

public:
    void registerRoutes(crow::SimpleApp& app);
};
