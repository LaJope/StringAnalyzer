cmake_minimum_required(VERSION 3.30.0)

project(StringAnalyzer VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_BUILD_PARALLEL_LEVEL 16)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(LoggerLib)
add_subdirectory(Sprint)
add_subdirectory(Scount)

add_custom_target(${PROJECT_NAME} ALL)
add_dependencies(${PROJECT_NAME} sprint scount logger)

