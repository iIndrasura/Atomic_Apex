#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <iostream>
#include <string>
#include "../core/handling.hpp"
#include "../offsets/offsets.cpp"

class Weapon {
private:
    //int m_entityListIndex;
    //QWORD m_basePointer = 0; // Added to store the base pointer

    //QWORD getUnresolvedBasePointer(rx_handle process);

    //QWORD getWeaponPointer(rx_handle process);

public:
    //Weapon(int entityListIndex, rx_handle process);

    //void markForPointerResolution();

    int getAmmo(rx_handle process, QWORD Weapon);

    //int getAmmoStockpile(rx_handle process);

    bool isSemiAuto(rx_handle process, QWORD Weapon);

    bool isBurst(rx_handle process, QWORD Weapon);

    //float getZoomFOV(rx_handle process);

    float getReadyTime(rx_handle process, QWORD Weapon);

    std::string getName(rx_handle process, QWORD Weapon);

    float getBulletSpeed(rx_handle process, QWORD Weapon);
    float getBulletGravity(rx_handle process, QWORD Weapon);

    //std::string getInvalidReason(rx_handle process);
};

#endif // WEAPON_HPP