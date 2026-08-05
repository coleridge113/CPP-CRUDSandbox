#pragma once

#include "crow/app.h"
#include "jwt-cpp/jwt.h"
#include "jwtService.hpp"
#include "models.hpp"

inline std::string userTypeToString(UserType type)
{
    switch (type)
    {
        case UserType::GUEST:    return "guest";
        case UserType::ADMIN:    return "admin";
        case UserType::USER:     return "user";
    }
}

inline bool isAuthenticated(const crow::request& req)
{
    const std::string authHeader = req.get_header_value("Authorization");
    constexpr std::string_view prefix = "Bearer ";

    if (authHeader.rfind(prefix, 0) != 0) return false;

    std::string token = authHeader.substr(prefix.length());

    return JwtService::validateJwt(token);
}

inline bool isAuthorized(const crow::request& req)
{
    const std::string authHeader = req.get_header_value("Authorization");
    constexpr std::string_view prefix = "Bearer ";

    if (authHeader.rfind(prefix, 0) != 0) return false;

    std::string token = authHeader.substr(prefix.length());
    auto decoded = jwt::decode(token);

    return decoded.get_payload_claim("role").as_string() != userTypeToString(UserType::GUEST);
}
