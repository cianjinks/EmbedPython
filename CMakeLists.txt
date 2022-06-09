cmake_minimum_required(VERSION 3.3)
project(EmbedPython)

find_package(PythonLibs REQUIRED)
include_directories(src ${PYTHON_INCLUDE_DIRS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE source CONFIGURE_DEPENDS "src/*.h" "src/*.hpp" "src/*.cpp" "src/*.c")

add_executable(EmbedPython ${source})
target_link_libraries(EmbedPython ${PYTHON_LIBRARIES})
