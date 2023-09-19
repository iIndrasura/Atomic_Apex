#include "glow.hpp"
#include "aimbot.hpp"

Glow::Glow(Aimbot& aimbotRef) : iTeamControl(0), iLocControl(0), target_entity(0), aimbot(aimbotRef) {
    memset(lastvis_aim, 0, sizeof(lastvis_aim));
    memset(entityVisibilityData, 0, sizeof(entityVisibilityData));
    memset(loopsSinceLastVisible, 0, sizeof(loopsSinceLastVisible));

    // Constructor implementation
}

Glow::~Glow() {
    // Destructor implementation
}

// Visibility check
bool Glow::IsEntityVisible(rx_handle process, QWORD player, VisibilityDataStr& visibilityData) {
    float visTime = rx_read_float(process, player + OFFSETS::LAST_VISIBLE_TIME);
    
    visibilityData.lastState = visTime > visibilityData.lastTime || (visTime < 0.0f && visibilityData.lastTime > 0.0f);
    visibilityData.lastTime = visTime;

    return visibilityData.lastState;
}

// Bool to enable glow
bool Glow::shouldGlowEnable(rx_handle process) {
    if ( ConfigValues::GLOW_ENABLED == 1 && (levelClass.isPlayable(process) || levelClass.isSpecialMode(process)) )
		 {
        return true;
    }
    return false;
}

// main
void Glow::ActivateGlow(rx_handle process, QWORD ClientEntityList, QWORD Sensitivity, int GameMode ) 
{
    QWORD localplayer = localplayerClass.getLocalplayerPointer(process);
    
    if (localplayer == 0)
    {
        memset(lastvis_aim, 0, sizeof(lastvis_aim));
        return;
    }

    // local Player Team number
    int LocTeam = localplayerClass.getLocalTeamNumber(process);

    // Weapon in hand
    DWORD weapon_id = localplayerClass.getWeaponHandle(process);
    QWORD weapon = playerClass.GetClientEntity(process, ClientEntityList, weapon_id - 1);

    float bulletSpeed = weaponClass.getBulletSpeed(process, weapon);
    float bulletGravity = weaponClass.getBulletGravity(process, weapon);

    vec3 muzzle;
    rx_read_process(process, localplayer + OFFSETS::m_muzzle, &muzzle, sizeof(vec3));

    QWORD target_entity = 0;
    float target_fov = 360.0f;

    // Bool enable or disable glow
    if (shouldGlowEnable(process))
    {   
        // Entity loop
        for (int i = 0; i < 70; i++)
        {
            QWORD entity = playerClass.GetClientEntity(process, ClientEntityList, i);
            
            // is Entity valid
            if (entity == 0) {
                continue;
            }

            QWORD EntityHandleAddress = playerClass.getplayerName(process, entity);
            
            // Read the string from EntityHandleAddress
            std::string Identifier = rx_read_string(process, EntityHandleAddress);
            const char* IdentifierC = Identifier.c_str();
            
            // Compare the C-style string to "player"
            if (strcmp(IdentifierC, "player") != 0) 
            {
                //Entity team number
                int EntTeam = playerClass.getPlayerTeamNum(process, entity);
                if (EntTeam % 2)
                {
                    iTeamControl = 1;
                }
                else
                {
                    iTeamControl = 2;
                }

                //LocalPlayer Team Number
                if (LocTeam % 2)
                {
                    iLocControl = 1;
                }
                else
                {
                    iLocControl = 2;
                }

                //GameMode check
                if (GameMode == 1953394531)
                {
                    if (iTeamControl == iLocControl)
                        continue;
                }

                // If entity is local player then skip this iteration
                if (entity == localplayer)
                    continue;

                /* 
                if (rx_read_i32(process, entity + m_iName) != 125780153691248)
                {
                    continue;
                }
                */

                // Squard/ Team check: if entity number is equal to Local number then skip this iteration
                if (EntTeam == LocTeam)
                {
                    continue;
                }

                // Entity Health
                if ((playerClass.getHealth(process, entity)) == 0)
                {
                    lastvis_aim[i] = 0;
                    continue;
                }

                // Get entity knocked state
                int entKnockedState = playerClass.getBleedState(process, entity);
                
                if ((playerClass.getLifeState(process, entity)) != 0)
                {
                    lastvis_aim[i] = 0;
                    continue;
                }
                
                // Logic to change entity color when in FOV or AimLock
                vec3 hitbox = playerClass.GetBonePosition(process, entity, 2);

                vec3 velocity;
                rx_read_process(process, entity + OFFSETS::m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

                float fl_time = vec_distance(hitbox, muzzle) / bulletSpeed;
                hitbox.z += (700.0f * bulletGravity * 0.5f) * (fl_time * fl_time);

                velocity.x = velocity.x * fl_time;
                velocity.y = velocity.y * fl_time;
                velocity.z = velocity.z * fl_time;

                hitbox.x += velocity.x;
                hitbox.y += velocity.y -40;
                hitbox.z += velocity.z;

                vec3 target_angle = CalcAngle(muzzle, hitbox);
                
                vec3 breath_angles;
                rx_read_process(process, localplayer + OFFSETS::m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

                // Last Visible time
                float entNewVisTime = playerClass.getLastVisibleTime(process, entity);

                // Get the shield value to change the color of the glow
                int shield = playerClass.getShield(process, entity);

                //Initialize R, G, and B with default values
                int R, G, B;

                // Glow enable
                rx_write_i32(process, entity + 0x3F8, 1);
                rx_write_i32(process, entity + 0x400, 2);
                    
                // Glow Mode
                //rx_write_i32(process, entity + 0x2F4, 1512990053);
                GlowMode glowModeData = { 101, 101, 46, 90 };
                rx_write_array(process, entity + 0x2F4, (char*)&glowModeData, sizeof(GlowMode));

                // Current entity Last Visible check
                bool Visible = IsEntityVisible(process, entity, entityVisibilityData[i]);

                // Local Player Field of View for aimbot
                float fov = get_fov(breath_angles, target_angle);

                // Entity not knocked
                if (entKnockedState == 0)
                {
                    if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0)
                    {
                        if (!Visible) 
                        { 
                            // if not visible, if they have been visible 20 loops ago
                            if (loopsSinceLastVisible[i] < 10) {
                                loopsSinceLastVisible[i]++; 
                            } 
                            else 
                            {
                                rx_write_float(process, entity + 0x3E4, ConfigValues::GLOW_DISTANCE);

                                rx_write_float(process, entity + 0x200, ConfigValues::INVISIBLE_RED_VALUE);   // Red
                                rx_write_float(process, entity + 0x204, ConfigValues::INVISIBLE_GREEN_VALUE); // Green
                                rx_write_float(process, entity + 0x208, ConfigValues::INVISIBLE_BLUE_VALUE);  // Blue

                                continue;
                            }
                        } 
                        else if (Visible) 
                        {
                            loopsSinceLastVisible[i] = 0; //only if truely visible

                            target_fov = fov;
                            target_entity = entity;
                            lastvis_aim[i] = entNewVisTime;

                            rx_write_float(process, entity + 0x3E4, ConfigValues::GLOW_DISTANCE);

                            rx_write_float(process, entity + 0x200, ConfigValues::VISIBLE_RED_VALUE);   // Red
                            rx_write_float(process, entity + 0x204, ConfigValues::VISIBLE_GREEN_VALUE); // Green
                            rx_write_float(process, entity + 0x208, ConfigValues::VISIBLE_BLUE_VALUE);  // Blue

                            // Pass the valid target_entity to the aimbot
                            aimbot.SetTargetEntity(entity);
                        }
                    }
                    // Health Based glow ESP, No Visible Check
                    else if (ConfigValues::GLOW_HEALTH == 1)
                    {
                        // Calculate R, G, B based on shield values
                        if (shield > 100)
                        {
                            R = 3;
                            G = 0;
                            B = 0;
                        }
                        else if (shield < 100 && shield > 75 )
                        {
                            R = 1;
                            G = 0;
                            B = 2;
                        }
                        else if (shield < 76 && shield > 50)
                        {
                            R = 0;
                            G = 1;
                            B = 2;
                        }
                        else if (shield < 51 && shield > 0)
                        {
                            R = 0;
                            G = 3;
                            B = 0;
                        }
                        else
                        {
                            R = 0;
                            G = 2;
                            B = 1;
                        }

                        rx_write_float(process, entity + 0x3E4, ConfigValues::GLOW_DISTANCE);
                            
                        rx_write_float(process, entity + 0x200, R); // Red
                        rx_write_float(process, entity + 0x204, G); // Green
                        rx_write_float(process, entity + 0x208, B); // Blue
                    }
                }
                else
                {
                    rx_write_float(process, entity + 0x3E4, ConfigValues::GLOW_DISTANCE);

                    // If Entity/ Player is in Knockdown State
                    rx_write_float(process, entity + 0x200, 10.0f);   // Red
                    rx_write_float(process, entity + 0x204, 10.0f); // Green
                    rx_write_float(process, entity + 0x208, 10.0f);  // Blue
                }
                /*
                }
                else if (GLOW_SQUARD == 1)
                {	
                    // Apply Color to Team/ Squard
                    rx_write_float(process, entity + 0x200, ConfigValues::squard_redValue);   // Red
                    rx_write_float(process, entity + 0x204, ConfigValues::squard_greenValue); // Green
                    rx_write_float(process, entity + 0x208, ConfigValues::squard_blueValue);  // Blue
                }
                */
            }
        }
    }
}