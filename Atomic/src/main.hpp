// Atomic-Apex.h : Include file for standard system include files,
// or project specific include files.

#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iomanip>
#include <random>

//#include <stdint.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/extensions/XTest.h>
//#include <unistd.h>

#include "../../rx/rx.h"

#include "core/handling.hpp"
#include "core/math.hpp"
#include "core/pid.hpp"

#include "features/aimbot.hpp"
#include "features/glow.hpp"
#include "features/triggerbot.hpp"
#include "features/rcs.hpp"
#include "features/item_esp.hpp"
#include "features/chams.hpp"
#include "features/spec.hpp"
#include "features/skin_changer.hpp"
#include "features/auto_grapple.hpp"

#include "offsets/offsets.cpp"

#include "config/configvalues.hpp"
#include "config/readConfig.hpp"

#include "player/input_system.hpp"
#include "player/level.hpp"
#include "player/localplayer.hpp"
#include "player/player.hpp"
#include "player/weapon.hpp"

#include "ui/menu.hpp"

// TODO: Reference additional headers your program requires here.

#endif // MAIN_HPP