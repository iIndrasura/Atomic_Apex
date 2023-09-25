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
#include "../player/highlights.hpp"
//#include "aimbot.hpp"

class Aimbot;

class Glow {

private:
    
    //Aimbot* aimbot;
    Aimbot& aimbot;

    // Class Instance
    LocalPlayer localplayerClass;
    Player playerClass;
    Level levelClass;
    Weapon weaponClass;
    //Aimbot aimbot;

    Highlights highlightClass;      // glow 2 test

    // Vis Check
    struct VisibilityDataStr {
        float lastTime;
        bool lastState;
        uintptr_t lastCheck;
    };

    VisibilityDataStr entityVisibilityData[70];

    float lastvis_aim[70];
    int loopsSinceLastVisible[70];

    // Aimbot entity
    QWORD target_entity;

    // Highlights 
    typedef struct {
    uint8_t InsideFunction;     // InsideFunction
    uint8_t OutlineFunction;    // OutlineFunction: HIGHLIGHT_OUTLINE_OBJECTIVE
    uint8_t OutlineRadius;      // OutlineRadius: size * 255 / 8
    uint8_t VisState;           // (EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)
    } HighlightBits;

    // Variables
    int iTeamControl;
    int iLocControl;

    uint32_t FunctionParameterIndexId;
    uint32_t ContextID;

    // std::chrono::steady_clock::time_point m_lastUpdated = {};
    // std::vector<QWORD> m_trainingAreaPlayers;
    // std::vector<QWORD> entities;

    // Functions
    bool IsEntityVisible(rx_handle process, QWORD player, VisibilityDataStr& visibilityData);

public:
    Glow(Aimbot& aimbotRef);    // Constructor
    ~Glow();                    // Destructor

    bool shouldGlowEnable(rx_handle process);
    void ActivateGlow(rx_handle process, QWORD ClientEntityList, QWORD Sensitivity, int GameMode/* , int TeamNum, int iWeapon, int lifeState */);

    //bool shouldGlow2Enable(rx_handle process);
    //void ActivateGlow2(rx_handle process, QWORD ClientEntityList, QWORD Sensitivity, int GameMode );

    // Training Area
    //bool shouldTrainingGlowEnable(rx_handle process);
    //void ActivateTrainingGlow(rx_handle process );
};

#endif // GLOW_HPP
