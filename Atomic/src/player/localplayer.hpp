#ifndef LOCALPLAYER_HPP
#define LOCALPLAYER_HPP

#include <string>
#include "../core/handling.hpp"
#include "../offsets/offsets.cpp"

typedef unsigned long ulong;
typedef float FLOAT;

class LocalPlayer {
private:
    QWORD m_basePointer = 0;
    QWORD m_bonePointer = 0;

    QWORD getUnresolvedBasePointer();

public:
    QWORD getLocalplayerPointer(rx_handle process);
    
    void markForPointerResolution();

    DWORD getWeaponHandle(rx_handle process);

    QWORD getWeaponEntity(rx_handle process);

    uintptr_t getViewModelHandle(rx_handle process);
    //uintptr_t getViewModelHandle(rx_handle process, QWORD LocalPlayerPtr);
    uintptr_t getViewModelArmHandle(rx_handle process);

    uintptr_t getViewModelPtr(rx_handle process, uintptr_t viewModelHandle);

    int getAmmo(rx_handle process);

    float getTime(rx_handle process);

    float getLocationX(rx_handle process);

    float getLocationY(rx_handle process);

    float getLocationZ(rx_handle process);

    float getCameraZ(rx_handle process);

    int getLocalTeamNumber(rx_handle process);

    int getTeamSpecial(rx_handle process);

    bool isInAttack(rx_handle process);

    int getAttackState(rx_handle process);

    void setAttackState(rx_handle process, int state);

    std::string getName(rx_handle process);

    bool isKnocked(rx_handle process);

    bool isAlive(rx_handle process);

    bool isSkydiving(rx_handle process);

    int getReloadDown(rx_handle process);

    int getReloadState(rx_handle process);

    void setReloadState(rx_handle process, int state);

    float getPunchPitch(rx_handle process);

    float getPunchYaw(rx_handle process);

    float getPitch(rx_handle process);

    float getYaw(rx_handle process);

    bool isZooming(rx_handle process);

    bool isGrounded(rx_handle process);

    float getlocalSpecYaw(rx_handle process);
};

#endif // LOCALPLAYER_HPP