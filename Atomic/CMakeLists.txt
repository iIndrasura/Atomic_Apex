# cmake_minimum_required(VERSION 3.0)

project(Atomic)
set(OUT Atomic)

add_executable ( ${OUT}
    src/main.cpp
    src/core/handling.cpp
    src/core/math.cpp
    src/offsets/offsets.cpp
    src/config/readConfig.cpp
    src/config/configvalues.cpp
    src/player/level.cpp
    src/player/player.cpp
    src/player/localplayer.cpp
    src/player/weapon.cpp
    src/player/input_system.cpp
    src/player/highlights.cpp
    src/features/glow.cpp
    src/features/aimbot.cpp
    src/features/rcs.cpp
    src/features/chams.cpp
    src/features/spec.cpp
    src/features/skin_changer.cpp
    src/features/auto_grapple.cpp
    src/features/radar.cpp
    src/features/item_esp.cpp
    src/features/triggerbot.cpp
    src/ui/menu.cpp
    )


set_target_properties(${OUT} PROPERTIES LINKER_LANGUAGE CXX)

# Optamization flag level 2
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -DNDEBUG -Wno-format-truncation -Wno-strict-aliasing -O2 -s")

# Optamization flag level 3
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -DNDEBUG -Wno-format-truncation -Wno-strict-aliasing -O3 -s")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-format-truncation -Wno-strict-aliasing")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

target_link_libraries(${OUT} rx "-Wall -DNDEBUG -pthread -s -O3")
