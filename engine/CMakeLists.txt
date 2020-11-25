set(ENGINE_FILES
    src/core/application/Application.cpp
    src/core/application/Application.h
    src/core/codes/KeyCodes.h
    src/core/codes/MouseCodes.h
    src/core/imGuiLayer/ImGuiLayer.cpp
    src/core/imGuiLayer/ImGuiLayer.h
    src/core/layer/Layer.cpp
    src/core/layer/Layer.h
    src/core/layerStack/LayerStack.cpp
    src/core/layerStack/LayerStack.h
    src/core/logger/Logger.cpp
    src/core/logger/Logger.h
    src/core/util/OpenGlDebug.cpp
    src/core/util/OpenGlDebug.h
    src/core/util/Shader.cpp
    src/core/util/Shader.h
    src/core/window/Input.h
    src/core/window/Window.h
    src/core/events/Event.h
    src/core/events/ApplicationEvent.h
    src/core/events/KeyEvent.h
    src/core/events/MouseEvent.h
    src/platform/windows/WindowsInput.cpp
    src/platform/windows/WindowsInput.h
    src/platform/windows/WindowsWindow.cpp
    src/platform/windows/WindowsWindow.h
    src/Engine.h
    src/Macros.h
    src/EngineUtil.h
    src/core/util/Random.cpp
    src/core/util/Random.h
    src/core/renderer/texture/Texture.cpp
    src/core/renderer/texture/Texture.h

    vendor/stb_image/stb_image.h
    vendor/stb_image/stb_image.cpp
    vendor/mini/ini.h
    )

set(ENGINE_INCLUDES
    src
    vendor/Glad/include
    vendor/glfw/include
    vendor/imgui
    vendor/spdlog/src
    vendor/glm
    vendor/stb_image
    vendor/mini
    )

add_library(JEngine STATIC ${ENGINE_FILES})
target_include_directories(JEngine PUBLIC ${ENGINE_INCLUDES})

add_subdirectory(vendor/Glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glm)

add_definitions(-DGLFW_INCLUDE_NONE)

target_link_libraries(JEngine PUBLIC Glad)
target_link_libraries(JEngine PUBLIC glfw)
target_link_libraries(JEngine PUBLIC imgui)
target_link_libraries(JEngine PUBLIC spdlog)
target_link_libraries(JEngine PUBLIC glm)
