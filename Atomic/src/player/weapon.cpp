#include "weapon.hpp"

/* 
QWORD Weapon::getUnresolvedBasePointer(rx_handle process) {
    QWORD unresolvedBasePointer = rx_read_i64(process, OFFSETS::REGION + OFFSETS::ENTITY_LIST + (m_entityListIndex << 5));
    return unresolvedBasePointer;
}

QWORD Weapon::getWeaponPointer(rx_handle process) {
    if (m_basePointer == 0) {
        m_basePointer = rx_read_i64(process, getUnresolvedBasePointer(process));
    } else {
        // If m_basePointer is not zero, check if it needs resetting
        QWORD expectedValue = rx_read_i64(process, getUnresolvedBasePointer(process));
        if (m_basePointer != expectedValue) {
            // Reset m_basePointer if it doesn't match the expected value
            m_basePointer = expectedValue;
        }
    }
    return m_basePointer;
}
 */
/* 
Weapon::Weapon(int entityListIndex, rx_handle process) {
    m_entityListIndex = entityListIndex;
    process = process;
    m_basePointer = 0; // Initialize the base pointer to 0
}
*/
/* 
void Weapon::markForPointerResolution() {
    m_basePointer = 0;
}
*/

int Weapon::getAmmo(rx_handle process, QWORD Weapon) {
    QWORD ptrLong = Weapon + OFFSETS::AMMO_IN_CLIP;
    int result = rx_read_i32(process, ptrLong);
    return result;
}

/* 
int Weapon::getAmmoStockpile(rx_handle process) {
    QWORD basePointer = getWeaponPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::AMMO_STOCKPILE;
    int result = rx_read_i32(process, ptrLong);
    return result;
}
*/
bool Weapon::isSemiAuto(rx_handle process, QWORD Weapon) {
    QWORD ptrLong = Weapon + OFFSETS::SEMI_AUTO;
    short result = rx_read_i16(process, ptrLong);
    return result > 0;
}

bool Weapon::isBurst(rx_handle process, QWORD Weapon) {
    QWORD ptrLong = Weapon + OFFSETS::BURST;
    int result = rx_read_i32(process, ptrLong);
    return result > 0;
}
/* 
float Weapon::getZoomFOV(rx_handle process) {
    QWORD basePointer = getWeaponPointer(process);
    QWORD ptrLong = basePointer + OFFSETS::ZOOM_FOV;
    float result = rx_read_float(process, ptrLong);
    return result;
}
*/
float Weapon::getReadyTime(rx_handle process, QWORD Weapon) {
    QWORD ptrLong = Weapon + OFFSETS::READY_TIME;
    float result = rx_read_float(process, ptrLong);
    return result;
}


std::string Weapon::getName(rx_handle process, QWORD Weapon) {
    //QWORD basePointer = getWeaponPointer(process);
    QWORD ptrLong = Weapon + OFFSETS::NAME;
    std::string result = rx_read_string(process, ptrLong);
    return result;
}

float Weapon::getBulletSpeed(rx_handle process, QWORD Weapon) {
    QWORD ptrLong = Weapon + OFFSETS::m_bulletSpeed;
    int result = rx_read_float(process, ptrLong);
    return result;
}

float Weapon::getBulletGravity(rx_handle process, QWORD Weapon) {
    QWORD ptrLong = Weapon + OFFSETS::m_bulletGravity;
    int result = rx_read_float(process, ptrLong);
    return result;
}

/* 
std::string Weapon::getInvalidReason(rx_handle process) {
    if (getWeaponPointer(process) == 0)
        return "Unresolved base pointer";
    else
        return "Weapon is valid";
}
*/