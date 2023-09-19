#ifndef TRIGGERBOT_HPP
#define TRIGGERBOT_HPP

#include <iostream>
#include <cstdio>

#include "../core/handling.hpp"
#include "../config/configvalues.hpp"

#include "../player/input_system.hpp"
#include "../player/level.hpp"
#include "../player/localplayer.hpp"
#include "../player/player.hpp"
#include "../player/weapon.hpp"

class TriggerBot {

private:

    Player playerClass;
    Level levelClass;
    LocalPlayer localplayerClass;
    Weapon weaponClass;

    struct LastCrosshairDataStr {
        float m_lastCrosshairTime;
        bool CrosshairlastState;
    };

    int loopsSinceLastCrosshair[70];
    LastCrosshairDataStr temp_crosshairData[70];
    bool bTrigger_cache;

    bool isCrosshair(rx_handle process, QWORD player, LastCrosshairDataStr& tempCrosshairData);

public:
    TriggerBot();
    ~TriggerBot();

    // Function to determine whether the TriggerBot feature should be enabled
    bool shouldTriggerBotEnable(rx_handle process);

    // Function to apply the TriggerBot logic
    void ApplyTriggerBot(rx_handle process, QWORD ClientEntityList, QWORD InputSystem, rx_handle mouseInput);

};

#endif // TRIGGERBOT_HPP