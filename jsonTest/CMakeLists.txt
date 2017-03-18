cmake_minimum_required(VERSION 3.6)
project(jsonTest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        compile
        index.html
        index.js
        json.hpp
        jsonTest.cpp
        jsonTest.js
        jsonTest.wasm)

add_executable(jsonTest ${SOURCE_FILES})