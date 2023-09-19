#ifndef RCS_HPP
#define RCS_HPP

#include <iostream>
#include <cstdio>

#include "../core/handling.hpp"
#include "../core/math.hpp"

#include "../config/configvalues.hpp"

#include "../player/level.hpp"
#include "../player/input_system.hpp"
#include "../player/localplayer.hpp"


class NoRecoil {
private:
    // Define any private members or helper functions needed for NoRecoil
    vec3 oldPunch;
    //double m_previousPunchPitch;
    //double m_previousPunchYaw;

    vec3 smoothedAngle;  // smoothedAngle
    const float smoothingFactor;  // smoothingFactor

    LocalPlayer localplayerClass;
    Level levelClass;
    
public:
    NoRecoil();
    ~NoRecoil();

    bool shouldNoRecoil1Enable(rx_handle process, QWORD InputSystem);
    //bool shouldNoRecoil2Enable(rx_handle process, QWORD InputSystem);
    void ApplyNoRecoil1(rx_handle process, QWORD InputSystem);
    //void ApplyNoRecoil2(rx_handle process, QWORD InputSystem);


};

#endif // RCS_HPP