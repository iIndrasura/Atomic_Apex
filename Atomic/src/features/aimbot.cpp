#include "aimbot.hpp"

Aimbot::Aimbot() : target_entity_(0), previous_tick(0) {
    // Initialize member variables
    maxfov_Ads = ConfigValues::AIMFOV_ADS_MAX;
    minfov_Ads = ConfigValues::AIMFOV_ADS_MIN;
    fov_Hipfire = ConfigValues::AIMFOV_HIPFIRE;
    fov_deadzone = ConfigValues::AIMFOV_DEADZONE;
    MaxDistance = ConfigValues::AIMBOT_MAXDISTANCE;
}

Aimbot::~Aimbot() {
    // Destructor implementation
}

// Bool to enable aim
bool Aimbot::shouldAimbotEnable(rx_handle process, QWORD InputSystem) {
    if((IsButtonDown(process, InputSystem, ConfigValues::AIMKEY) || IsButtonDown(process, InputSystem, ConfigValues::AIMKEY2)) && ConfigValues::AIMBOT_ENABLED == 1) {

        return true;
    }
    return false;
}

// main
void Aimbot::ActivateAimbot(rx_handle process, QWORD InputSystem, QWORD ClientEntityList, QWORD Sensitivity, int PlayerData, int bZooming) 
{
    //fovAds = ConfigValues::AIMFOV_ADS;
    //fovHipfire = ConfigValues::AIMFOV_HIPFIRE;
    //fovdeadzone = ConfigValues::AIMFOV_DEADZONE;
    float fovAds = 0.0f;

    QWORD localplayer = localplayerClass.getLocalplayerPointer(process);
    
    if (localplayer == 0)
    {
        previous_tick = 0;
        return;
    }

    float fl_sensitivity = rx_read_float(process, Sensitivity + 0x68);
    DWORD weapon_id = localplayerClass.getWeaponHandle(process);
    QWORD weapon = playerClass.GetClientEntity(process, ClientEntityList, weapon_id - 1);

    float bulletSpeed = weaponClass.getBulletSpeed(process, weapon);
    float bulletGravity = weaponClass.getBulletGravity(process, weapon);

    vec3 muzzle;
    rx_read_process(process, localplayer + OFFSETS::m_muzzle, &muzzle, sizeof(vec3)); //CPlayer!camera_origin

    QWORD target_entity = target_entity_;

    vec3 local_position;
    rx_read_process(process, localplayer + OFFSETS::m_vecAbsOrigin, &local_position, sizeof(vec3));

    if (target_entity && shouldAimbotEnable(process, InputSystem))
    {
        if ((playerClass.getHealth(process, target_entity)) == 0)
            return;
        if ((playerClass.getBleedState(process, target_entity)) > 0) // ignore knock
            return;


        vec3 enmPos;

        rx_read_process(process, localplayer + OFFSETS::m_vecAbsOrigin, &local_position, sizeof(vec3)); // local Player Position
        rx_read_process(process, target_entity + OFFSETS::m_vecAbsOrigin, &enmPos, sizeof(vec3)); // enemy position


        //enmPos.x = 31518;
        //enmPos.y = -6712;
        //enmPos.z = -29235;


        float distance = ((CalcDistance(local_position, enmPos) / 100) * 2);   // need to verify
        //printf("  	distance %f", ((CalcDistance(local_position, enmPos))/100)*2);
        bool far = (distance >= MaxDistance);

        if (far)
        {
            //printf(" Cancelling ");
            return;
        }

        /* --------------------- DYNAMIC FOV -----------------------*/
        // Calculate the scaling factor based on distance
        float distanceFactor = std::min(1.0f, std::max(0.0f, 1.0f - (distance / MaxDistance)));
        // Calculate the scaled fovdeadzone
        float scaledFovDeadzone = fov_deadzone * distanceFactor;

        // Calculate the scaled fovAds within the range of minfovAds and maxfovAds
        float scaledFovAds = minfov_Ads + distanceFactor * (maxfov_Ads - minfov_Ads);

        //printf(" Continue ");

        // prediction
        vec3 target_angle = {0, 0, 0};
        float fov = 360.0f;

        // alteration hitbox
        // int bone_list[] = {2, 3, 5, 8};
        //int bone_list[] = {5,5,5,5}; // chest

        vec3 breath_angles;
        rx_read_process(process, localplayer + OFFSETS::m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

        for (int bone : ConfigValues::BONE_LIST)
        {
            vec3 hitbox = playerClass.GetBonePosition(process, target_entity, bone);

            vec3 velocity;
            rx_read_process(process, target_entity + OFFSETS::m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

            float fl_time = vec_distance(hitbox, muzzle) / bulletSpeed;

            hitbox.z += (700.0f * bulletGravity * 0.5f) * (fl_time * fl_time);

            velocity.x = velocity.x * fl_time;
            velocity.y = velocity.y * fl_time;
            velocity.z = velocity.z * fl_time;

            hitbox.x += velocity.x;
            hitbox.y += velocity.y;
            hitbox.z += velocity.z;

            vec3 angle = CalcAngle(muzzle, hitbox);
            float temp_fov = get_fov(breath_angles, angle);
            if (temp_fov < fov)
            {
                fov = temp_fov;
                target_angle = angle;
            }
        }

        //DWORD weapon_id = rx_read_i32(process, localplayer + m_iWeapon) & 0xFFFF;
        //QWORD weapon = playerClass.GetClientEntity(process, IClientEntityList, weapon_id - 1);
        float zoom_fov = rx_read_float(process, weapon + PlayerData + 0xb8);

        if (rx_read_i8(process, localplayer + bZooming))
        {
            fl_sensitivity = (zoom_fov / 90.0f) * fl_sensitivity;
            fovAds = scaledFovAds;
        }else{
            fovAds = fov_Hipfire;
        }


        if (fov <= fovAds)
        {
            vec3 angles;
            angles.x = breath_angles.x - target_angle.x;
            angles.y = breath_angles.y - target_angle.y;
            angles.z = 0;
            vec_clamp(&angles);

            // Apply DeadZone on angles
            // angles = ApplyDeadzone(angles, fovdeadzone);
            angles = ApplyDeadzone(angles, scaledFovDeadzone);

            float x = angles.y;
            float y = angles.x;
            x = (x / fl_sensitivity) / 0.022f;
            y = (y / fl_sensitivity) / -0.022f;

            float sx = 0.0f, sy = 0.0f;

            float smooth = ConfigValues::AIMSMOOTH;

            DWORD aim_ticks = 0;

            if (smooth >= 1.0f) 
            {
                if (sx < x)
                    sx = sx + 1.0f + (x / smooth);
                else if (sx > x)
                    sx = sx - 1.0f + (x / smooth);
                else
                    sx = x;

                if (sy < y)
                    sy = sy + 1.0f + (y / smooth);
                else if (sy > y)
                    sy = sy - 1.0f + (y / smooth);
                else
                    sy = y;

                aim_ticks = (DWORD)(smooth / 100.0f);
            } else {
                sx = x;
                sy = y;
            }

            /* 
            if (smooth >= 1.0f) 
            {
                sx = (sx < x) ? (sx + 1.0f + (x / smooth)) :
                    (sx > x) ? (sx - 1.0f + (x / smooth)) :
                    x;

                sy = (sy < y) ? (sy + 1.0f + (y / smooth)) :
                    (sy > y) ? (sy - 1.0f + (y / smooth)) :
                    y;

                aim_ticks = (DWORD)(smooth / 100.0f);
            } else {
                sx = x;
                sy = y;
            }
            */
    
            if (qabs((int)sx) > 100)
            return;

            if (qabs((int)sy) > 100)
            return;

            DWORD current_tick = rx_read_i32(process, InputSystem + 0xcd8);
            if (current_tick - previous_tick > aim_ticks)
            {
                previous_tick = current_tick;

                mouseData.x = (int)sx;
                mouseData.y = (int)sy;
                rx_write_process(process, InputSystem + 0x1DB0, &mouseData, sizeof(mouseData));
                //printf(" x %i y %i",data.x,data.y);
                
                std::this_thread::sleep_for(ConfigValues::AIMBOT_SLEEP);
            }
        }
    }  	
}
