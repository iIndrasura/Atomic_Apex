#include "localplayer.h"

QWORD LocalPlayer::getUnresolvedBasePointer() {
    QWORD unresolvedBasePointer = OFFSETS::REGION + OFFSETS::m_localplayer;
    return unresolvedBasePointer;
}

QWORD LocalPlayer::getBasePointer(rx_handle process) {
    if (m_basePointer == 0)
        m_basePointer = rx_read_i64(process, getUnresolvedBasePointer());
    return m_basePointer;
}

void LocalPlayer::markForPointerResolution() {
    m_basePointer = 0;
}

int LocalPlayer::getWeaponHandle(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::iWEAPON;
    ulong wepHandle = rx_read_i64(process, ptrLong);
    return wepHandle & 0xFFFF;
}

QWORD LocalPlayer::getWeaponEntity(rx_handle process) {
    int wepHandle = getWeaponHandle(process);
    QWORD unresolvedBasePointer = OFFSETS::REGION + OFFSETS::m_cl_entitylist + (wepHandle << 5);
    return rx_read_i64(process, unresolvedBasePointer);
}

int LocalPlayer::getAmmo(rx_handle process) {
    QWORD basePointer = getWeaponEntity(process);
    QWORD ptrLong = basePointer + OFFSETS::m_ammoInClip;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

float LocalPlayer::getTime(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getLocationX(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LOCAL_ORIGIN;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getLocationY(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LOCAL_ORIGIN + sizeof(float);
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getLocationZ(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LOCAL_ORIGIN + (sizeof(float) * 2);
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getCameraZ(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_muzzle + (sizeof(float) * 2);
    float result = rx_read_float(process, ptrLong);
    return result;
}

int LocalPlayer::getTeamNumber(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::TEAM_NUMBER;
    int result = rx_read_int(process, ptrLong);
    return result;
}

int LocalPlayer::getTeamSpecial(rx_handle process) {
    int teamNumber = getTeamNumber(process);
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
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::NAME;
    std::string result = rx_read_string(process, ptrLong);
    return result;
}

bool LocalPlayer::isKnocked(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_bleedoutState;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

bool LocalPlayer::isDead(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LIFE_STATE;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

bool LocalPlayer::isSkydiving(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
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
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iAimPunch;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getPunchYaw(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iAimPunch + sizeof(float);
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getPitch(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iViewAngles;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float LocalPlayer::getYaw(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iViewAngles + sizeof(float);
    float result = rx_read_float(process, ptrLong);
    return result;
}

bool LocalPlayer::isZooming(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::m_iViewAngles;
    short result = rx_read_i32(process, ptrLong);
    return result > 0;
}

bool LocalPlayer::isGrounded(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::FLAGS;
    uint32_t result = rx_read_i32(process, ptrLong);
    return (result & 0x1) != 0;
}

float LocalPlayer::getSpecYaw(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::spec_YAW;
    float result = rx_read_float(process, ptrLong);
    return result;
}
