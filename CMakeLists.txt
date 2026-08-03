cmake_minimum_required(VERSION 3.16)
project(url_shortener CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Setup FetchContent
include(FetchContent)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        v1.3.3
)
FetchContent_MakeAvailable(Crow)

# 2. Define the executable FIRST
add_executable(url_shortener src/main.cpp)

# 3. Now link libraries to the target you just created
target_link_libraries(url_shortener PRIVATE Crow::Crow)

find_package(SQLite3 REQUIRED)
target_link_libraries(url_shortener PRIVATE SQLite3::SQLite3)

# 4. Add the custom command
add_custom_target(run
    COMMAND url_shortener
    DEPENDS url_shortener
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running URL Shortener server..."
)
