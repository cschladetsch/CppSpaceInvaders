cmake_minimum_required(VERSION 3.20)
project(CppSpaceInvaders)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SDL3 - for manually built SDL3
find_package(SDL3 REQUIRED)
if(NOT SDL3_FOUND)
    message(FATAL_ERROR "SDL3 not found. Please specify SDL3_DIR if SDL3 is installed in a non-standard location.")
endif()

# Find SDL3_ttf
find_package(SDL3_ttf QUIET)
if(NOT SDL3_ttf_FOUND)
    message(STATUS "SDL3_ttf not found, will use alternative text rendering")
    add_definitions(-DNO_SDL_TTF)
else()
    message(STATUS "Found SDL3_ttf: ${SDL3_ttf_LIBRARIES}")
endif()

# Print SDL3 information
message(STATUS "Found SDL3:")
message(STATUS "  SDL3_INCLUDE_DIRS: ${SDL3_INCLUDE_DIRS}")
message(STATUS "  SDL3_LIBRARIES: ${SDL3_LIBRARIES}")
message(STATUS "  SDL3_VERSION: ${SDL3_VERSION}")

# Define source files based on actual file structure
set(SOURCE_FILES
    src/main.cpp
    src/Game.cpp
    src/TextRenderer.cpp
    game/Graphics.cpp
    Transform.cpp
    Entity/Bullet.cpp
    Entity/Enemy.cpp
    Entity/Player.cpp
    Entity/Barrier.cpp
    Entity/UFO.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity
    ${SDL3_INCLUDE_DIRS}
)

# Link SDL3 libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL3_LIBRARIES}
)

# Link SDL3_ttf if found
if(SDL3_ttf_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL3_ttf_LIBRARIES}
    )
endif()

# Set startup project in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Enable solution folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Output executable to bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)

# Copy assets directory to build output
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Print configuration summary
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
