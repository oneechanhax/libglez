target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/glez.c"
        "${CMAKE_CURRENT_LIST_DIR}/program.c"

        "${CMAKE_CURRENT_LIST_DIR}/glez.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/font.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/texture.cpp")

add_subdirectory(detail)