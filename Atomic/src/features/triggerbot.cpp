#include "triggerbot.hpp"

TriggerBot::TriggerBot() : bTrigger_cache(false) {
    // Initialize with zero
    memset(temp_crosshairData, 0, sizeof(temp_crosshairData));
    memset(loopsSinceLastCrosshair, 0, sizeof(loopsSinceLastCrosshair));
}

TriggerBot::~TriggerBot() {
    // Destructor implementation
}

// Entity in crosshair and store the last visible values for all entity in loop. then compare
bool TriggerBot::isCrosshair(rx_handle process, QWORD player, LastCrosshairDataStr& tempCrosshairData) {
    float lastCrosshairTime = rx_read_float(process, player + OFFSETS::LAST_CROSSHAIR_TIME);

    /*
    if (tempCrosshairData.m_lastCrosshairTime == -1.f) {
        tempCrosshairData.m_lastCrosshairTime = lastCrosshairTime;
        //printf("CHECK 1 LAST CROSSHAIR TIME: [%f] \n", tempCrosshairData.m_lastCrosshairTime);
        return false;
    }
    */

    tempCrosshairData.CrosshairlastState = lastCrosshairTime > tempCrosshairData.m_lastCrosshairTime;   //bool value
    tempCrosshairData.m_lastCrosshairTime = lastCrosshairTime;

    return tempCrosshairData.CrosshairlastState;
}

bool TriggerBot::shouldTriggerBotEnable(rx_handle process) {
    // Check if the TriggerBot feature should be enabled based on game state and configuration
    if (ConfigValues::TRIGGER_ENABLED == 1 && (levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process))) {
        return true;
    }
    return false;
}

void TriggerBot::ApplyTriggerBot(rx_handle process, QWORD ClientEntityList, QWORD InputSystem, rx_handle mouseInput) 
{
    if (shouldTriggerBotEnable(process)) 
    {
        DWORD weaponId = localplayerClass.getWeaponHandle(process);
        QWORD weapon = playerClass.GetClientEntity(process, ClientEntityList, weaponId - 1);

        for (int p = 0; p < 70; p++) 
        {
            QWORD pEntity = playerClass.GetClientEntity(process, ClientEntityList, p);

            // is Entity valid
            if (pEntity == 0) {
                continue;
            }

            // Team Number check
            int entTeam = playerClass.getPlayerTeamNum( process,  pEntity);
            int locTeam = localplayerClass.getLocalTeamNumber(process);
            
            if (entTeam == locTeam) {
                continue;
            }

            int entKnocked = playerClass.getBleedState(process, pEntity);

            if(entKnocked != 0 && ConfigValues::TB_IGNORE_KNOCKED == 1){
                continue;
            }

            bool crosshair = isCrosshair(process, pEntity, temp_crosshairData[p]);
            
            auto tmpTriggerBot = false;

            if (!crosshair) 
            { 
                // If not Crosshair
                if (loopsSinceLastCrosshair[p] < 5) 
                {
                    loopsSinceLastCrosshair[p]++;
                    tmpTriggerBot = false;
                }
            } 
            else if (crosshair && (IsButtonDown(process, InputSystem, ConfigValues::TRIGGER_KEY))) 
            {
                loopsSinceLastCrosshair[p] = 0;

                int ammoInClip = weaponClass.getAmmo(process, weapon);
                float readyTime = weaponClass.getReadyTime(process, weapon);

                if (ammoInClip > 0 && readyTime == 0) 
                {
                    rx_send_input_key(mouseInput, RX_BTN_LEFT, true);
                    tmpTriggerBot = true;
                } else {
                    tmpTriggerBot = false;
                }
            }

            if (bTrigger_cache == true && tmpTriggerBot == false) 
            {
                rx_send_input_key(mouseInput, RX_BTN_LEFT, false);
            }

            bTrigger_cache = tmpTriggerBot;
        }
    }
}


// Mouse Event For in Attack offset
// Mem::Write<uint32_t>(Mem::Base() + OFFSET_IN_ATTACK + 0x8, 4);
// Sleep(1);
// Mem::Write<uint32_t>(Mem::Base() + OFFSET_IN_ATTACK + 0x8, 5);
// Sleep(1);
// Mem::Write<uint32_t>(Mem::Base() + OFFSET_IN_ATTACK + 0x8, 4);