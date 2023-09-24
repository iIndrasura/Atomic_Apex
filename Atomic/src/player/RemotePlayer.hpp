#ifndef REMOTEPLAYERS_HPP
#define REMOTEPLAYERS_HPP

#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

#include "level.hpp"
#include "player.hpp"

class RemotePlayers {

private:
    Player playerClass;
    Level levelClass;
    int m_counter;
    std::vector<QWORD> m_players; // Keep it as QWORD if Player is defined as QWORD
    std::vector<QWORD> m_trainingAreaPlayers; // Keep it as QWORD if Player is defined as QWORD
    std::chrono::steady_clock::time_point m_lastUpdated;

    static const int MAX_PLAYERS = 72;

    void findTrainingAreaPlayers(rx_handle process, std::vector<QWORD>& result); // Corrected the parameter type

public:
    RemotePlayers(Level* level, int counter);
    void markForPointerResolution(rx_handle process);
    std::vector<QWORD>& getPlayers(rx_handle process); // Keep it as QWORD if Player is defined as QWORD
};

#endif // REMOTEPLAYERS_HPP
