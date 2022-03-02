cmake_minimum_required(VERSION 3.12)
project(messari)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(json
    URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
    SOURCE_DIR ${PROJECT_SOURCE_DIR}/inc/nlohmann
)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
    FetchContent_Populate(json)
    add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(${PROJECT_SOURCE_DIR}/inc/)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    nlohmann_json::nlohmann_json
)
