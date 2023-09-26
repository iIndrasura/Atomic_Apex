#include "glow.hpp"
#include "aimbot.hpp"

Glow::Glow(Aimbot& aimbotRef) : iTeamControl(0), iLocControl(0), /* target_entity(0), */ aimbot(aimbotRef) {
    memset(lastvis_aim, 0, sizeof(lastvis_aim));
    memset(entityVisibilityData, 0, sizeof(entityVisibilityData));
    memset(loopsSinceLastVisible, 0, sizeof(loopsSinceLastVisible));

    //FOV_ADS = ConfigValues::AIMFOV_ADS_MAX;
    //FOV_HipFire = ConfigValues::AIMFOV_HIPFIRE;

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

// Glow 2 bool
// Bool to enable glow
bool Glow::shouldGlowEnable(rx_handle process) {
    if ( ConfigValues::GLOW_ENABLED == 1 && (levelClass.isPlayable(process) || levelClass.isSpecialMode(process)) )
		 {
        return true;
    }
    return false;
}

// GLOW 2
void Glow::ActivateGlow(rx_handle process, QWORD ClientEntityList, QWORD Sensitivity, int GameMode ) 
{
    if (shouldGlowEnable(process))
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

        // Training area check

        // if (levelClass.isTrainingArea(process)) {
        //     auto now = std::chrono::steady_clock::now();
            
        //     // Calculate time elapsed since the last update
        //     if (now - m_lastUpdated > std::chrono::seconds(1)) {
        //         // If it's been more than 1 second since the last update, update the entities
        //         m_trainingAreaPlayers.clear();
                
        //         for (int i = 0; i < 20000; i++) {
        //             QWORD entity = playerClass.GetClientEntity(process, ClientEntityList, i);
        //             if (entity != 0 && playerClass.isNPC(process, entity)) {
        //                 // If it's an NPC, store it in the vector
        //                 m_trainingAreaPlayers.push_back(entity);
        //             }
        //         }
                
        //         // Update the last update time
        //         m_lastUpdated = now;
        //     }
        //     // No else condition needed, just use m_trainingAreaPlayers
        // } else {
        //     // Non-training area logic remains the same
        //     for (int i = 0; i < 70; i++) {
        //         QWORD entity = playerClass.GetClientEntity(process, ClientEntityList, i);
        //         entities.push_back(entity);
        //     }
        // }

        // Training area check
        // bool trainingArea = levelClass.isTrainingArea(process);
        // int iterations = trainingArea ? 10000 : 70;               // for (int i = 0; i < iterations; i++)   
        
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
            if (strcmp(IdentifierC, "player") != 0 ) 
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

                // Entity
                int entHealth = playerClass.getHealth(process, entity);

                // Entity Health
                if ( entHealth == 0)
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

                // Local Player Field of View for aimbot
                float fov = get_fov(breath_angles, target_angle);

                // Last Visible time
                float entNewVisTime = playerClass.getLastVisibleTime(process, entity);

                // Get the shield value to change the color of the glow
                int shield = playerClass.getShield(process, entity);

                //Initialize R, G, and B with default values
                float R, G, B;
                //float GlowDistance;
                uint32_t FunctionParameterIndexId;

                // Glow Settings
                uint32_t ContextID = 1;
                highlightClass.SetHighlightCurrentContext(process, entity, ContextID);      // Glow set context ID aka Enable
                highlightClass.SetHighlightVisibilityType(process, entity, 2);              // Glow Vis Type aka Through Walls

                // Current entity Last Visible check
                bool Visible = IsEntityVisible(process, entity, entityVisibilityData[i]);

                // Entity not knocked
                if (entKnockedState == 0)
                {
                    if (!Visible) 
                    {
                        // if not visible, if they have been visible 20 loops ago
                        if (loopsSinceLastVisible[i] < 10) 
                        {
                            loopsSinceLastVisible[i]++;
                        } 
                        else 
                        {
                            target_entity = 0;

                            if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0)
                            {
                                FunctionParameterIndexId = 69;
                                R = ConfigValues::INVISIBLE_RED_VALUE;
                                G = ConfigValues::INVISIBLE_GREEN_VALUE;
                                B = ConfigValues::INVISIBLE_BLUE_VALUE;

                                //GlowDistance = ConfigValues::GLOW_DISTANCE;
                            }
                            else if (ConfigValues::GLOW_HEALTH == 1)
                            {
                                if (shield > 100)
                                {
                                    FunctionParameterIndexId = 70;
                                    R = 0.725f;
                                    G = 0.0f;
                                    B = 0.0f;
                                    // Dark Red
                                }
                                else if (shield <= 100 && shield > 75 )
                                {
                                    FunctionParameterIndexId = 71;
                                    R = 0.5f;
                                    G = 0.0f;
                                    B = 0.5f;
                                    // dark Purple
                                }
                                else if (shield <= 75 && shield > 50)
                                {
                                    FunctionParameterIndexId = 72;
                                    R = 0.2f;
                                    G = 0.2f;
                                    B = 1.0f;
                                    // dark Blue
                                }
                                else if (shield <= 50 && shield > 0)
                                {
                                    FunctionParameterIndexId = 73;
                                    R = 0.0f;
                                    G = 0.6f;
                                    B = 0.298f;
                                    // dark light green
                                }
                                else
                                {
                                    FunctionParameterIndexId = 74;
                                    R = 0.0f;
                                    G = 1.0f;
                                    B = 0.0f;
                                    // Green
                                }

                                //GlowDistance = 3938.0f;  // 75 meters divided by 0.01905f
                            }
                            
                            highlightClass.SetHighlightActiveState(process, entity, ContextID, FunctionParameterIndexId);
                            highlightClass.SetHighlightFunctions(process, entity, FunctionParameterIndexId, 137, 138, 1.5f, true, 0, false);    // 101, 125
                            
                            // set color    { 1.0f, 0.0f, 0.0f }
                            Color color = { R, G, B};
                            highlightClass.SetHighlightParameter(process, entity, FunctionParameterIndexId, &color.r);

                            highlightClass.SetHighlightDistance(process, entity, ConfigValues::GLOW_DISTANCE);     // GlowDistance

                            continue;
                        }
                    } 
                    else if (Visible) 
                    {
                        loopsSinceLastVisible[i] = 0; //only if truely visible

                        /*if ((fov < target_fov)  && (fov <= FOV_ADS || fov <= FOV_HipFire)  && (entNewVisTime > lastvis_aim[i]) )*/
                        
                        target_fov = fov;
                        target_entity = entity;
                        lastvis_aim[i] = entNewVisTime;
                        
                        if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0) 
                        {
                            FunctionParameterIndexId = 75;
                            R = ConfigValues::VISIBLE_RED_VALUE;
                            G = ConfigValues::VISIBLE_GREEN_VALUE;
                            B = ConfigValues::VISIBLE_BLUE_VALUE;
                        }
                        else if (ConfigValues::GLOW_HEALTH == 1)
                        {
                            if (shield > 100)
                            {
                                FunctionParameterIndexId = 76;
                                R = 1.0f;
                                G = 0.0f;
                                B = 0.0f;
                                // Red
                            }
                            else if (shield <= 100 && shield > 75 )
                            {
                                FunctionParameterIndexId = 77;
                                R = 1.0f;
                                G = 0.0f;
                                B = 1.0f;
                                // Purple
                            }
                            else if (shield <= 75 && shield > 50)
                            {
                                FunctionParameterIndexId = 78;
                                R = 0.0f;
                                G = 0.5f;
                                B = 1.0f;
                                // Blue
                            }
                            else if (shield <= 50 && shield > 0)
                            {
                                FunctionParameterIndexId = 79;
                                R = 0.0f;
                                G = 1.0f;
                                B = 0.5f;
                                // light green
                            }
                            else
                            {
                                FunctionParameterIndexId = 80;
                                R = 0.0f;
                                G = 1.0f;
                                B = 0.0f;
                                // Green
                            }
                        }
                        
                        highlightClass.SetHighlightActiveState(process, entity, ContextID, FunctionParameterIndexId);
                        highlightClass.SetHighlightFunctions(process, entity, FunctionParameterIndexId, 137, 125, 1.5f, true, 0, false);
                        
                        // set color
                        Color color = { R, G, B };
                        highlightClass.SetHighlightParameter(process, entity, FunctionParameterIndexId, &color.r);

                        highlightClass.SetHighlightDistance(process, entity, ConfigValues::GLOW_DISTANCE);  
                    }

                    aimbot.SetTargetEntity(target_entity);
                }
                else
                {
                    // Knocked entity
                    FunctionParameterIndexId = 81;
                    highlightClass.SetHighlightActiveState(process, entity, ContextID, FunctionParameterIndexId);
                    highlightClass.SetHighlightFunctions(process, entity, FunctionParameterIndexId, 137, 125, 1.5f, true, 0, false);
                    
                    // set color
                    Color color = { 
                        ConfigValues::KNOCKED_RED_VALUE, 
                        ConfigValues::KNOCKED_GREEN_VALUE, 
                        ConfigValues::KNOCKED_BLUE_VALUE
                    };

                    highlightClass.SetHighlightParameter(process, entity, FunctionParameterIndexId, &color.r);

                    highlightClass.SetHighlightDistance(process, entity, ConfigValues::GLOW_DISTANCE);
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

    // Pass the valid target_entity to the aimbot
    }
}

/* 
bool Glow::shouldTrainingGlowEnable(rx_handle process) {
    if ( ConfigValues::GLOW_ENABLED == 1 && levelClass.isTrainingArea(process) )
		 {
        return true;
    }
    return false;
}

//
void Glow::ActivateTrainingGlow(rx_handle process) 
{
    if (shouldTrainingGlowEnable(process))
    {  
        QWORD players = remotePlayersClass.getPlayers(process);

        for (std::size_t i = 0; i < players.size(); i++) 
        {
            Player& player = players[i];
            
            // is player valid
            if (!playerClass.isValid(process)) {
                continue;
            }

            //if(( (playerClass.isDummy(process, player)) || (playerClass.isNPC(process, player)) || (playerClass.getPlayerTeamNum(process, player) == 97) ) )           
            int shield = playerClass.getShield(process, player);

            //Initialize R, G, and B with default values
            float R, G, B;
            uint32_t FunctionParameterIndexId;

            // Glow Settings
            uint32_t ContextID = 1;
            highlightClass.SetHighlightCurrentContext(process, player, ContextID);      // Glow set context ID aka Enable
            highlightClass.SetHighlightVisibilityType(process, player, 2);              // Glow Vis Type aka Through Walls

            // Current player Last Visible check
            bool Visible = IsEntityVisible(process, player, entityVisibilityData[j]);

            if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0)
            {
                if (!Visible) 
                {
                    // if not visible, if they have been visible 20 loops ago
                    if (loopsSinceLastVisible[j] < 10) {
                        loopsSinceLastVisible[j]++; 
                    } 
                    else 
                    {
                        FunctionParameterIndexId = 65;
                        highlightClass.SetHighlightActiveState(process, player, ContextID, FunctionParameterIndexId);
                        highlightClass.SetHighlightFunctions(process, player, FunctionParameterIndexId, 137, 138, 1.5f, true, 0, false);    // 101, 125
                        
                        // set color    { 1.0f, 0.0f, 0.0f }
                        Color color = { 
                            ConfigValues::INVISIBLE_RED_VALUE, 
                            ConfigValues::INVISIBLE_GREEN_VALUE, 
                            ConfigValues::INVISIBLE_BLUE_VALUE
                        };
                        highlightClass.SetHighlightParameter(process, player, FunctionParameterIndexId, &color.r);

                        highlightClass.SetHighlightDistance(process, player, ConfigValues::GLOW_DISTANCE);     // GlowDistance

                        continue;
                    }
                } 
                else if (Visible) 
                {
                    loopsSinceLastVisible[j] = 0; //only if truely visible

                    aimbot.SetTargetEntity(player);

                    
                    FunctionParameterIndexId = 66;
                    highlightClass.SetHighlightActiveState(process, player, ContextID, FunctionParameterIndexId);
                    highlightClass.SetHighlightFunctions(process, player, FunctionParameterIndexId, 137, 125, 1.5f, true, 0, false);    // 101, 125
                    
                    // set color    { 1.0f, 0.0f, 0.0f }
                    Color color = { 
                        ConfigValues::VISIBLE_RED_VALUE, 
                        ConfigValues::VISIBLE_GREEN_VALUE, 
                        ConfigValues::VISIBLE_BLUE_VALUE
                    };
                    highlightClass.SetHighlightParameter(process, player, FunctionParameterIndexId, &color.r);

                    highlightClass.SetHighlightDistance(process, player, ConfigValues::GLOW_DISTANCE);     // GlowDistance
                }
            }
            else if (ConfigValues::GLOW_HEALTH == 1)
            {
                if (shield > 100)
                {
                    FunctionParameterIndexId = 67;
                    R = 1.0f;
                    G = 0.0f;
                    B = 0.0f;
                    // Red
                }
                else if (shield <= 100 && shield > 75 )
                {
                    FunctionParameterIndexId = 68;
                    R = 1.0f;
                    G = 0.0f;
                    B = 1.0f;
                    // Purple
                }
                else if (shield <= 75 && shield > 50)
                {
                    FunctionParameterIndexId = 69;
                    R = 0.0f;
                    G = 0.5f;
                    B = 1.0f;
                    // Blue
                }
                else if (shield <= 50 && shield > 0)
                {
                    FunctionParameterIndexId = 70;
                    R = 0.0f;
                    G = 1.0f;
                    B = 0.5f;
                    // light green
                }
                else
                {
                    FunctionParameterIndexId = 71;
                    R = 0.0f;
                    G = 1.0f;
                    B = 0.0f;
                    // Green
                }

                highlightClass.SetHighlightActiveState(process, player, ContextID, FunctionParameterIndexId);
                highlightClass.SetHighlightFunctions(process, player, FunctionParameterIndexId, 137, 125, 1.5f, true, 0, false);
                
                // set color
                Color color = { R, G, B };
                highlightClass.SetHighlightParameter(process, player, FunctionParameterIndexId, &color.r);

                highlightClass.SetHighlightDistance(process, player, ConfigValues::GLOW_DISTANCE);
            }
        }
    }
}
 */

////////////////////////////////////////////////////////////////////////////////////////

// health ESP with Vis check

/* 
if (!Visible) 
{
    // if not visible, if they have been visible 20 loops ago
    if (loopsSinceLastVisible[i] < 10) {
        loopsSinceLastVisible[i]++; 
    } 
    else 
    {   
        if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0)
        {
            R = ConfigValues::INVISIBLE_RED_VALUE;
            G = ConfigValues::INVISIBLE_GREEN_VALUE;
            B = ConfigValues::INVISIBLE_BLUE_VALUE;

            //GlowDistance = ConfigValues::GLOW_DISTANCE;
        }
        else if (ConfigValues::GLOW_HEALTH == 1)
        {
            if (shield > 100)
            {
                FunctionParameterIndexId = 67;
                R = 0.725f;
                G = 0.0f;
                B = 0.0f;
                // Dark Red
            }
            else if (shield <= 100 && shield > 75 )
            {
                FunctionParameterIndexId = 68;
                R = 0.5f;
                G = 0.0f;
                B = 0.5f;
                // dark Purple
            }
            else if (shield <= 75 && shield > 50)
            {
                FunctionParameterIndexId = 69;
                R = 0.2f;
                G = 0.2f;
                B = 1.0f;
                // dark Blue
            }
            else if (shield <= 50 && shield > 0)
            {
                FunctionParameterIndexId = 70;
                R = 0.0f;
                G = 0.6f;
                B = 0.298f;
                // dark light green
            }
            else
            {
                FunctionParameterIndexId = 71;
                R = 0.0f;
                G = 1.0f;
                B = 0.0f;
                // Green
            }

            //GlowDistance = 3938.0f;  // 75 meters divided by 0.01905f
        }
        
        FunctionParameterIndexId = 65;
        highlightClass.SetHighlightActiveState(process, entity, ContextID, FunctionParameterIndexId);
        highlightClass.SetHighlightFunctions(process, entity, FunctionParameterIndexId, 137, 138, 1.5f, true, 0, false);    // 101, 125
        
        // set color    { 1.0f, 0.0f, 0.0f }
        Color color = { R, G, B};
        highlightClass.SetHighlightParameter(process, entity, FunctionParameterIndexId, &color.r);

        highlightClass.SetHighlightDistance(process, entity, ConfigValues::GLOW_DISTANCE);     // GlowDistance

        continue;
    }
} 
else if (Visible) 
{
    loopsSinceLastVisible[i] = 0; //only if truely visible

    if (fov < target_fov && entNewVisTime > lastvis_aim[i])
    {
        target_fov = fov;
        target_entity = entity;
        lastvis_aim[i] = entNewVisTime;
        // Pass the valid target_entity to the aimbot
        aimbot.SetTargetEntity(entity);
    }
    
    if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0) 
    {
        FunctionParameterIndexId = 66;
        R = ConfigValues::VISIBLE_RED_VALUE;
        G = ConfigValues::VISIBLE_GREEN_VALUE;
        B = ConfigValues::VISIBLE_BLUE_VALUE;
    }
    else if (ConfigValues::GLOW_HEALTH == 1)
    {
        if (shield > 100)
        {
            FunctionParameterIndexId = 67;
            R = 1.0f;
            G = 0.0f;
            B = 0.0f;
            // Red
        }
        else if (shield <= 100 && shield > 75 )
        {
            FunctionParameterIndexId = 68;
            R = 1.0f;
            G = 0.0f;
            B = 1.0f;
            // Purple
        }
        else if (shield <= 75 && shield > 50)
        {
            FunctionParameterIndexId = 69;
            R = 0.0f;
            G = 0.5f;
            B = 1.0f;
            // Blue
        }
        else if (shield <= 50 && shield > 0)
        {
            FunctionParameterIndexId = 70;
            R = 0.0f;
            G = 1.0f;
            B = 0.5f;
            // light green
        }
        else
        {
            FunctionParameterIndexId = 71;
            R = 0.0f;
            G = 1.0f;
            B = 0.0f;
            // Green
        }
    }
    
    highlightClass.SetHighlightActiveState(process, entity, ContextID, FunctionParameterIndexId);
    highlightClass.SetHighlightFunctions(process, entity, FunctionParameterIndexId, 101, 125, 1.5f, true, 0, false);
    
    // set color
    Color color = { R, G, B };
    highlightClass.SetHighlightParameter(process, entity, FunctionParameterIndexId, &color.r);

    highlightClass.SetHighlightDistance(process, entity, ConfigValues::GLOW_DISTANCE);  
} 
*/
/////////////////////////////////////////////////////////////////////////////////////////

// // OLD GLOW
// void Glow::ActivateGlow(rx_handle process, QWORD ClientEntityList, QWORD Sensitivity, int GameMode ) 
// {
//     if (shouldGlowEnable(process))
//     {   
//         QWORD localplayer = localplayerClass.getLocalplayerPointer(process);
        
//         if (localplayer == 0)
//         {
//             memset(lastvis_aim, 0, sizeof(lastvis_aim));
//             return;
//         }

//         // local Player Team number
//         int LocTeam = localplayerClass.getLocalTeamNumber(process);

//         // Weapon in hand
//         DWORD weapon_id = localplayerClass.getWeaponHandle(process);
//         QWORD weapon = playerClass.GetClientEntity(process, ClientEntityList, weapon_id - 1);

//         float bulletSpeed = weaponClass.getBulletSpeed(process, weapon);
//         float bulletGravity = weaponClass.getBulletGravity(process, weapon);

//         vec3 muzzle;
//         rx_read_process(process, localplayer + OFFSETS::m_muzzle, &muzzle, sizeof(vec3));

//         QWORD target_entity = 0;
//         float target_fov = 360.0f;

//         // Entity loop
//         for (int i = 0; i < 70; i++)
//         {
//             QWORD entity = playerClass.GetClientEntity(process, ClientEntityList, i);
            
//             // is Entity valid
//             if (entity == 0) {
//                 continue;
//             }

//             QWORD EntityHandleAddress = playerClass.getplayerName(process, entity);
            
//             // Read the string from EntityHandleAddress
//             std::string Identifier = rx_read_string(process, EntityHandleAddress);
//             const char* IdentifierC = Identifier.c_str();
            
//             // Compare the C-style string to "player"
//             if (strcmp(IdentifierC, "player") != 0) 
//             {
//                 //Entity team number
//                 int EntTeam = playerClass.getPlayerTeamNum(process, entity);
//                 if (EntTeam % 2)
//                 {
//                     iTeamControl = 1;
//                 }
//                 else
//                 {
//                     iTeamControl = 2;
//                 }

//                 //LocalPlayer Team Number
//                 if (LocTeam % 2)
//                 {
//                     iLocControl = 1;
//                 }
//                 else
//                 {
//                     iLocControl = 2;
//                 }

//                 //GameMode check
//                 if (GameMode == 1953394531)
//                 {
//                     if (iTeamControl == iLocControl)
//                         continue;
//                 }

//                 // If entity is local player then skip this iteration
//                 if (entity == localplayer)
//                     continue;

//                 /* 
//                 if (rx_read_i32(process, entity + m_iName) != 125780153691248)
//                 {
//                     continue;
//                 }
//                 */

//                 // Squard/ Team check: if entity number is equal to Local number then skip this iteration
//                 if (EntTeam == LocTeam)
//                 {
//                     continue;
//                 }

//                 // Entity Health
//                 if ((playerClass.getHealth(process, entity)) == 0)
//                 {
//                     lastvis_aim[i] = 0;
//                     continue;
//                 }

//                 // Get entity knocked state
//                 int entKnockedState = playerClass.getBleedState(process, entity);
                
//                 if ((playerClass.getLifeState(process, entity)) != 0)
//                 {
//                     lastvis_aim[i] = 0;
//                     continue;
//                 }
                
//                 // Logic to change entity color when in FOV or AimLock
//                 vec3 hitbox = playerClass.GetBonePosition(process, entity, 2);

//                 vec3 velocity;
//                 rx_read_process(process, entity + OFFSETS::m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

//                 float fl_time = vec_distance(hitbox, muzzle) / bulletSpeed;
//                 hitbox.z += (700.0f * bulletGravity * 0.5f) * (fl_time * fl_time);

//                 velocity.x = velocity.x * fl_time;
//                 velocity.y = velocity.y * fl_time;
//                 velocity.z = velocity.z * fl_time;

//                 hitbox.x += velocity.x;
//                 hitbox.y += velocity.y -40;
//                 hitbox.z += velocity.z;

//                 vec3 target_angle = CalcAngle(muzzle, hitbox);
                
//                 vec3 breath_angles;
//                 rx_read_process(process, localplayer + OFFSETS::m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

//                 // Last Visible time
//                 float entNewVisTime = playerClass.getLastVisibleTime(process, entity);

//                 // Get the shield value to change the color of the glow
//                 int shield = playerClass.getShield(process, entity);

//                 //Initialize R, G, and B with default values
//                 int R, G, B;

//                 // Glow enable
//                 //rx_write_i32(process, entity + 0x294, 1);   // OFFSET_GLOW_ENABLE  0x3F8  //7 = enabled, 2 = disabled
//                 rx_write_i32(process, entity + 0x278, 2);   // OFFSET_GLOW_THROUGH_WALLS  0x400 //2 = enabled, 5 = disabled
                    
//                 // Glow Mode
//                 //rx_write_i32(process, entity + 0x29c, 1512990053);
//                 // GlowMode glowModeData = { 101, 101, 46, 90 };
//                 // rx_write_array(process, entity + 0x29c, (char*)&glowModeData, sizeof(GlowMode));    // GLOW_TYPE  0x2c4 + 0x30 (0x2F4)

//                 // Current entity Last Visible check
//                 bool Visible = IsEntityVisible(process, entity, entityVisibilityData[i]);

//                 // Local Player Field of View for aimbot
//                 float fov = get_fov(breath_angles, target_angle);

//                 // Entity not knocked
//                 if (entKnockedState == 0)
//                 {
//                     if (ConfigValues::GLOW_ENEMY == 1 && ConfigValues::GLOW_HEALTH == 0)
//                     {
//                         if (!Visible) 
//                         { 
//                             // if not visible, if they have been visible 20 loops ago
//                             if (loopsSinceLastVisible[i] < 10) {
//                                 loopsSinceLastVisible[i]++; 
//                             } 
//                             else 
//                             {
//                                 rx_write_float(process, entity + 0x26c, ConfigValues::GLOW_DISTANCE);   // 0x3E4

//                                 // rx_write_float(process, entity + 0x200, ConfigValues::INVISIBLE_RED_VALUE);   // Red    GLOW_COLOR_R  = 0x200
//                                 // rx_write_float(process, entity + 0x204, ConfigValues::INVISIBLE_GREEN_VALUE); // Green  GLOW_COLOR_G  = GLOW_COLOR_R + 0x04
//                                 // rx_write_float(process, entity + 0x208, ConfigValues::INVISIBLE_BLUE_VALUE);  // Blue   GLOW_COLOR_B  = GLOW_COLOR_G + 0x04
                                
//                                 //rx_write_float(process, entity + 0x298, 10);

//                                 rx_write_i32(process, entity + 0x294, 0);
//                                 rx_write_i32(process, entity + 0x298, 28);

//                                 continue;
//                             }
//                         } 
//                         else if (Visible) 
//                         {
//                             loopsSinceLastVisible[i] = 0; //only if truely visible

//                             target_fov = fov;
//                             target_entity = entity;
//                             lastvis_aim[i] = entNewVisTime;

//                             rx_write_float(process, entity + 0x26c, ConfigValues::GLOW_DISTANCE);

//                             // rx_write_float(process, entity + 0x200, ConfigValues::VISIBLE_RED_VALUE);   // Red
//                             // rx_write_float(process, entity + 0x204, ConfigValues::VISIBLE_GREEN_VALUE); // Green
//                             // rx_write_float(process, entity + 0x208, ConfigValues::VISIBLE_BLUE_VALUE);  // Blue

//                             rx_write_i32(process, entity + 0x294, 0);
//                             rx_write_i32(process, entity + 0x298, 12);

//                             // Pass the valid target_entity to the aimbot
//                             aimbot.SetTargetEntity(entity);
//                         }
//                     }
//                     // Health Based glow ESP, No Visible Check
//                     else if (ConfigValues::GLOW_HEALTH == 1)
//                     {
//                         // Calculate R, G, B based on shield values
//                         if (shield > 100)
//                         {
//                             R = 3;
//                             G = 0;
//                             B = 0;
//                         }
//                         else if (shield < 100 && shield > 75 )
//                         {
//                             R = 1;
//                             G = 0;
//                             B = 2;
//                         }
//                         else if (shield < 76 && shield > 50)
//                         {
//                             R = 0;
//                             G = 1;
//                             B = 2;
//                         }
//                         else if (shield < 51 && shield > 0)
//                         {
//                             R = 0;
//                             G = 3;
//                             B = 0;
//                         }
//                         else
//                         {
//                             R = 0;
//                             G = 2;
//                             B = 1;
//                         }

//                         rx_write_float(process, entity + 0x26c, ConfigValues::GLOW_DISTANCE);
                            
//                         rx_write_float(process, entity + 0x200, R); // Red
//                         rx_write_float(process, entity + 0x204, G); // Green
//                         rx_write_float(process, entity + 0x208, B); // Blue
//                     }
//                 }
//                 else
//                 {
//                     rx_write_float(process, entity + 0x26c, ConfigValues::GLOW_DISTANCE);

//                     // If Entity/ Player is in Knockdown State
//                     rx_write_float(process, entity + 0x200, 10.0f);   // Red
//                     rx_write_float(process, entity + 0x204, 10.0f); // Green
//                     rx_write_float(process, entity + 0x208, 10.0f);  // Blue
//                 }
//                 /*
//                 }
//                 else if (GLOW_SQUARD == 1)
//                 {	
//                     // Apply Color to Team/ Squard
//                     rx_write_float(process, entity + 0x200, ConfigValues::squard_redValue);   // Red
//                     rx_write_float(process, entity + 0x204, ConfigValues::squard_greenValue); // Green
//                     rx_write_float(process, entity + 0x208, ConfigValues::squard_blueValue);  // Blue
//                 }
//                 */
//             }
//         }
//     }
// }



// old............
// #define OFFSET_GLOW_T1  0x292 //16256 = enabled, 0 = disabled => NEED TO CHECK/CHANGE
// #define OFFSET_GLOW_T2  0x30c //1193322764 = enabled, 0 = disabled => NEED TO CHECK/CHANGE
// #define OFFSET_GLOW_ENABLE  0x3F8 //0x3c8 //7 = enabled, 2 = disabled => NEED TO CHECK/CHANGE
// #define OFFSET_GLOW_THROUGH_WALLS  0x400 //2 = enabled, 5 = disabled => NEED TO CHECK/CHANGE
// #define GLOW_TYPE  0x2c4 + 0x30 // => NEED TO CHECK/CHANGE
// #define GLOW_COLOR_R  0x200 // => NEED TO CHECK/CHANGE
// #define GLOW_COLOR_G  GLOW_COLOR_R + 0x04 // => NEED TO CHECK/CHANGE
// #define GLOW_COLOR_B  GLOW_COLOR_G + 0x04 // => NEED TO CHECK/CHANGE
