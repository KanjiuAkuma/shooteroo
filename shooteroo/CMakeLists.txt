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

if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(STATUS "Using WIN32")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
    add_executable(shooteroo WIN32 ${SHOOTEROO_FILES})
else()
    add_executable(shooteroo ${SHOOTEROO_FILES})
endif()
target_include_directories(shooteroo PUBLIC src)
target_link_libraries(shooteroo PRIVATE JEngine)

add_custom_command(TARGET shooteroo PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/shooteroo/resources $<TARGET_FILE_DIR:shooteroo>/resources)