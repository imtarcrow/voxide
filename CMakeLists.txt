cmake_minimum_required(VERSION 3.31.10)

project("voxide" VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set_source_files_properties(
    src/glad.c
    PROPERTIES
    COMPILE_OPTIONS "-w"
)
# Setup project files
set(SOURCES 
    src/main.cpp
    src/window.cpp
    src/game.cpp
    src/glad.c
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ./include)

find_package(SDL3 REQUIRED CONFIG)
find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 OpenGL::GL)

# Disable C++ Compiler extensions 
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_EXTENSIONS OFF
    CXX_EXTENSIONS OFF
)

# Setup compiler settings
target_compile_features(${PROJECT_NAME} PRIVATE c_std_23 cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
