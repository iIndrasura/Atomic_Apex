#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <iostream>
#include <string>

#include "../core/handling.hpp"
#include "../offsets/offsets.cpp"

class Level {
    private:
        static QWORD getBasePointer(rx_handle process);
    public:
        static std::string getName(rx_handle process);
        static QWORD getGamemodePointer(rx_handle process);
        static std::string getGamemode(rx_handle process);
        static bool isSpecialMode(rx_handle process);
        static bool isPlayable(rx_handle process);
        static bool isTrainingArea(rx_handle process);
};

#endif // LEVEL_HPP