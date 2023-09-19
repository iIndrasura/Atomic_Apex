#ifndef SKIN_CHANGER_HPP
#define SKIN_CHANGER_HPP

#include <iostream>
#include <cstdio>

#include "../core/handling.hpp"
#include "../config/configvalues.hpp"

#include "../player/player.hpp"
#include "../player/level.hpp"
#include "../player/localplayer.hpp"
#include "../offsets/offsets.cpp"

class SkinChanger {

private:
    Player playerClass;
    Level levelClass;
    LocalPlayer localplayerClass;
    
public:
    SkinChanger();
    ~SkinChanger();
    
    // Function to determine whether the skin changer should be enabled
    bool shouldSkinChangerEnable(rx_handle process);
    
    // Function to apply the skin changer logic
    void ApplySkinChanger(rx_handle process, QWORD ClientEntityList);
};

#endif // SKIN_CHANGER_HPP