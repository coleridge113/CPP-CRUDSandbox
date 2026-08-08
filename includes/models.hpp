#pragma once 

#include <string>
#include <chrono>

enum class UserType { GUEST, USER, ADMIN };

struct UserCredential
{
    std::string username;
    std::string passwordHash;
};

using Instant = std::chrono::system_clock::time_point;
struct Message
{
    std::string sender;
    std::string recipient;
    std::string body;
    Instant time_sent;
};
