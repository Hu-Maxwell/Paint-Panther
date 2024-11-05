cmake_minimum_required(VERSION 3.5.0)
project(paintapp VERSION 0.1.0 LANGUAGES C CXX)

# Set VCPKG path (optional, adjust according to your setup)
set(VCPKG_DIR "C:/VCPKG/vcpkg/installed/x64-windows")

# Include directories
include_directories(
    ${VCPKG_DIR}/include
    "${CMAKE_SOURCE_DIR}/Paint Panther/src" # Include your source directory
)

# Link directories
link_directories(
    ${VCPKG_DIR}/lib
)

# Find SFML and its components
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Add the executable and include all source files with their relative path
add_executable(paintapp 
    "Paint Panther/src/main.cpp" 
    "Paint Panther/src/paintapp.cpp" 
    "Paint Panther/src/ui.cpp"
    "Paint Panther/include/paintapp.h" 
    "Paint Panther/include/ui.h"
)

# Link SFML libraries to the paintapp target
target_link_libraries(paintapp PRIVATE sfml-system sfml-window sfml-graphics)

# If you want SFML to provide an implementation of main() (for Windows)
target_link_libraries(paintapp PRIVATE sfml-main)