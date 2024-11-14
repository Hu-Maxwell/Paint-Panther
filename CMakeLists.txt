cmake_minimum_required(VERSION 3.5.0)
project(paintapp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

if (APPLE AND EXISTS "/opt/homebrew")
    set(HOMEBREW_PREFIX "/opt/homebrew")
else()
    set(HOMEBREW_PREFIX "/usr/local")
endif()

set(SFML_DIR "${HOMEBREW_PREFIX}/lib/cmake/SFML")
set(CURL_DIR "${HOMEBREW_PREFIX}")
set(NLOHMANN_JSON_DIR "${HOMEBREW_PREFIX}/share/nlohmann_json")

find_package(SFML COMPONENTS system window graphics REQUIRED PATHS ${SFML_DIR})
find_package(CURL REQUIRED PATHS ${CURL_DIR})
find_package(nlohmann_json REQUIRED PATHS ${NLOHMANN_JSON_DIR})

set(SOURCE_FILES
    "Paint Panther/src/main.cpp" 
    "Paint Panther/src/paintapp.cpp" 
    "Paint Panther/src/ui.cpp"
    "Paint Panther/src/api.cpp"
    "Paint Panther/src/shapes.cpp"
    "Paint Panther/src/tools.cpp"
    "Paint Panther/src/user.cpp"
)
set(HEADER_FILES
    "Paint Panther/include/paintapp.h" 
    "Paint Panther/include/ui.h"
    "Paint Panther/include/apiHandler.h"
)

add_executable(paintapp ${SOURCE_FILES} ${HEADER_FILES})

add_compile_definitions(PROJECT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")

add_custom_command(
    TARGET paintapp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/"Paint Panther"/assets
        $<TARGET_FILE_DIR:paintapp>/assets
)

target_link_libraries(paintapp PRIVATE sfml-system sfml-window sfml-graphics CURL::libcurl nlohmann_json::nlohmann_json)
