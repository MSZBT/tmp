cmake_minimum_required(VERSION 3.20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(TARGET_NAME migr)
project(${TARGET_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)
find_package(libpqxx REQUIRED)


set(UTILITY_SOURCES
        source/database_manager.cpp
        source/logger.cpp
        source/parser.cpp
        utility.cpp
)

set(UTILITY_HEADERS
        include/database_manager.h
        include/logger.h
        include/parsing.h
)


add_executable(${TARGET_NAME} ${UTILITY_SOURCES} ${UTILITY_HEADERS})


target_include_directories(${TARGET_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

string(REPLACE "\\" "/" PROJECT_DIR_FIXED "${PROJECT_SOURCE_DIR}")
target_compile_definitions(${TARGET_NAME} PRIVATE PROJECT_DIR="${PROJECT_DIR_FIXED}")

target_link_libraries(${TARGET_NAME} PRIVATE CLI11::CLI11 libpqxx::pqxx spdlog::spdlog nlohmann_json::nlohmann_json)