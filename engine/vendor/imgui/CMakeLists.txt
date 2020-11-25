set(IMGUI_FILES
    imconfig.h
    imgui.h
    imgui.cpp
    imgui_draw.cpp
    imgui_internal.h
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    imgui_demo.cpp
    examples/imgui_impl_glfw.cpp
    examples/imgui_impl_glfw.h
    examples/imgui_impl_opengl3.cpp
    examples/imgui_impl_opengl3.h
    )

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)
add_library(imgui STATIC ${IMGUI_FILES})

target_include_directories(imgui PUBLIC
                           ${PROJECT_SOURCE_DIR}/engine/vendor/imgui
                           ${PROJECT_SOURCE_DIR}/engine/vendor/glad/include
                           ${PROJECT_SOURCE_DIR}/engine/vendor/glfw/include)