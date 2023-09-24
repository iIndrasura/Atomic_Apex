#include "level.hpp"

// Level to check gamemode
// Get basic info about the player and check player is in game or not

QWORD Level::getBasePointer(rx_handle process) {
    return OFFSETS::REGION + OFFSETS::LEVEL;
}

std::string Level::getName(rx_handle process) {
    QWORD basePointer = getBasePointer(process);
    return rx_read_string(process, basePointer);
}

QWORD Level::getGamemodePointer(rx_handle process) {
    return OFFSETS::REGION + OFFSETS::GAMEMODE;
}

std::string Level::getGamemode(rx_handle process) {
    QWORD gamemodePointer = rx_read_i64(process, getGamemodePointer(process) + 0x58);
    return rx_read_string(process, gamemodePointer);
}

// Special Game Mode
bool Level::isSpecialMode(rx_handle process) {
    std::string gamemode = getGamemode(process);
    return (gamemode.compare("control") == 0 || gamemode.compare("freedm") == 0);
}

// In lobby check
bool Level::isPlayable(rx_handle process) {
    std::string name = getName(process);
    return (!name.empty() && name.compare("mp_lobby") != 0);
}

/*
bool inLobby(rx_handle process, QWORD localPlayer) {
    // Read the level name
    std::string levelName = rx_read_string(process, OFFSETS::REGION + OFFSETS::LEVEL);

    // Check if the level name is empty or if it's "mp_lobby"
    if (levelName.empty() || levelName.compare("mp_lobby") == 0) {
        return true; // In lobby
    }

    // If the level name is not empty and not "mp_lobby," assume the player is in a game
    return false; // In a game
}
*/

// In training or firing range check
bool Level::isTrainingArea(rx_handle process) {
    std::string name = getName(process);
    return (name.compare("mp_rr_canyonlands_staging_mu1") == 0) || (name.compare("mp_rr_canyonlands_staging") == 0);
}

