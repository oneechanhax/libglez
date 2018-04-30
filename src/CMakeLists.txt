target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/draw.c"
        "${CMAKE_CURRENT_LIST_DIR}/fonts.c"
        "${CMAKE_CURRENT_LIST_DIR}/glez.c"
        "${CMAKE_CURRENT_LIST_DIR}/program.c"
        "${CMAKE_CURRENT_LIST_DIR}/textures.c"

        "${CMAKE_CURRENT_LIST_DIR}/glez.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/texture.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/render.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/program.cpp")