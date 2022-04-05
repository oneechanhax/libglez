target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/freetype-gl.h"
        "${CMAKE_CURRENT_LIST_DIR}/mat4.h"
        "${CMAKE_CURRENT_LIST_DIR}/texture-atlas.h"
        "${CMAKE_CURRENT_LIST_DIR}/vertex-attribute.h")
    
target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/mat4.c"
        "${CMAKE_CURRENT_LIST_DIR}/vertex-attribute.c")