cmake_minimum_required(VERSION 3.28)
project(TimberGame)

set(CMAKE_CXX_STANDARD 17)

add_executable(TimberGame main.cpp)

#add_executable(TimberGame ${SOURCE_FILES})
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(TimberGame sfml-system sfml-window sfml-graphics sfml-audio sfml-network)