cmake_minimum_required(VERSION 4.2)

project("voxide" VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Setup project files
set(SOURCES 
    src/main.cpp
    src/window.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)

find_package(SDL3 REQUIRED CONFIG)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# Disable C++ Compiler extensions 
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_EXTENSIONS OFF
)

# Setup compiler settings
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
