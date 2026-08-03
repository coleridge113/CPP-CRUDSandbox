cmake_minimum_required(VERSION 3.16)
project(crud_sandbox CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

# 1. System Libraries
find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

# 2. Crow HTTP Server
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        v1.3.0
)
FetchContent_MakeAvailable(Crow)

# 3. Nlohmann JSON (Prevents jwt-cpp from fetching an outdated version)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

# 4. JWT-CPP
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JWT_BUILD_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    jwt-cpp
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG        v0.7.0
)
FetchContent_MakeAvailable(jwt-cpp)

# ------------------------------------------------------------------------------
# Executable & Linking
# ------------------------------------------------------------------------------

add_executable(crud_sandbox src/main.cpp)

target_link_libraries(crud_sandbox PRIVATE
    Crow::Crow
    jwt-cpp::jwt-cpp
    nlohmann_json::nlohmann_json
    SQLite3::SQLite3
    OpenSSL::SSL
    OpenSSL::Crypto
)

# ------------------------------------------------------------------------------
# Custom Run Target
# ------------------------------------------------------------------------------

add_custom_target(run
    COMMAND crud_sandbox
    DEPENDS crud_sandbox
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running CRUD Sandbox server..."
)
