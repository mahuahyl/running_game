cmake_minimum_required(VERSION 3.16)
project(RunRun LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SFML_DIR "/c/msys64/mingw64/lib/cmake/SFML")
find_package(SFML REQUIRED COMPONENTS Graphics Window System)

add_executable(runrun src/main.cpp)

target_link_libraries(runrun PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# === Build ===
# In MSYS2 MINGW64 terminal:
#   cmake -B build -G "MinGW Makefiles"
#   cmake --build build
#   ./bin/runrun.exe
#
# Or with Ninja (faster):
#   cmake -B build -G "Ninja"
#   cmake --build build
