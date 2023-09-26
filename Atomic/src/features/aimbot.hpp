#ifndef AIMBOT_HPP
#define AIMBOT_HPP

#include <iostream>
#include <cstdio>
#include <thread>

#include "../core/handling.hpp"
#include "../core/math.hpp"
#include "../config/configvalues.hpp"

#include "../player/input_system.hpp"
#include "../player/level.hpp"
#include "../player/localplayer.hpp"
#include "../player/player.hpp"
#include "../player/weapon.hpp"
//#include "glow.hpp"

class Glow;

class Aimbot {

private:
    //Glow& glow;
    
    LocalPlayer localplayerClass;
    Level levelClass;
    Player playerClass;
    Weapon weaponClass;
    //Glow glow;

    DWORD previous_tick;
    QWORD target_entity_ = 0;

    MouseMovementData mouseData;

public:
    //Aimbot(Glow& glowRef);
    Aimbot();
    ~Aimbot();

    //Glow* glow;
    //void SetGlow(Glow* glowPtr);

    // Declare a getter method to retrieve the target_entity
    //QWORD GetTargetEntity() const;

    // Declare a setter method to set the target_entity
    //void SetTargetEntity(QWORD target_entity);

    void SetTargetEntity(QWORD entity);

    QWORD GetTargetEntity() const;

    bool shouldAimbotEnable(rx_handle process, QWORD InputSystem);
    void ActivateAimbot(rx_handle process, QWORD InputSystem, QWORD ClientEntityList, QWORD Sensitivity, int PlayerData, int bZooming);
};

#endif 