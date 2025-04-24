cmake_minimum_required(VERSION 3.10)
project(TradingEngine)

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Qt6_DIR "/opt/homebrew/Cellar/qt/6.9.0/lib/cmake/Qt6")
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

include_directories(${Qt6Widgets_INCLUDE_DIRS} include)

file(GLOB HEADERS "include/*.hpp")
file(GLOB SRC_ALL "src/*.cpp")
list(FILTER SRC_ALL EXCLUDE REGEX ".*main\\.cpp")  
set(MAIN_SRC src/main.cpp)                         

add_executable(TradingEngine ${MAIN_SRC} ${SRC_ALL} ${HEADERS})
target_link_libraries(TradingEngine Qt6::Widgets Qt6::Charts)

file(GLOB TEST_SOURCES "tests/test_*.cpp")

foreach(TEST_FILE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE} ${SRC_ALL} ${HEADERS})
    target_link_libraries(${TEST_NAME} Qt6::Widgets Qt6::Charts)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()