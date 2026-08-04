#pragma once

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
            auto token = jwt::create()
                .set_type("JWS")
                .set_issuer("auth0")
                .set_payload_claim("sample", jwt::claim(std::string("test")))
                .sign(jwt::algorithm::hs256{"secret"});

            return token;
        }
};

