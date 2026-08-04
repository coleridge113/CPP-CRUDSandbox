#pragma once

#include <chrono>
#include <jwt-cpp/jwt.h>
#include <string>

class JwtService 
{
    public:
        std::string authenticateUser(
                std::string& username,
                std::string& password
                )
        {
            jwt::date issuedAt = std::chrono::system_clock::now();
            jwt::date expiresAt = std::chrono::system_clock::now() +
                std::chrono::hours(24);

            auto token = jwt::create()
                .set_type("JWS")
                .set_issuer("crud-sandbox")
                .set_payload_claim("user", jwt::claim(std::string(username)))
                .set_payload_claim("role", jwt::claim(std::string("user")))
                .set_issued_at(issuedAt)
                .set_expires_at(expiresAt)
                .sign(jwt::algorithm::hs256{"secret"});

            return token;
        }
};
