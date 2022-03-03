cmake_minimum_required(VERSION 3.12)
project(messari)

set(VERBOSE=1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)
FetchContent_Declare(
    boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
    PREFIX ${PROJECT_SOURCE_DIR}/inc/boost/
    STAMP_DIR ${PROJECT_SOURCE_DIR}/inc/boost/log/
    BUILD_COMMAND ./bootstrap.sh
)
FetchContent_GetProperties(boost)
if (NOT boost_POPULATED)
    message(STATUS "Populating boost")
    FetchContent_Populate(boost)
    set(BOOST_BUILD_TESTS OFF)
    include_directories(${boost_SOURCE_DIR} ${boost_BINARY_DIR})
    message(STATUS "Done populating boost")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(${PROJECT_SOURCE_DIR}/inc/)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)
