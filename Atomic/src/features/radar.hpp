#ifndef RADAR_HPP
#define RADAR_HPP

#include <array>
#include <string>

#include <iostream>
#include <cstdio>
#include <thread>

#include "../core/handling.hpp"
#include "../core/math.hpp"

#include "../config/configvalues.hpp"

#include "../player/level.hpp"
#include "../player/localplayer.hpp"
#include "../player/player.hpp"

#include "spec.hpp"

struct ScanResult {
    int minDistance = 0;
    std::array<std::array<int, 9>, 9> enemiesMap{};
};

static const std::array<int, 8> Distances { -200, -100, -50, -5, 5, 50, 100, 200 };

class Radar {

    private:
        LocalPlayer localplayerClass;
        Level levelClass;
        Player playerClass;

        SpectatorCount specCount;

        std::array<char, 32> nowStr;

        int EntTeam;
        int LocTeam;
        double yaw;
        QWORD localplayer;

    public:
        bool shouldRadarEnable(rx_handle process);
        void UpdateRadar(rx_handle process, QWORD ClientEntityList);
        ScanResult scan(const char* nowStr, rx_handle process, QWORD ClientEntityList);
};

#endif 
