cmake_minimum_required(VERSION 3.0)
project(benzenee)
set(OUT benzenee)

add_executable ( ${OUT}
    src/main.cpp
    src/handling.cpp
    src/math.cpp
    src/offsets.cpp
    src/readConfig.cpp
    src/level.cpp
    src/player.cpp
    src/localplayer.cpp
    src/menu.cpp
    src/configvalues.cpp
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
