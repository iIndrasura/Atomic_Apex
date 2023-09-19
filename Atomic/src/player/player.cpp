#include "player.hpp"

QWORD Player::getUnresolvedBasePointer() {
    QWORD unresolvedBasePointer = OFFSETS::REGION + OFFSETS::ENTITY_LIST + ((m_entityListIndex + 1) << 5);
    return unresolvedBasePointer;
}

QWORD Player::getPlayerPointer(rx_handle process) {
    if (m_basePointer == 0) {
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

QWORD Player::getBonePointer(rx_handle process) {
    if (m_bonePointer == 0) {
        QWORD basePointer = getPlayerPointer(process);
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
    QWORD basePointer = getPlayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::SIGNIFIER_NAME;
    std::string result = rx_read_string(process, ptrLong);
    return result;
}

// Is player NPC
bool Player::isDummy(rx_handle process) {
    if (getSignifierName(process).compare("npc_dummie") == 0)
        return true;
    return false;
}

// Visible time
float Player::getLastVisibleTime(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::LAST_VISIBLE_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

/*
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

/* 
bool Player::IsEntityVisible(rx_handle process, QWORD player, VisibilityDataStr& visibilityData) {
    float visTime = rx_read_float(process, player + OFFSETS::LAST_VISIBLE_TIME);
    
    visibilityData.lastState = visTime > visibilityData.lastTime || (visTime < 0.0f && visibilityData.lastTime > 0.0f);
    visibilityData.lastTime = visTime;

    return visibilityData.lastState;
}
*/

// Entity in loop Name
QWORD Player::getplayerName(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::NAME;
    QWORD result = rx_read_i64(process, ptrLong);
    return result;
}

// Entity in loop spec yaw
float Player::getplayerSpecYaw(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::spec_YAW;
    float result = rx_read_float(process, ptrLong);
    return result;
}

// Entity in loop Shield
int Player::getShield(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::m_shieldHealth;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

// Entity in loop health
int Player::getHealth(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::m_iHealth;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

// Entity in loop bleedout state
int Player::getBleedState(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::m_bleedoutState;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

// Entity in loop life state
int Player::getLifeState(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::LIFE_STATE;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

// Entity in loop Team Number
int Player::getPlayerTeamNum(rx_handle process, QWORD player) {    
    QWORD ptrLong = player + OFFSETS::TEAM_NUMBER;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

/* 
// Entity in crosshair and store the last visible values for all entity in loop. then compare
bool Player::isCrosshair(rx_handle process, QWORD player, LastCrosshairDataStr& tempCrosshairData) {
    float lastCrosshairTime = rx_read_float(process, player + OFFSETS::LAST_CROSSHAIR_TIME);

    // if (tempCrosshairData.m_lastCrosshairTime == -1.f) {
    //     tempCrosshairData.m_lastCrosshairTime = lastCrosshairTime;
    //     //printf("CHECK 1 LAST CROSSHAIR TIME: [%f] \n", tempCrosshairData.m_lastCrosshairTime);
    //     return false;
    // }

    tempCrosshairData.CrosshairlastState = lastCrosshairTime > tempCrosshairData.m_lastCrosshairTime;   //bool value
    tempCrosshairData.m_lastCrosshairTime = lastCrosshairTime;

    return tempCrosshairData.CrosshairlastState;
}
 */
float Player::getLastCrosshairTime(rx_handle process) {
    QWORD basePointer = getPlayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::LAST_CROSSHAIR_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

float Player::getStartCrosshairTime(rx_handle process) {
    QWORD basePointer = getPlayerPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::START_CROSSHAIR_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}

// Entity loop
QWORD Player::GetClientEntity(rx_handle game_process, QWORD entity, QWORD index)
{

	index = index + 1;
	index = index << 0x5;

	return rx_read_i64(game_process, (index + entity) - 0x280050);
}

// Entity Bone Position from Bone Matrix
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
