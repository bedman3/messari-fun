cmake_minimum_required(VERSION 3.12)
project(messari)

set(CMAKE_CXX_STANDARD 14)

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

FetchContent_Declare(
    ctpl
    GIT_REPOSITORY https://github.com/vit-vit/CTPL.git
    GIT_TAG ctpl_v.0.0.2
    PREFIX ${PROJECT_SOURCE_DIR}/inc/ctpl/
    STAMP_DIR ${PROJECT_SOURCE_DIR}/inc/ctpl/log/
)
FetchContent_GetProperties(ctpl)
if (NOT ctpl_POPULATED)
    message(STATUS "Populating ctpl")
    FetchContent_Populate(ctpl)
    include_directories(${ctpl_SOURCE_DIR} ${ctpl_BINARY_DIR})
    message(STATUS "Done populating ctpl")
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/inc/)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    pthread
)
