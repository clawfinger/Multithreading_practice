cmake_minimum_required(VERSION 3.5)

set(PROJECT Multithreading)

project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    SimpleThreadsafeQueue.h
    )

set(SOURCES
    main.cpp
    SimpleThreadsafeQueue.cpp)

add_executable(${PROJECT} ${SOURCES} ${HEADERS})
