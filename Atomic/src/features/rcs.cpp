#include "rcs.hpp"

NoRecoil::NoRecoil() : oldPunch({ 0.0f, 0.0f, 0.0f }), smoothedAngle({ 0.0f, 0.0f, 0.0f }), smoothingFactor(1.5)/* , m_previousPunchPitch(0), m_previousPunchYaw(0) */ {
    //m_previousPunchPitch = 0.0f;
    //m_previousPunchYaw = 0.0f;
    // tested smoothingFactor = 0.09, 1.5
}

NoRecoil::~NoRecoil() {
    // Destructor implementation
}

bool NoRecoil::shouldNoRecoil1Enable(rx_handle process, QWORD InputSystem) {
    if (ConfigValues::NORECOIL_ENABLED == 1 && !localplayerClass.isKnocked(process) && levelClass.isPlayable(process)) {
        if((IsButtonDown(process, InputSystem, ConfigValues::AIMKEY) || IsButtonDown(process, InputSystem, ConfigValues::AIMKEY2))){

            return true;
        }
    }
    return false;
}

/* 
bool NoRecoil::shouldNoRecoil2Enable(rx_handle process, QWORD InputSystem) {
    if (ConfigValues::NORECOIL2_ENABLED == 1 && localplayerClass.isKnocked(process)) {
        if((IsButtonDown(process, InputSystem, ConfigValues::AIMKEY) || IsButtonDown(process, InputSystem, ConfigValues::AIMKEY2))){

            return true;
        }
    }
    return false;
}
*/

void NoRecoil::ApplyNoRecoil1(rx_handle process, QWORD InputSystem)
{
    QWORD localplayer = localplayerClass.getLocalplayerPointer(process);

    if (shouldNoRecoil1Enable(process, InputSystem)) 
    {
        vec3 viewAngles;
        viewAngles.x = rx_read_float(process, localplayer + OFFSETS::m_iViewAngles);
        viewAngles.y = rx_read_float(process, localplayer + OFFSETS::m_iViewAngles + sizeof(float));
        viewAngles.z = rx_read_float(process, localplayer + OFFSETS::m_iViewAngles + 2 * sizeof(float));

        vec3 punchAngle;
        punchAngle.x = rx_read_float(process, localplayer + OFFSETS::m_iAimPunch);
        punchAngle.y = rx_read_float(process, localplayer + OFFSETS::m_iAimPunch + sizeof(float));
        punchAngle.z = rx_read_float(process, localplayer + OFFSETS::m_iAimPunch + 2 * sizeof(float));

        vec3 newAngle;
        newAngle.x = viewAngles.x + (oldPunch.x - punchAngle.x) * (ConfigValues::NORECOIL_STRENGTH / 100.f);
        newAngle.y = viewAngles.y + (oldPunch.y - punchAngle.y) * (ConfigValues::NORECOIL_STRENGTH / 100.f);
        newAngle.z = viewAngles.z;

        // Smooth the new angle using EMA
        smoothedAngle.x = smoothingFactor * newAngle.x + (1 - smoothingFactor) * smoothedAngle.x;
        smoothedAngle.y = smoothingFactor * newAngle.y + (1 - smoothingFactor) * smoothedAngle.y;

        vec_clamp(&newAngle);

        rx_write_float(process, localplayer + OFFSETS::m_iViewAngles, newAngle.x);
        rx_write_float(process, localplayer + OFFSETS::m_iViewAngles + sizeof(float), newAngle.y);
        rx_write_float(process, localplayer + OFFSETS::m_iViewAngles + 2 * sizeof(float), newAngle.z);

        oldPunch.x = punchAngle.x;
        oldPunch.y = punchAngle.y;
        oldPunch.z = punchAngle.z;
    } 
    else 
    {
        // Reset values when the condition is false
        oldPunch = { 0.0f, 0.0f, 0.0f };
    }
}

/* 
void NoRecoil::ApplyNoRecoil2(rx_handle process, QWORD InputSystem) 
{
    QWORD localplayer = localplayerClass.getLocalplayerPointer(process);

    if (shouldNoRecoil2Enable(process, InputSystem)) 
    {
        float punchPitch = rx_read_float(process, localplayer + OFFSETS::m_iAimPunch);
        if (punchPitch != 0) {
            double pitch = rx_read_float(process, localplayer + OFFSETS::m_iViewAngles);
            double punchPitchDelta = (punchPitch - m_previousPunchPitch) * ConfigValues::norecoilPitchStrength;
            if (pitch - punchPitchDelta > 89 || pitch - punchPitchDelta < -89) {
                return;
            }

            rx_write_float(process, localplayer + OFFSETS::m_iViewAngles, pitch - punchPitchDelta);
            m_previousPunchPitch = punchPitch;
        }

        float punchYaw = rx_read_float(process, localplayer + OFFSETS::m_iAimPunch + sizeof(float));
        if (punchYaw != 0) {
            double yaw = rx_read_float(process, localplayer + OFFSETS::m_iViewAngles + sizeof(float));
            double punchYawDelta = (punchYaw - m_previousPunchYaw) * ConfigValues::norecoilYawStrength;
            if (yaw - punchYawDelta > 180 || yaw - punchYawDelta < -180) {
                return;
            }

            rx_write_float(process, localplayer + OFFSETS::m_iViewAngles + sizeof(float), yaw - punchYawDelta);
            m_previousPunchYaw = punchYaw;
        }
    }
    else
    {
        m_previousPunchPitch = 0.0f;
        m_previousPunchYaw = 0.0f;
    }
}
*/