#pragma once

#include <chrono>
#include <jwt-cpp/jwt.h>
#include <string>

namespace JwtService 
{

    inline const std::string secretKey = "my-secret-key";

    inline std::string authenticateUser(
            const std::string& username,
            const std::string& password
            )
    {
        jwt::date issuedAt = std::chrono::system_clock::now();
        jwt::date expiresAt = issuedAt + std::chrono::hours(24);

        auto token = jwt::create()
            .set_type("JWS")
            .set_issuer("crud-sandbox")
            .set_payload_claim("name", jwt::claim(username))
            .set_payload_claim("role", jwt::claim(std::string("user")))
            .set_issued_at(issuedAt)
            .set_expires_at(expiresAt)
            .sign(jwt::algorithm::hs256{secretKey});

        return token;
    }
}
