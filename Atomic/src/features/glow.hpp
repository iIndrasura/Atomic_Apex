#ifndef GLOW_HPP
#define GLOW_HPP

#include <iostream>
#include <cstdio>
#include <string>

#include "../core/handling.hpp"
#include "../core/math.hpp"
#include "../config/configvalues.hpp"

#include "../player/level.hpp"
#include "../player/localplayer.hpp"
#include "../player/player.hpp"
#include "../player/weapon.hpp"
//#include "aimbot.hpp"

class Aimbot;

class Glow {

private:
    
    //Aimbot* aimbot;
    Aimbot& aimbot;

    LocalPlayer localplayerClass;
    Player playerClass;
    Level levelClass;
    Weapon weaponClass;
    //Aimbot aimbot;

    int iTeamControl;
    int iLocControl;

    struct VisibilityDataStr {
        float lastTime;
        bool lastState;
        uintptr_t lastCheck;
    };

    float lastvis_aim[70];
    int loopsSinceLastVisible[70];
    QWORD target_entity;

    VisibilityDataStr entityVisibilityData[70];

    bool IsEntityVisible(rx_handle process, QWORD player, VisibilityDataStr& visibilityData);	

public:
    Glow(Aimbot& aimbotRef);  // Constructor
    ~Glow(); // Destructor

    //Aimbot* aimbot; // Declare a pointer to Aimbot
    // method to set the aimbot pointer
    //void SetAimbot(Aimbot* aimbotPtr);

    bool shouldGlowEnable(rx_handle process);
    void ActivateGlow(rx_handle process, QWORD ClientEntityList, QWORD Sensitivity, int GameMode/* , int TeamNum, int iWeapon, int lifeState */);
};

#endif // GLOW_HPP
