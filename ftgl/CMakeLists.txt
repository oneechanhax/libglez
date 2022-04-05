target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/freetype-gl.h"
        "${CMAKE_CURRENT_LIST_DIR}/mat4.h"
        "${CMAKE_CURRENT_LIST_DIR}/texture-atlas.h"
        "${CMAKE_CURRENT_LIST_DIR}/texture-font.h"
        "${CMAKE_CURRENT_LIST_DIR}/utf8-utils.h"
        "${CMAKE_CURRENT_LIST_DIR}/vec234.h"
        "${CMAKE_CURRENT_LIST_DIR}/vector.h"
        "${CMAKE_CURRENT_LIST_DIR}/vertex-attribute.h"
        "${CMAKE_CURRENT_LIST_DIR}/vertex-buffer.h")
    
target_sources(glez PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/mat4.c"
        "${CMAKE_CURRENT_LIST_DIR}/texture-atlas.c"
        "${CMAKE_CURRENT_LIST_DIR}/texture-font.c"
        "${CMAKE_CURRENT_LIST_DIR}/utf8-utils.c"
        "${CMAKE_CURRENT_LIST_DIR}/vector.c"
        "${CMAKE_CURRENT_LIST_DIR}/vertex-attribute.c"
        "${CMAKE_CURRENT_LIST_DIR}/vertex-buffer.c")