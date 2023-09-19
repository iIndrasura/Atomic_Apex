#include "skin_changer.hpp"

SkinChanger::SkinChanger() {
    // Constructor implementation
}

SkinChanger::~SkinChanger() {
    // Destructor implementation
}

bool SkinChanger::shouldSkinChangerEnable(rx_handle process) {
    if (ConfigValues::SKIN_CHANGER == 1 &&
        (levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process)) 
        && localplayerClass.isGrounded(process) 
        /* && localplayerClass.isAlive(process) && !localplayerClass.isKnocked(process) */) {
        
        return true;
    }
    return false;
}

void SkinChanger::ApplySkinChanger(rx_handle process, QWORD ClientEntityList) 
{
    if (shouldSkinChangerEnable(process)) 
    {
        DWORD weaponId = localplayerClass.getWeaponHandle(process);
        QWORD weapon = playerClass.GetClientEntity(process, ClientEntityList, weaponId - 1);
        QWORD localplayer = localplayerClass.getLocalplayerPointer(process);

        rx_write_i32(process, weapon + OFFSETS::nSKIN, ConfigValues::WeaponSkinID);
        rx_write_i32(process, localplayer + OFFSETS::nSKIN, ConfigValues::PlayerSkinID);
    }
}
