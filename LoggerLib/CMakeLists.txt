project(logger)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(SOURCE_FILES ./Source_Files/Logger.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
add_library(logger::lib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include/)
