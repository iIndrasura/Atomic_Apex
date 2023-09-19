#include "auto_grapple.hpp"

AutoGrapple::AutoGrapple() {
    // Constructor implementation
}

AutoGrapple::~AutoGrapple() {
    // Destructor implementation
}

bool AutoGrapple::shouldAutoGrappleEnable(rx_handle process) {
    // Check if the AutoGrapple feature should be enabled based on game state and configuration
    if (ConfigValues::AUTO_GRAPPLE == 1 && (levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process))) {
        return true;
    }
    return false;
}

void AutoGrapple::ApplyAutoGrapple(rx_handle process) {
    if (shouldAutoGrappleEnable(process)) 
    {
        QWORD localplayer = localplayerClass.getLocalplayerPointer(process);
        
        rx_write_i32(process, OFFSETS::REGION + OFFSETS::in_jump + 0x8, 4);
        
        // Check if the grapple is attached or not
        auto grappleAttached = rx_read_i32(process, localplayer + OFFSETS::m_grapple + OFFSETS::m_grappleAttached);
        
        if (grappleAttached == 1) 
        {
            // Simulate releasing the grapple
            rx_write_i32(process, OFFSETS::REGION + OFFSETS::in_jump + 0x8, 5);
            // Simulate resetting the grapple activate time
            rx_write_i32(process, OFFSETS::REGION + OFFSETS::m_grappleActivateTime, 1);
        }
    }
}
