project(scount)

set(CMAKE_BUILD_PARALLEL_LEVEL 16)

set(SOURCE_FILES ./Source_Files/main.cpp ./Source_Files/SprintWriter.cpp ./Source_Files/StdinReader.cpp ./Source_Files/Application.cpp ./Source_Files/StringProcess.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(${PROJECT_NAME} PRIVATE ./Header_Files/)

target_link_libraries(${PROJECT_NAME} logger::lib)

