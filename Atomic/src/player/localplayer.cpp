#include "localplayer.hpp"

QWORD LocalPlayer::getUnresolvedBasePointer() {
    QWORD unresolvedBasePointer = OFFSETS::REGION + OFFSETS::m_localplayer;
    return unresolvedBasePointer;
}

QWORD LocalPlayer::getLocalplayerPointer(rx_handle process) {
    if (m_basePointer == 0) {
        // If m_basePointer is zero, calculate and store the value
        m_basePointer = rx_read_i64(process, getUnresolvedBasePointer());
    } else {
        // If m_basePointer is not zero, check if it needs resetting
        QWORD expectedValue = rx_read_i64(process, getUnresolvedBasePointer());
        if (m_basePointer != expectedValue) {
            // Reset m_basePointer if it doesn't match the expected value
            m_basePointer = expectedValue;
        }
    }
    return m_basePointer;
}

void LocalPlayer::markForPointerResolution() {
    m_basePointer = 0;
}

DWORD LocalPlayer::getWeaponHandle(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::iWEAPON;
    ulong wepHandle = rx_read_i32(process, ptrLong);
    return wepHandle & 0xFFFF;
}

QWORD LocalPlayer::getWeaponEntity(rx_handle process) {
    int wepHandle = getWeaponHandle(process);
    QWORD unresolvedBasePointer = OFFSETS::REGION + OFFSETS::ENTITY_LIST + (wepHandle << 5);
    return rx_read_i64(process, unresolvedBasePointer);
}
/////
//View Model Handle
uintptr_t LocalPlayer::getViewModelHandle(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    
    QWORD viewModelHandleAddress = basePointer + OFFSETS::m_hViewModels;
    //QWORD viewModelHandleAddress = LocalPlayerPtr + OFFSETS::m_hViewModels;
        
    ulong viewModelHandle = rx_read_i64(process, viewModelHandleAddress);
    return viewModelHandle & 0xFFFF;
}

//View Model Arms
uintptr_t LocalPlayer::getViewModelArmHandle(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD viewModelArmHandleAddress = basePointer + OFFSETS::arm_ViewModels;
    ulong viewModelArmHandle = rx_read_i64(process, viewModelArmHandleAddress);
    return viewModelArmHandle & 0xFFFF;
}


//View Model Pointer
uintptr_t LocalPlayer::getViewModelPtr(rx_handle process, uintptr_t viewModelHandle) {
    QWORD entitylistAddress = OFFSETS::REGION + OFFSETS::ENTITY_LIST + viewModelHandle * 0x20;
    uintptr_t viewModelPtr = rx_read_i64(process, entitylistAddress);
    return viewModelPtr;
}
////

int LocalPlayer::getAmmo(rx_handle process) {
    QWORD basePointer = getWeaponEntity(process);
    QWORD ptrLong = basePointer + OFFSETS::AMMO_IN_CLIP;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

float LocalPlayer::getTime(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getLocationX(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LOCAL_ORIGIN;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getLocationY(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LOCAL_ORIGIN + sizeof(float);
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getLocationZ(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LOCAL_ORIGIN + (sizeof(float) * 2);
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getCameraZ(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_muzzle + (sizeof(float) * 2);
    float result = rx_read_float(process, ptrLong);
    return result;
}

int LocalPlayer::getLocalTeamNumber(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::TEAM_NUMBER;
    int result = rx_read_int(process, ptrLong);
    return result;
}

int LocalPlayer::getTeamSpecial(rx_handle process) {
    int teamNumber = getLocalTeamNumber(process);
    if (teamNumber % 2) {
        return 1;
    } else {
        return 2;
    }
}

bool LocalPlayer::isInAttack(rx_handle process) {
    QWORD ptrLong = OFFSETS::REGION + OFFSETS::in_Attack;
    int result = rx_read_int(process, ptrLong);
    return result > 0;
}

int LocalPlayer::getAttackState(rx_handle process) {
    QWORD ptrLong = OFFSETS::REGION + OFFSETS::in_Attack + 0x8;
    int result = rx_read_int(process, ptrLong);
    return result;
}

// State are 5 is on and 4 is Off
void LocalPlayer::setAttackState(rx_handle process, int state) {
    QWORD ptrLong = OFFSETS::REGION + OFFSETS::in_Attack + 0x8;
    rx_write_i32(process, ptrLong, state);
}

std::string LocalPlayer::getName(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::NAME;
    std::string result = rx_read_string(process, ptrLong);
    return result;
}

bool LocalPlayer::isKnocked(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_bleedoutState;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

bool LocalPlayer::isAlive(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LIFE_STATE;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

bool LocalPlayer::isSkydiving(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::SKYDIVE_STATE;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

int LocalPlayer::getReloadDown(rx_handle process) {
    QWORD ptrLong = OFFSETS::REGION + OFFSETS::IN_RELOAD;
    int result = rx_read_int(process, ptrLong);
    return result;
}

int LocalPlayer::getReloadState(rx_handle process) {
    QWORD ptrLong = OFFSETS::REGION + OFFSETS::IN_RELOAD + 0x8;
    int result = rx_read_int(process, ptrLong);
    return result;
}

// State are 5 is on and 4 is Off
void LocalPlayer::setReloadState(rx_handle process, int state) {
    QWORD ptrLong = OFFSETS::REGION + OFFSETS::IN_RELOAD + 0x8;
    rx_write_i32(process, ptrLong, state);
}

float LocalPlayer::getPunchPitch(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iAimPunch;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getPunchYaw(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iAimPunch + sizeof(float);
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getPitch(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iViewAngles;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getYaw(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iViewAngles + sizeof(float);
    float result = rx_read_float(process, ptrLong);
    return result;
}

bool LocalPlayer::isZooming(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iViewAngles;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

bool LocalPlayer::isGrounded(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::FLAGS;
    uint32_t result = rx_read_i32(process, ptrLong);
    return (result & 0x1) != 0;
}

//Spec Yaw for local player
float LocalPlayer::getlocalSpecYaw(rx_handle process) {
    QWORD basePointer = getLocalplayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::spec_YAW;
    float result = rx_read_float(process, ptrLong);
    return result;
}