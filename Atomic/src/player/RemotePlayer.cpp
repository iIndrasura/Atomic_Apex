#include "RemotePlayer.hpp"

/* RemotePlayers::RemotePlayers(Level* level, int* counter) {
    m_counter = *counter;
    m_lastUpdated = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_PLAYERS; i++) {
        QWORD playerValue(i);
        m_players.push_back(playerValue); 
    }
} */

/* RemotePlayers::RemotePlayers( counter) {
    m_counter = *counter; // Assuming counter is an int pointer
    m_lastUpdated = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player player(i, m_counter); // Initialize Player with entityListIndex and m_counter
        m_players.push_back(player);
    }
}
 */
RemotePlayers::RemotePlayers(Level* level, int* counter) : m_counter(*counter) {
    m_lastUpdated = std::chrono::steady_clock::now();

    for (int i = 0; i < MAX_PLAYERS; i++) {
        QWORD playerValue(i, m_counter);
        m_players.push_back(playerValue); 
    }
}


void RemotePlayers::markForPointerResolution(rx_handle process) {
    auto& players = getPlayers(process);
    for (auto& player : players) {
        playerClass.markForPointerResolution();
    }
}

std::vector<QWORD>& RemotePlayers::getPlayers(rx_handle process) {
    if (levelClass.isTrainingArea(process)) {
        auto now = std::chrono::steady_clock::now();
        if (now - m_lastUpdated > std::chrono::seconds(1) || std::any_of(m_players.begin(), m_players.end(), [](auto& player) { return !playerClass.isValid(); })) {
            findTrainingAreaPlayers(process, m_trainingAreaPlayers);
            m_lastUpdated = now;
        }
        return m_trainingAreaPlayers;
    } else {
        m_trainingAreaPlayers.clear();
        return m_players;
    }
}

void RemotePlayers::findTrainingAreaPlayers(rx_handle process, std::vector<QWORD>& result) {
    result.clear();
    for (int i = 0; i < 20000; i++) {
        Player player(i, m_counter); // Initialize Player with entityListIndex and m_counter
        if (playerClass.isNPC(process)) {
            // Get the QWORD value from the Player object and push it into the result vector
            QWORD playerValue = playerClass.getPlayerPointer(process);
            result.push_back(playerValue);
        }
    }
}

