cmake_minimum_required(VERSION 3.10)

# Project name
project(ClothSimulation)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the source files
set(SOURCE_FILES
    Main.cpp
    Renderer.cpp 
)

# Add the executable
add_executable(main ${SOURCE_FILES})

# Platform-specific configurations

# macOS configuration
if(APPLE)
    set(SDL2_LIBRARY "/opt/homebrew/lib/libSDL2-2.0.0.dylib")  # Update with the correct path if needed
    set(SDL2_IMAGE_LIBRARY "/opt/homebrew/lib/libSDL2_image.dylib")  # Update with the correct path if needed
    set(SDL2_INCLUDE_DIR "/opt/homebrew/include/")
    set(SDL2_IMAGE_INCLUDE_DIR "/opt/homebrew/include/")

    # Check if the paths were found
    if(NOT EXISTS ${SDL2_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2 include directory not found.")
    endif()

    if(NOT EXISTS ${SDL2_IMAGE_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2_image include directory not found.")
    endif()

    # Link SDL2 and SDL2_image libraries for macOS
    target_include_directories(main PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(main ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

# Linux (WSL) configuration
elseif(UNIX)
    # Set SDL2 paths for Linux (WSL)
    set(SDL2_LIBRARY "/usr/lib/x86_64-linux-gnu/libSDL2.so")  # Path for SDL2 on Ubuntu
    set(SDL2_IMAGE_LIBRARY "/usr/lib/x86_64-linux-gnu/libSDL2_image.so")  # Path for SDL2_image on Ubuntu

    set(SDL2_INCLUDE_DIR "/usr/include/SDL2")  # Path for SDL2 headers
    set(SDL2_IMAGE_INCLUDE_DIR "/usr/include/SDL2")  # Path for SDL2_image headers (if needed)

    # Check if the paths were found
    if(NOT EXISTS ${SDL2_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2 include directory not found.")
    endif()

    if(NOT EXISTS ${SDL2_IMAGE_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2_image include directory not found.")
    endif()

    # Link SDL2 and SDL2_image libraries for Linux (WSL)
    target_include_directories(main PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(main ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY})

# Windows configuration
elseif(WIN32)
    # Specify the path to g++ explicitly
    set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe")
    set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe")  # Optionally set the C compiler too

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mconsole")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mconsole")

    # Set paths for SDL2 and SDL2_image on Windows using .lib files for MinGW
    set(SDL2_LIBRARY "${CMAKE_SOURCE_DIR}/Dependencies/SDL/x64/SDL2.lib")  # Adjust path for MinGW
    set(SDL2_IMAGE_LIBRARY "${CMAKE_SOURCE_DIR}/Dependencies/SDL/x64/SDL2_image.lib")  # Adjust path for MinGW
    set(SDL2_MAIN_LIBRARY "${CMAKE_SOURCE_DIR}/Dependencies/SDL/x64/SDL2main.lib")  # Adjust path for MinGW"

    # Include directories for SDL2 and SDL2_image
    set(SDL2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dependencies/SDL")  # Adjust path for your system
    set(SDL2_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/Dependencies/SDL")  # Adjust path for your system

    # Check if the paths were found
    if(NOT EXISTS ${SDL2_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2 include directory not found.")
    endif()

    if(NOT EXISTS ${SDL2_IMAGE_INCLUDE_DIR})
        message(FATAL_ERROR "SDL2_image include directory not found.")
    endif()

    # Link SDL2 and SDL2_image libraries for Windows
    target_include_directories(main PRIVATE ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})
    target_link_libraries(main ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MAIN_LIBRARY})

endif()
