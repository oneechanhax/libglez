target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/color.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/glez.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/font.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/texture.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/draw.hpp")

add_subdirectory(detail)