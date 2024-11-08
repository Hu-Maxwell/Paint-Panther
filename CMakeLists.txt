cmake_minimum_required(VERSION 3.5.0)
project(paintapp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake where to find SFML
set(SFML_DIR "C:/SFML-2.6.1/lib/cmake/SFML")  # Adjust path if needed

# Find the SFML package with specific components
find_package(SFML COMPONENTS system window graphics REQUIRED)

# Add the executable and include all source files with their relative path
add_executable(paintapp 
    "Paint Panther/src/main.cpp" 
    "Paint Panther/src/paintapp.cpp" 
    "Paint Panther/src/ui.cpp"
    "Paint Panther/include/paintapp.h" 
    "Paint Panther/include/ui.h"
)

# Link SFML libraries to the project
target_link_libraries(paintapp PRIVATE sfml-system sfml-window sfml-graphics)