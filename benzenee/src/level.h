#pragma once

#include <iostream>
#include <string>

#include "handling.h"
#include "offsets.cpp"

class Level {
    public:
        static QWORD getBasePointer(rx_handle process);
        static std::string getName(rx_handle process);
        static QWORD getGamemodePointer(rx_handle process);
        static std::string getGamemode(rx_handle process);
        static bool isSpecialMode(rx_handle process);
        static bool isPlayable(rx_handle process);
        static bool isTrainingArea(rx_handle process);
};