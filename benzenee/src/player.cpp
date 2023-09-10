#include "player.h"


QWORD Player::getUnresolvedBasePointer(rx_handle process) {
    QWORD unresolvedBasePointer = OFFSETS::REGION + OFFSETS::m_cl_entitylist + ((m_entityListIndex + 1) << 5);
    return unresolvedBasePointer;
}

QWORD Player::getBasePointer(rx_handle process) {
    if (m_basePointer == 0) {
        m_basePointer = rx_read_i64(process, getUnresolvedBasePointer(process));
    }
    return m_basePointer;
}

QWORD Player::getBonePointer(rx_handle process) {
    if (m_bonePointer == 0) {
        QWORD basePointer = getBasePointer(process);
        m_bonePointer = rx_read_i64(process, basePointer + OFFSETS::BONES);
    }
    return m_bonePointer;
}

float Player::getBoneZ(rx_handle process, int id) {
    QWORD bonePointer = getBonePointer(process);
    uint32_t boneLoc = (id * 0x30);
    QWORD address = bonePointer + boneLoc + 0xCC + 0x4 + 0xC + 0x4 + 0xC;
    float result = rx_read_float(process, address);
    return result;
}

std::string Player::getSignifierName(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::SIGNIFIER_NAME;
    std::string result = rx_read_string(process, ptrLong);
    return result;
}

bool Player::isDummy(rx_handle process) {
    if (getSignifierName(process).compare("npc_dummie") == 0)
        return true;
    return false;
}

/*
// Visible time
float Player::getLastVisibleTime(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LAST_VISIBLE_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

// is visible or not
// Here we calculating the Last Visible
bool Player::isVisible(rx_handle process, bool update) {
    if (update) {
        const float lastVisibleTime = getLastVisibleTime(process);
        Visibility = lastVisibleTime > m_lastVisibleTime;
        m_lastVisibleTime = lastVisibleTime;
        return Visibility;
    } else {
        return Visibility;
    }
}
*/

// 
bool Player::isVisible(float lastVisibleTime, bool update) {
    if (update) {
        Visibility = (lastVisibleTime > m_lastVisibleTime) || (lastVisibleTime < 0.0f && m_lastVisibleTime > 0.0f);
        m_lastVisibleTime = lastVisibleTime;
        return Visibility;
    } else {
        return Visibility;
    }
}

bool Player::IsEntityVisible(rx_handle process, QWORD player, VisibilityDataStr& visibilityData) {
    float visTime = rx_read_float(process, player + OFFSETS::LAST_VISIBLE_TIME);
    
    visibilityData.lastState = visTime > visibilityData.lastTime || (visTime < 0.0f && visibilityData.lastTime > 0.0f);
    visibilityData.lastTime = visTime;

    return visibilityData.lastState;
}

float Player::getLastCrosshairTime(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LAST_CROSSHAIR_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float Player::getStartCrosshairTime(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    QWORD ptrLong = basePointer + OFFSETS::START_CROSSHAIR_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

bool Player::isCrosshair(rx_handle process, QWORD player, LastCrosshairDataStr& tempCrosshairData) {
    float lastCrosshairTime = rx_read_float(process, player + OFFSETS::LAST_CROSSHAIR_TIME);

    /*
    if (tempCrosshairData.m_lastCrosshairTime == -1.f) {
        tempCrosshairData.m_lastCrosshairTime = lastCrosshairTime;
        //printf("CHECK 1 LAST CROSSHAIR TIME: [%f] \n", tempCrosshairData.m_lastCrosshairTime);
        return false;
    }
    */

    tempCrosshairData.CrosshairlastState = lastCrosshairTime > tempCrosshairData.m_lastCrosshairTime;   //bool value
    tempCrosshairData.m_lastCrosshairTime = lastCrosshairTime;

    return tempCrosshairData.CrosshairlastState;
}

/*
bool Player::isCrosshair(rx_handle process) {
    const float lastCrosshairTime = getLastCrosshairTime(process);

    if (m_lastCrosshairTime == -1.f)
        {
            m_lastCrosshairTime = lastCrosshairTime;
            printf("CHECK 1 LAST CROSSHAIR TIME: [%f] \n", m_lastCrosshairTime );
            return false;
        }

    bool isCrosshair = lastCrosshairTime > m_lastCrosshairTime;
    m_lastCrosshairTime = lastCrosshairTime;
    return isCrosshair;
}
*/

// Entity loop
QWORD Player::GetClientEntity(rx_handle game_process, QWORD entity, QWORD index)
{

	index = index + 1;
	index = index << 0x5;

	return rx_read_i64(game_process, (index + entity) - 0x280050);
}

// Entity/ Player Bone Position from Bone Matrix
vec3 Player::GetBonePosition(rx_handle game_process, QWORD entity_address, int index)
{
	vec3 position;
	rx_read_process(game_process, entity_address + OFFSETS::m_vecAbsOrigin, &position, sizeof(position));

	QWORD bonematrix = rx_read_i64(game_process, entity_address + OFFSETS::BONE_MATRIX);

	matrix3x4_t matrix;
	rx_read_process(game_process, bonematrix + (0x30 * index), &matrix, sizeof(matrix3x4_t));

	vec3 bonepos;
	bonepos.x = matrix.x + position.x;
	bonepos.y = matrix.y + position.y;
	bonepos.z = matrix.z + position.z;

	return bonepos;
}
