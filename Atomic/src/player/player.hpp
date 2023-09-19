#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <chrono>
#include <string>

#include "../core/handling.hpp"
#include "../offsets/offsets.cpp"
#include "../core/math.hpp"

class Player {
private:
    bool Visibility = false;
    int m_entityListIndex;
    float m_lastVisibleTime;
    float m_lastCrosshairTime;
    QWORD m_basePointer = 0;
    QWORD m_bonePointer = 0;

    QWORD getUnresolvedBasePointer();
    QWORD getPlayerPointer(rx_handle process);
    QWORD getBonePointer(rx_handle process);

public:
    float getBoneZ(rx_handle process, int id);
    std::string getSignifierName(rx_handle process);
    bool isDummy(rx_handle process);
    
    float getLastVisibleTime(rx_handle process, QWORD player);
    //bool isVisible(rx_handle process, bool update);
    bool isVisible(float lastVisibleTime, bool update);

    // Visibility Check for glow
    //bool IsEntityVisible(rx_handle process, QWORD player, VisibilityDataStr& visibilityData);

    float getLastCrosshairTime(rx_handle process);
    float getStartCrosshairTime(rx_handle process);
    //bool isCrosshair(rx_handle process);
    //bool isCrosshair(rx_handle process, QWORD player, LastCrosshairDataStr& tempCrosshairData);

    QWORD getplayerName(rx_handle process, QWORD player);
    float getplayerSpecYaw(rx_handle process, QWORD player);
    int getShield(rx_handle process, QWORD player);
    int getHealth(rx_handle process, QWORD player);
    int getBleedState(rx_handle process, QWORD player);
    int getLifeState(rx_handle process, QWORD player);
    int getPlayerTeamNum(rx_handle process, QWORD player);

    QWORD GetClientEntity(rx_handle game_process, QWORD entity, QWORD index);
    vec3 GetBonePosition(rx_handle game_process, QWORD entity_address, int index);
};

#endif // PLAYER_HPP