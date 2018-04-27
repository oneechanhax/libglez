# This builds a 32-bit version of library, building 64 bit is not supported yet.

cmake_minimum_required(VERSION 3.0)
project(glez LANGUAGES C VERSION 0.0.1 DESCRIPTION "Basic OpenGL primitive + text drawing using freetype-gl")

set(glez_dest "lib/glez-${glez_VERSION}")
set(include_dest "include/glez-${glez_VERSION}")
set(lib_dest "${glez_dest}/${CMAKE_BUILD_TYPE}")

find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(GLEW REQUIRED)

add_library(glez SHARED "")

set_target_properties(glez PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_include_directories(glez PRIVATE 
    $<BUILD_INTERFACE:${glez_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${glez_SOURCE_DIR}/ftgl>
    $<BUILD_INTERFACE:${FREETYPE_INCLUDE_DIRS}>
    $<BUILD_INTERFACE:${PNG_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${GLEW_INCLUDE_DIRS}>
    $<INSTALL_INTERFACE:${include_dest}>
)

target_compile_definitions(glez PRIVATE ${PNG_DEFINITIONS})

target_link_libraries(glez ${FREETYPE_LIBRARIES} ${PNG_LIBRARIES} ${GLEW_LIBRARIES})

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(ftgl)

install(TARGETS glez EXPORT glez DESTINATION "${lib_dest}")
install(FILES "include/glez.h" DESTINATION "${include_dest}")
install(EXPORT glez DESTINATION "${lib_dest}")
install(FILES glez-config.cmake DESTINATION ${glez_dest})