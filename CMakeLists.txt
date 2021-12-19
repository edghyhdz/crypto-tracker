cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)
project(crypto_warning)

file(GLOB PROJECT_SRCS
    main.cpp
    src/*.cpp
    include/*.h
)

include_directories(include)
include_directories(src)

add_executable(crypto_warning ${PROJECT_SRCS})

