cmake_minimum_required(VERSION 3.21)
project(ReactionTime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SFML directory manually
set(SFML_DIR "C:/vcpkg/installed/x64-windows/share/sfml")

# Find SFML - SFML 3.0 uses different component names
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Add your executable
add_executable(reaction_time main.cpp)

# Link SFML - note the capitalization for SFML 3.0
target_link_libraries(reaction_time PRIVATE SFML::Graphics SFML::Window SFML::System)

# Copy SFML DLLs to build directory (Windows only)
if(WIN32)
    add_custom_command(TARGET reaction_time POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SFML::Graphics>
        $<TARGET_FILE:SFML::Window>
        $<TARGET_FILE:SFML::System>
        $<TARGET_FILE_DIR:reaction_time>)
endif()