cmake_minimum_required(VERSION 3.11.3)

set(CMAKE_CXX_STANDARD 17)
project(crypto_warning)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SERVED_PKG REQUIRED IMPORTED_TARGET libmongocxx)  

file(GLOB PROJECT_SRCS
    main.cpp
    src/*.cpp
    include/*.h
)

include_directories(include)
include_directories(src)

add_executable(crypto_warning ${PROJECT_SRCS})
target_link_libraries(crypto_warning PkgConfig::SERVED_PKG Threads::Threads)
