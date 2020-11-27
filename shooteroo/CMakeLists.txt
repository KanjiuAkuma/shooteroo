set(SHOOTEROO_FILES
    src/main.cpp

    src/Shooteroo.cpp
    src/Shooteroo.h
    src/settings/Definitions.h
    src/settings/GameSettings.cpp
    src/settings/GameSettings.h
    src/Game.h
    src/Game.cpp
    src/base/Entity.h
    src/base/Entity.cpp
    src/base/Actor.h
    src/base/Actor.cpp
    src/entities/Player.h
    src/entities/Player.cpp
    src/entities/Bullet.h
    src/entities/Bullet.cpp
    src/entities/Hostile.h
    src/entities/Hostile.cpp
    src/entities/Projectile.cpp
    src/entities/Projectile.h)

if (CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "Using WIN32")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    add_executable(shooteroo WIN32 ${SHOOTEROO_FILES})
else ()
    add_executable(shooteroo ${SHOOTEROO_FILES})
endif ()

string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_CONFIG)
message(STATUS "build config ${BUILD_CONFIG}")

target_include_directories(shooteroo PUBLIC src)
target_link_libraries(shooteroo PRIVATE JEngine)

# what a mess... copy resources and create release zips
add_custom_command(OUTPUT
                   ${CMAKE_CURRENT_BINARY_DIR}_dummy_copy # dummy to ensure we run
                   ${CMAKE_CURRENT_BINARY_DIR}/resources
                   COMMAND
                   ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/resources
                   ${CMAKE_CURRENT_BINARY_DIR}/resources)

add_custom_command(OUTPUT
                   ${CMAKE_CURRENT_BINARY_DIR}_dummy_zip # dummy to ensure we run
                   ${CMAKE_CURRENT_BINARY_DIR}/shooteroo_${BUILD_CONFIG}.zip
                   COMMAND
                   ${CMAKE_COMMAND} -E tar "cf" "shooteroo_${BUILD_CONFIG}.zip" --format=zip
                   "${CMAKE_CURRENT_BINARY_DIR}/shooteroo.exe"
                   "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_target(copy_resources
                  COMMENT "copying resources"
                  DEPENDS
                  ${CMAKE_CURRENT_BINARY_DIR}_dummy_copy)

add_custom_target(create_zip
                  COMMENT "creating zip"
                  DEPENDS
                  ${CMAKE_CURRENT_BINARY_DIR}_dummy_zip
                  ${CMAKE_CURRENT_BINARY_DIR}/resources
                  ${CMAKE_CURRENT_BINARY_DIR}/shooteroo_${BUILD_CONFIG}.zip
                  shooteroo)

add_dependencies(shooteroo copy_resources)