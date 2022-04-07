cmake_minimum_required(VERSION 3.0)

project(glez LANGUAGES CXX VERSION 2.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
set(OpenGL_GL_PREFERENCE "GLVND")
find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} SHARED "")

target_include_directories(${PROJECT_NAME} PRIVATE
    "${FREETYPE_INCLUDE_DIRS}"
    "ftgl"
    "${GLEW_INCLUDE_DIRS}"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARIES} ${FREETYPE_LIBRARIES} z GL freetype-gl)

add_subdirectory(src)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION "lib")
install(DIRECTORY include/glez DESTINATION "include")
