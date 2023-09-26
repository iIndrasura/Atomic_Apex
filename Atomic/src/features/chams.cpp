#include "chams.hpp"

Chams::Chams() {
    // Constructor implementation
}

Chams::~Chams() {
    // Destructor implementation
}

bool Chams::shouldChamsEnable(rx_handle process) {
    // Check if the player is in a valid game state (training area, playable, or special mode)
    if (!(levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process))) {
        return false; // Not in a valid game state
    }

    // Check if the local player is alive and not knocked
    // if (!localplayerClass.isAlive(process) || localplayerClass.isKnocked(process)) {
    //     return false; // Local player is not in the correct state
    // }

    //All conditions are met, enable weapon chams
    return true;
}

void Chams::ApplyChamsLogic(rx_handle process) {
    bool chamsEnabled = (ConfigValues::CHAMS_ENABLED == 1) && shouldChamsEnable(process);

    if (chamsEnabled) {
        float chamsRedValue = ConfigValues::CHAMS_RED_VALUE;
        float chamsGreenValue = ConfigValues::CHAMS_GREEN_VALUE;
        float chamsBlueValue = ConfigValues::CHAMS_BLUE_VALUE;

        if (ConfigValues::WEAPON_CHAMS == 1) {
            uintptr_t ViewModelHandle = localplayerClass.getViewModelHandle(process);
            uintptr_t ViewModelPtr = localplayerClass.getViewModelPtr(process, ViewModelHandle);
            ApplyChamsToViewModel(process, ViewModelPtr, chamsRedValue, chamsGreenValue, chamsBlueValue);
        }

        if (ConfigValues::ARMS_CHAMS == 1) {
            uintptr_t ViewModelArmHandle = localplayerClass.getViewModelArmHandle(process);
            uintptr_t ViewModelArm = localplayerClass.getViewModelPtr(process, ViewModelArmHandle);
            ApplyChamsToViewModel(process, ViewModelArm, chamsRedValue, chamsGreenValue, chamsBlueValue);
        }
    }
}

void Chams::ApplyChamsToViewModel(rx_handle process, uintptr_t viewModelPtr, float red, float green, float blue) {
    // rx_write_i32(process, viewModelPtr + 0x3F8, 1);
    // rx_write_i32(process, viewModelPtr + 0x400, 2);

    // //GlowMode glowModeData = { 101, 101, 80, 0 };
    // GlowMode glowModeData = { 101, 101, ConfigValues::CHAM_BORDER, 0 };
    // rx_write_array(process, viewModelPtr + 0x2F4, (char*)&glowModeData, sizeof(GlowMode));

    // rx_write_float(process, viewModelPtr + 0x200, red);
    // rx_write_float(process, viewModelPtr + 0x204, green);
    // rx_write_float(process, viewModelPtr + 0x208, blue);

    uint32_t ContextID = highlightClass.GetHighlightCurrentContext(process, viewModelPtr);
    highlightClass.SetHighlightCurrentContext(process, viewModelPtr, ContextID);      // Glow set context ID aka Enable
    highlightClass.SetHighlightVisibilityType(process, viewModelPtr, 2); 

    uint32_t FunctionParameterIndexId = 66;
    highlightClass.SetHighlightActiveState(process, viewModelPtr, ContextID, FunctionParameterIndexId);
    highlightClass.SetHighlightFunctions(process, viewModelPtr, FunctionParameterIndexId, 0, 125, ConfigValues::CHAM_BORDER, true, 0, false);

    // set color    { 1.0f, 0.0f, 0.0f }
    Color color = { red, green, blue };
    highlightClass.SetHighlightParameter(process, viewModelPtr, FunctionParameterIndexId, &color.r);
}


