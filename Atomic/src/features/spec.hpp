#ifndef SPEC_HPP
#define SPEC_HPP

#include <cstdio>
#include <iostream>

#include "../core/handling.hpp"
#include "../config/configvalues.hpp"

#include "../player/player.hpp"
#include "../player/level.hpp"
#include "../player/localplayer.hpp"
#include "../offsets/offsets.cpp"


class SpectatorCount {

private:
    Player playerClass;
    Level levelClass;
    LocalPlayer localplayerClass;

    int previousSpectatorCount;

public:
    SpectatorCount();
    ~SpectatorCount();
    
    // Function to determine whether spectator count should be enabled
    bool shouldSpectatorCountEnable(rx_handle process);
    
    // Function to update and display the spectator count
    void UpdateSpectatorCount(rx_handle process, QWORD ClientEntityList);
};

#endif // SPEC_HPP