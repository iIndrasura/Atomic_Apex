// update: 2023-09-19   https://pastebin.com/hzxwmZmH
// update: 2023-08-30   https://pastebin.com/FBKH8mLZ

#pragma once

namespace OFFSETS
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Core
    const long REGION = 0x140000000;                        // [Mine]          -> Region
    const long LEVEL = 0x16f71e0;                           // [Miscellaneous] -> LevelName
    const long m_localplayer = 0x21D3758;             // [Miscellaneous] -> AVC_GameMovement+0x8 /  player_overheat_time_to_overheat + 0xA8
    const long ENTITY_LIST = 0x1e23418;                 // [Miscellaneous] -> cl_entitylist
    const long GAMEMODE = 0x0220c5c0;                       // [ConVars] mp_gamemode       -> mp_gamemode+0x58 not sur eif you ened to add 0x58

    const long INPUT_SYSTEM = 0x1774cc0;
    const long GLOBALS = 0x16f6d20;                         //GlobalVars
    const long LOCAL_ORIGIN = 0x0188;                     // [DataMap.CBaseViewModel]    -> 	m_localOrigin
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
    // Weapon
    const long iWEAPON = 0x1954;                            // [RecvTable.DT_BaseCombatCharacter]     -> m_latestPrimaryWeapons
    const long AMMO_IN_CLIP = 0x15a0;                       // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_ammoInClip
    const long AMMO_STOCKPILE = 0x15a4;             // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_ammoInStockpile
    const long SEMI_AUTO = 0x19ad + 0x3 + 0x018c;                     // [Miscellaneous] -> CWeaponX!m_isSemi [RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3 + 0x018c
    const long BURST = 0x1708;                            // [DataMap.CWeaponX]                     -> m_burstFireCount
    const long m_iAimPunch = 0x23f8;                        // [DataMap.C_Player] -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const long m_muzzle = 0x1e90;                           // [Miscellaneous]              -> CPlayer!camera_origin
    const long m_sensitivity = 0x021bf6c0;                  //mouse_sensitivity
    const long m_bulletSpeed = 0x1E9C;                      //CWeaponX!m_flProjectileSpeed      or        [WeaponSettingsMeta] base + [WeaponSettings] projectile_launch_speed 
    const long m_bulletGravity = m_bulletSpeed + 0x8;       //CWeaponX!m_flProjectileSpeed + 0x8        or      [WeaponSettingsMeta] base + [WeaponSettings] projectile_gravity_scale       
    const long bZOOMING = 0x1b91;                           // [RecvTable.DT_Player]            -> m_bZooming
    const long READY_TIME = 0x1578;                    // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_nextReadyTime
    const long ZOOM_FOV = 0x15f0 + 0x00bc;                // [DataMap.WeaponPlayerData]             -> m_playerData + m_targetZoomFOV

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Player
    const long TIME = 0x2048;                       // [DataMap.C_Player]           -> m_currentFramePlayer.timeBase
    const long LAST_VISIBLE_TIME = 0x19ad + 0x3;                  // [Miscellaneous] -> CPlayer!lastVisibleTime   or [WeaponSettingsMeta] base or [RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3
    const long m_iHealth = 0x036c;                          //RecvTable.DT_Player.      m_iHealth
    const long m_shieldHealth = 0x01a0;                     // [RecvTable.DT_BaseEntity]        -> m_shieldHealth
    const long SKYDIVE_STATE = 0x4620;              // [RecvTable.DT_Player]            -> m_skydiveState
    const long LIFE_STATE = 0x06c8;                         // [RecvTable.DT_Player]            -> m_lifeState
    const long TEAM_NUMBER = 0x037c;                        // [RecvTable.DT_BaseEntity]        -> m_iTeamNum
    const long m_iViewAngles = 0x24f4 - 0x14;               //[DataMap.C_Player] -> m_ammoPoolCapacity - 0x14
    const long CAMERA_ANGLES = 0x1ba0 + 0x2EC;              //m_zoomFullStartTime + 0x2EC
    const long m_bleedoutState = 0x26a0;                    // [RecvTable.DT_Player]            -> m_bleedoutState
    const long PLAYER_DATA = 0x15f0;                        // [RecvTable.DT_WeaponX]         -> m_playerData
    //const long m_targetZoomFOV=0x00bc;                    // [RecvTable.DT_WeaponPlayerData]   -> m_targetZoomFOV

    const long BONE_MATRIX = 0x0dd0 + 0x50 - 0x8;           //m_nForceBone + 0x50 - 0x8
    const long BONES = 0x0dd0 + 0x48;                     // [RecvTable.DT_BaseAnimating]     -> m_nForceBone + 0x48

    const long FLAGS = 0x00c8;                            // [RecvTable.DT_Player]            -> m_fFlags
    const long START_CROSSHAIR_TIME = LAST_VISIBLE_TIME - 0x4;               // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime - 0x4
    const long LAST_CROSSHAIR_TIME = LAST_VISIBLE_TIME + 0x08;                // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime           (OFFSET_VISIBLE_TIME + 0x08) // CPlayer!lastCrosshairTargetTime
    //const long SKYDIVE_STATE = 0x4620;                    // [RecvTable.DT_Player]            -> m_skydiveState
    const long NAME = 0x04b9;                             // [RecvTable.DT_BaseEntity]        -> m_iName

    const long m_hViewModels = 0x2ce0;                      //m_hViewModels
    const long arm_ViewModels = m_hViewModels + 0xC;        // m_hViewModels + 0xc

    const long spec_YAW = 0x21fc - 0x8;                     //m_currentFramePlayer.m_ammoPoolCount - 0x8
    const long SIGNIFIER_NAME = 0x04b0;             // [RecvTable.DT_BaseEntity]        -> m_iSignifierName

    //LocalPlayer
    const long nSKIN = 0x0d84;                        //int value  m_nSkin
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Highlights
    //const long GLOW_ENABLE = 0x03f0 + 0x8;                // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    //const long GLOW_THROUGH_WALL = 0x03f0 + 0x10;         // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    //const long GLOW_COLOR = 0x01e8;                       // [RecvTable.DT_HighlightSettings] -> m_highlightParams + 0x18

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // [Buttons]
    const long in_duck = 0x07422a48;                        //[Buttons] -> in_duck
    const long in_Attack = 0x07422868;                      //[Buttons] -> in_attack+0x8
    const long in_forward = 0x07422798;                     //[Buttons] -> in_forward
    const long in_jump = 0x07422950;                        //[Buttons] -> in_jump
    //const long IN_BACKWARD = 0x074227c0;                  // if do not work remove + 0x8 testing
    //const long IN_WALK = 0x074229f8;                      // [Buttons] -> in_walk
    const long IN_RELOAD = 0x074228b8;  // [Buttons] -> in_reload

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //
    const long m_vecAbsOrigin = 0x017c;                     //DataMap.CBaseViewModel.   m_vecAbsOrigin
    const long m_itemId = 0x1578;                           //RecvTable.DT_PropSurvival.m_customScriptInt
    const long m_iObserverMode = 0x3454;                    //m_iObserverMode
    //const long host_timescale = 0x017b9080;                 // ConVars_host_timescale for FakeDuck

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Grapple   
    const long m_grapple = 0x2c08;                          //[RecvTable.DT_Player] m_grapple     
    const long m_grappleAttached = 0x0048;                  //[DataMap.GrappleData] m_grappleAttached
    const long m_grappleActivateTime=0x0054;

    //Server time
    //const long server_time = 0x1415FA4D8;               // 48 8B 05 ?? ?? ?? ?? F3 0F 58 70 10

        ////Glow
    // #define OFFSET_GLOW_DISTANCE  0x3B4 + 0x30 //0x3E4
    // #define OFFSET_GLOW_COLOR           0x200
    // #define OFFSET_GLOW_TYPE                 0x2C4 + 0x30
    // #define OFFSET_GLOW_CONTEXT          0x03f0 + 0x8 //7 = enabled, 2 = disabled
    // #define OFFSET_GLOW_THROUGH_WALLS   0x400 //2 = enabled, 5 = disabled
			
}
