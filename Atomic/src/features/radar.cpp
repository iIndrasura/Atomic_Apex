#include "radar.hpp"
#include "../core/utils.hpp"

bool Radar::shouldRadarEnable(rx_handle process) {
    if (ConfigValues::RADAR == 1 &&
        (levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process))) {
        
        return true;
    }
    return false;
}

ScanResult Radar::scan(const char* nowStr, rx_handle process, QWORD ClientEntityList) 
{
    ScanResult result {};

    localplayer = localplayerClass.getLocalplayerPointer(process);
    double localX = localplayerClass.getLocationX(process);
    double localY = localplayerClass.getLocationY(process);

    yaw = localplayerClass.getYaw(process);
    LocTeam = localplayerClass.getLocalTeamNumber(process);

    result.minDistance = 999;

    for (int i = 0; i < 70; i++) 
    {
        QWORD player = playerClass.GetClientEntity(process, ClientEntityList, i);

        if (player == 0) {
            continue;
        }
        
        if (player == localplayer) {
            continue;
        }

        EntTeam = playerClass.getPlayerTeamNum(process, player);

        if (EntTeam == LocTeam) {
            continue;
        }

        double targetX = playerClass.getLocationX(process, player);
        double targetY = playerClass.getLocationY(process, player);
        
        double distance = distanceToMeters( calculateDistance2D(localX, localY, targetX, targetY));

        double diffX_ = distanceToMeters(targetX - localX);
        double diffY_ = distanceToMeters(targetY - localY);

        double diffX = 0;
        double diffY = 0;

        convertPointByYaw(yaw, diffX_, diffY_, diffX, diffY);

        int indexX = Distances.size();
        int indexY = Distances.size();

        for (std::size_t j = 0; j < Distances.size(); ++j) {
            if (diffX < Distances[j]) {
                indexX = j;
                break;
            }
        }
        for (std::size_t j = 0; j < Distances.size(); ++j) {
            if (-diffY < Distances[j]) {
                indexY = j;
                break;
            }
        }
        result.enemiesMap[indexY][indexX] += 1;
        result.minDistance = std::min<int>(result.minDistance, distance);
    }
    
    /*
    clearScreen();
    printf("[%s] Min Distance: %d, Radar:\n",
        nowStr,
        result.minDistance);
    for (size_t i = 0; i < result.enemiesMap.size(); ++i) {
        const auto& row = result.enemiesMap[i];
        printf("[ ");
        for (size_t j = 0; j < row.size(); ++j) {
            bool isClose = (i >= 2 && i < 7 && j >= 2 && j < 7);
            int count = row[j];
            if (isClose && count >= 1) {
                printf("\033[0;31m%2d\033[0m ", count);
            } else if (isClose) {
                printf("\033[0;33m%2d\033[0m ", count);
            } else {
                printf("%2d ", count);
            }
        }
        printf("]\n");
    }
    */
    return result;
}


void Radar::UpdateRadar(rx_handle process, QWORD ClientEntityList) {
    if (shouldRadarEnable(process)) {
        // Get the current time and format it as "HH:MM:SS"
        
        auto now = std::time(nullptr);
        std::strftime(nowStr.data(), nowStr.size(), "%H:%M:%S", std::localtime(&now));

        // Call the scan function and process the results
        ScanResult result = scan(nowStr.data(), process, ClientEntityList);

        // Get the spectator count using SpectatorCount::getSpectatorCount
        int spectatorCount = specCount.UpdateSpectatorCount(process, ClientEntityList);

        // Print the radar information
        // clearScreen();

        // Save the cursor position and clear the screen
        //printf("\e[s\e[H\e[2J\e[3J");

        // Save the cursor position
        printf("\e[s");

        // Print the spectator count
        if (spectatorCount >= 1) {
            printf("\t\t\033[32mSpectator: %-3d\033[0m\n", spectatorCount); // Green color
        } else {
            printf("\t\tSpectator: %-3d\n", spectatorCount);
        }

        // Determine the color for Min Distance based on new thresholds
        std::string minDistanceColor;
        if (result.minDistance <= 50) {
            minDistanceColor = "\033[0;31m"; // Red color
        } else if (result.minDistance <= 100) {
            minDistanceColor = "\033[0;33m"; // Yellow color
        } else if (result.minDistance <= 200) {
            minDistanceColor = "\033[0;34m"; // Blue color
        } else {
            minDistanceColor = "\033[0;37m"; // White color
        }

        printf("[%s] Min Distance: %s%d\033[0m, Radar:\n", nowStr.data(), minDistanceColor.c_str(), result.minDistance);

        for (size_t i = 0; i < result.enemiesMap.size(); ++i) {
            const auto& row = result.enemiesMap[i];
            printf("[ ");
            for (size_t j = 0; j < row.size(); ++j) {
                bool isClose = (i >= 2 && i < 7 && j >= 2 && j < 7);
                int count = row[j];
                if (isClose && count >= 1) {
                    printf("\033[0;31m%2d\033[0m ", count);
                } else if (isClose) {
                    printf("\033[0;33m%2d\033[0m ", count);
                } else {
                    printf("%2d ", count);
                }
            }
            printf("]\n");
        }
        
        // Restore the cursor position
        printf("\e[u");

        // Clear everything between the saved and restored positions
        printf("\e[J");
    }
}