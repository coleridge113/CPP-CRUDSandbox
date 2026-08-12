#pragma once

#include "crow/app.h"
#include "jwt-cpp/jwt.h"
#include "./service/jwtService.hpp"
#include "models.hpp"
#include <openssl/evp.h>
#include <sstream>
#include <string>

using Instant = std::chrono::system_clock::time_point;

namespace utils
{
    inline std::string userTypeToString(UserType type)
    {
        switch (type)
        {
            case UserType::GUEST:    return "guest";
            case UserType::ADMIN:    return "admin";
            case UserType::USER:     return "user";
        }

        return "unknown";
    }

    inline std::string extractRole(const std::string& token)
    {
        try
        {
            const auto decoded = jwt::decode(token);
            return decoded.get_payload_claim("role").as_string();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
            return "";
        }
    }

    inline std::string extractToken(const crow::request& req)
    {
        const std::string authHeader = req.get_header_value("Authorization");
        constexpr std::string_view prefix = "Bearer ";

        if (authHeader.rfind(prefix, 0) != 0)
        {  
            return ""; 
        }
        else
        {
            return authHeader.substr(prefix.length());
        }
    }

    inline bool isAuthenticated(const crow::request& req)
    {
        std::string token = extractToken(req);
        return JwtService::validateJwt(token);
    }

    inline bool isAuthorized(const crow::request& req)
    {
        std::string token = extractToken(req);
        auto decoded = jwt::decode(token);

        return decoded.get_payload_claim("role").as_string() != userTypeToString(UserType::GUEST);
    }

    inline std::string hashPassword(const std::string& password)
    {
        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int lengthOfHash = 0;

        EVP_MD_CTX* context = EVP_MD_CTX_new();
        EVP_DigestInit_ex(context, EVP_sha256(), nullptr);
        EVP_DigestUpdate(context, password.c_str(), password.size());
        EVP_DigestFinal_ex(context, hash, &lengthOfHash);
        EVP_MD_CTX_free(context);

        std::stringstream ss;
        for (unsigned int i = 0; i < lengthOfHash; ++i)
        {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        return ss.str();
    }

    inline Instant parseIsoToTimeStamp(const std::string& isoStr)
    {
        std::istringstream ss(isoStr);
        Instant tp;

        ss >> std::chrono::parse("%Y-%m-%dT%H:%M:%SZ", tp);

        if (ss.fail())
        {
            return std::chrono::system_clock::now();
        }
        return tp;
    }
}
