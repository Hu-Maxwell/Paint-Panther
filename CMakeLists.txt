cmake_minimum_required(VERSION 3.1.0)
project(paintapp)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake where to find SFML
set(SFML_DIR "/usr/local/Cellar/sfml/2.6.2")  # Adjust path if needed

# Find the SFML package with specific components
find_package(SFML COMPONENTS system window graphics REQUIRED)

# Add the executable and include all source files with their relative path
add_executable(paintapp 
    "Paint Panther/src/main.cpp" 
    "Paint Panther/src/paintapp.cpp" 
    "Paint Panther/src/ui.cpp"
    "Paint Panther/src/user.cpp"
    "Paint Panther/include/paintapp.h" 
    "Paint Panther/include/ui.h"
)

# Link SFML libraries to the project
target_link_libraries(paintapp PRIVATE sfml-system sfml-window sfml-graphics)