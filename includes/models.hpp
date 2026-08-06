#pragma once 

#include <string>
enum class UserType { GUEST, USER, ADMIN };

struct UserCredential
{
    std::string username;
    std::string passwordHash;
};
