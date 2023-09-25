#include "spec.hpp"

SpectatorCount::SpectatorCount() : previousSpectatorCount(-1) {
    // Constructor implementation
}

SpectatorCount::~SpectatorCount() {
    // Destructor implementation
}

bool SpectatorCount::shouldSpectatorCountEnable(rx_handle process) {
    // Check if spectator count should be enabled based on game state and configuration
    if (ConfigValues::SPECTATOR_COUNT == 1 && (/* levelClass.isTrainingArea(process) || */ levelClass.isPlayable(process) || levelClass.isSpecialMode(process))) {
        return true;
    }
    return false;
}

int SpectatorCount::UpdateSpectatorCount(rx_handle process, QWORD ClientEntityList) 
{
    if (shouldSpectatorCountEnable(process)) 
    {
        int spectatorCount = 0;

        for (int j = 0; j < 70; j++) 
        {
            QWORD player = playerClass.GetClientEntity(process, ClientEntityList, j);

            if (player == 0) {
                continue;
            }
            
            float targetAngle = playerClass.getplayerSpecYaw(process, player);

            float targetYaw = -targetAngle; // yaw is inverted
            if (targetYaw < 0)
                targetYaw += 360;
            targetYaw += 90; // yaw is off by 90
            if (targetYaw > 360)
                targetYaw -= 360;

            float localAngle = localplayerClass.getlocalSpecYaw(process);

            float localYaw = -localAngle; // yaw is inverted
            if (localYaw < 0)
                localYaw += 360;
            localYaw += 90; // yaw is off by 90
            if (localYaw > 360)
                localYaw -= 360;

            if (targetYaw == localYaw && playerClass.getHealth(process, player) == 0)
            //if (targetYaw == localYaw && rx_read_i32(process, player + OFFSETS::m_iHealth) == 0)
                spectatorCount++;
        }
        /* 
        // Check if the value has changed
        if (spectatorCount != previousSpectatorCount) 
        {
            // Calculate the length of the text to be cleared (including ANSI escape codes)
            int textLength = snprintf(NULL, 0, "\033[32mSpectator: %-3d\033[0m", spectatorCount);

            // Clear the entire line by overwriting with spaces
            printf("\r%-*s", textLength, " ");

            // Print the updated value with the desired color
            if (spectatorCount >= 1) {
                printf("\033[32mSpectator: %-3d\033[0m", spectatorCount); // Green color
            } else {
                printf("Spectator: %-3d", spectatorCount);
            }

            previousSpectatorCount = spectatorCount;
            fflush(stdout);
        }
        */
        if (spectatorCount != previousSpectatorCount) {
            previousSpectatorCount = spectatorCount;
            return spectatorCount;      // Return spectatorCount only when it has changed
        }

        return previousSpectatorCount;
    }

    return previousSpectatorCount == 0;      // Return the previous value if there was no change
}

