cmake_minimum_required(VERSION 3.10)
project(TradingEngine)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(TradingEngine ${SOURCES})

file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(TradingEngineTests ${TEST_SOURCES} ${SOURCES})