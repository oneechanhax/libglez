# This builds a 32-bit version of library, building 64 bit is not supported yet.

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type")
endif()
set(CMAKE_BUILD_TYPE_VALUES "Debug;Release" CACHE INTERNAL "List of supported build")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_BUILD_TYPE_VALUES})

cmake_minimum_required(VERSION 3.0)

project(glez LANGUAGES C VERSION 0.0.1)

set(export_dest "lib/${PROJECT_NAME}-${PROJECT_VERSION}")
set(include_dest "include/${PROJECT_NAME}-${PROJECT_VERSION}")
set(lib_dest "${export_dest}/${CMAKE_BUILD_TYPE}")

find_package(PNG REQUIRED)
find_package(GLEW REQUIRED)

add_library(${PROJECT_NAME} SHARED "")

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/ftgl>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/freetype/include/freetype2>
    $<BUILD_INTERFACE:${PNG_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${GLEW_INCLUDE_DIRS}>
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PNG_DEFINITIONS})

target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARIES} ${GLEW_LIBRARIES} ${PROJECT_SOURCE_DIR}/freetype/lib/libfreetype.a)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(ftgl)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION "${lib_dest}")
install(FILES "include/glez.h" DESTINATION "${include_dest}")
install(EXPORT ${PROJECT_NAME} DESTINATION "${lib_dest}")
install(FILES ${PROJECT_NAME}-config.cmake DESTINATION ${export_dest})
