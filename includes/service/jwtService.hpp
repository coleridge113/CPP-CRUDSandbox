#pragma once

#include <chrono>
#include <iostream>
#include <jwt-cpp/jwt.h>
#include <string>

namespace JwtService 
{

    inline const std::string secretKey = "my-secret-key";
    inline const std::string issuer = "crud-sandbox";
    inline const auto signingAlgorithm = jwt::algorithm::hs256{secretKey};

    inline std::string authenticateUser(const std::string& username)
    {
        jwt::date issuedAt = std::chrono::system_clock::now();
        jwt::date expiresAt = issuedAt + std::chrono::hours(24);

        auto token = jwt::create()
            .set_type("JWS")
            .set_issuer(issuer)
            .set_payload_claim("name", jwt::claim(username))
            .set_payload_claim("role", jwt::claim(std::string("user")))
            .set_issued_at(issuedAt)
            .set_expires_at(expiresAt)
            .sign(signingAlgorithm);

        return token;
    }

    inline bool validateJwt(const std::string& token)
    {
        try
        {
            auto decode = jwt::decode(token);
            auto verifier = jwt::verify()
                .with_issuer(issuer)
                .allow_algorithm(signingAlgorithm);

            verifier.verify(decode);
            return true;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error validating token: " << e.what() << '\n';
            return false;
        }
    }
}
