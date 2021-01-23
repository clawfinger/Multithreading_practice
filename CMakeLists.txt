cmake_minimum_required(VERSION 3.5)

set(PROJECT Multithreading)

project(${PROJECT} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    SimpleThreadsafeQueue.h
    SimpleThreadPool.h
    )

set(SOURCES
    main.cpp
    SimpleThreadsafeQueue.cpp
    SimpleThreadPool.cpp
    )

add_executable(${PROJECT} ${SOURCES} ${HEADERS})
