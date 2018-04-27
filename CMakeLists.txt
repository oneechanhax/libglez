# This builds a 32-bit version of library, building 64 bit is not supported yet.

cmake_minimum_required(VERSION 3.0)
project(glez VERSION 0.0.1 DESCRIPTION "Basic OpenGL primitive + text drawing using freetype-gl")

find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(GLEW REQUIRED)

add_library(glez SHARED "")

set_target_properties(glez PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_include_directories(glez PUBLIC include)
target_include_directories(glez PRIVATE ftgl)
target_include_directories(glez PRIVATE ${FREETYPE_INCLUDE_DIRS} ${PNG_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

target_compile_definitions(glez PRIVATE ${PNG_DEFINITIONS})

target_link_libraries(glez ${FREETYPE_LIBRARIES} ${PNG_LIBRARIES} ${GLEW_LIBRARIES})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(ftgl)