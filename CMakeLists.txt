cmake_minimum_required(VERSION 3.10)
project(TradingEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Qt6_DIR "/opt/homebrew/Cellar/qt/6.9.0/lib/cmake/Qt6")
find_package(Qt6 COMPONENTS Widgets Charts REQUIRED)

include_directories(${Qt6Widgets_INCLUDE_DIRS} include)
file(GLOB HEADERS "include/*.hpp")
file(GLOB SOURCES "src/*.cpp")

add_executable(TradingEngine ${SOURCES} ${HEADERS})
target_link_libraries(TradingEngine Qt6::Widgets Qt6::Charts)