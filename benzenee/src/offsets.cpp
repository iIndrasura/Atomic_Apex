// update: 2023-08-30   https://pastebin.com/FBKH8mLZ
// update: 2023-08-21

#pragma once

namespace OFFSETS
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Core
    const long REGION = 0x140000000;                        // [Mine]          -> Region
    const long LEVEL = 0x16efe10;                           // [Miscellaneous] -> LevelName
    const long m_localplayer = 0x2225640 + 0x8;             // [Miscellaneous] -> AVC_GameMovement+0x8
    const long m_cl_entitylist = 0x1e754c8;                 // [Miscellaneous] -> cl_entitylist
    const long GAMEMODE = 0x0225e4d0;                       // [ConVars] mp_gamemode       -> mp_gamemode+0x58 not sur eif you ened to add 0x58

    const long GLOBALS = 0x16ef950;                         //GlobalVars
    const long LOCAL_ORIGIN = 0x0188;                     // [DataMap.CBaseViewModel]    -> 	m_localOrigin
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
    // Weapon
    const long iWEAPON = 0x1a44;                            // [RecvTable.DT_BaseCombatCharacter]     -> m_latestPrimaryWeapons
    const long m_ammoInClip = 0x1664;                       // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_ammoInClip
    //const long m_isSemiAuto = 0x1c2c;                     // [Miscellaneous]                        -> CWeaponX!m_isSemiAuto STILL NEED 
    //const long BURST = 0x17f8;                            // [DataMap.CWeaponX]                     -> m_burstFireCount
    const long m_iAimPunch = 0x24e8;                        // [DataMap.C_Player] -> m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
    const long m_muzzle = 0x1f80;                           // [Miscellaneous]              -> CPlayer!camera_origin
    const long m_sensitivity = 0x02211770;                  //mouse_sensitivity
    const long m_bulletSpeed = 0x1F6C;                      //CWeaponX!m_flProjectileSpeed
    const long m_bulletGravity = m_bulletSpeed + 0x8;       //CWeaponX!m_flProjectileSpeed + 0x8
    const long bZOOMING = 0x1c81;                           // [RecvTable.DT_Player]            -> m_bZooming
    const long m_nextReadyTime = 0x1668;                    // [RecvTable.DT_WeaponX_LocalWeaponData] -> m_nextReadyTime
    //const long ZOOM_FOV = 0x16e0 + 0x00bc;                // [DataMap.WeaponPlayerData]             -> m_playerData + m_targetZoomFOV

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Player
    const long TIME = 0x2138;                       // [DataMap.C_Player]           -> m_currentFramePlayer.timeBase
    const long LAST_VISIBLE_TIME = 0x1AA0;                  // [Miscellaneous]                  -> CPlayer!lastVisibleTime 
    const long m_iHealth = 0x0470;                          //RecvTable.DT_Player.m_iHealth
    const long m_shieldHealth = 0x01a0;                     // [RecvTable.DT_BaseEntity]        -> m_shieldHealth
    const long SKYDIVE_STATE = 0x46f0;              // [RecvTable.DT_Player]            -> m_skydiveState
    const long LIFE_STATE = 0x07d0;                         // [RecvTable.DT_Player]            -> m_lifeState
    const long TEAM_NUMBER = 0x0480;                        // [RecvTable.DT_BaseEntity]        -> m_iTeamNum
    const long m_iViewAngles = 0x25e4 - 0x14;               //[DataMap.C_Player] -> m_ammoPoolCapacity - 0x14
    const long CAMERA_ANGLES = 0x1c90 + 0x2EC;              //m_zoomFullStartTime + 0x2EC
    const long m_bleedoutState = 0x2790;                    // [RecvTable.DT_Player]            -> m_bleedoutState
    const long PLAYER_DATA = 0x16e0;                        // [RecvTable.DT_WeaponX]         -> m_playerData
    //const long m_targetZoomFOV=0x00bc;                    // [RecvTable.DT_WeaponPlayerData]   -> m_targetZoomFOV

    const long BONE_MATRIX = 0x0ec8 + 0x50 - 0x8;           //m_nForceBone + 0x50 - 0x8
    const long BONES = 0x0ec8 + 0x48;                     // [RecvTable.DT_BaseAnimating]     -> m_nForceBone + 0x48

    const long FLAGS = 0x00c8;                            // [RecvTable.DT_Player]            -> m_fFlags
    const long START_CROSSHAIR_TIME = 0x1AA4;               // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime - 0x4
    const long LAST_CROSSHAIR_TIME = 0x1AA8;                // [Miscellaneous]                  -> CPlayer!lastCrosshairTargetTime
    //const long SKYDIVE_STATE = 0x46f0;                    // [RecvTable.DT_Player]            -> m_skydiveState
    const long NAME = 0x05c1;                             // [RecvTable.DT_BaseEntity]        -> m_iName
    //const long SKYDIVE_STATE = 0x46f0;                    // [RecvTable.DT_Player]            -> m_skydiveState

    const long m_hViewModels = 0x2dc0;                      //m_hViewModels
    const long arm_ViewModels = m_hViewModels + 0xC;        // m_hViewModels + 0xc

    const long spec_YAW = 0x22ec - 0x8;                     //m_currentFramePlayer.m_ammoPoolCount - 0x8
    const long SIGNIFIER_NAME = 0x05b8;             // [RecvTable.DT_BaseEntity]        -> m_iSignifierName

    //LocalPlayer
    const long nSKIN = 0x0e84;                        //int value  m_nSkin
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Highlights
    //const long GLOW_ENABLE = 0x03f0 + 0x8;                // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x8
    //const long GLOW_THROUGH_WALL = 0x03f0 + 0x10;         // [RecvTable.DT_HighlightSettings] -> m_highlightServerContextID + 0x10
    //const long GLOW_COLOR = 0x01e8;                       // [RecvTable.DT_HighlightSettings] -> m_highlightParams + 0x18

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // [Buttons]
    const long in_duck = 0x07474188;                        //[Buttons] -> in_duck
    const long in_Attack = 0x07473fb8;                      //[Buttons] -> in_attack+0x8
    const long in_forward = 0x074741d8;                     //[Buttons] -> in_forward
    const long in_jump = 0x074740a8;                        //[Buttons] -> in_jump
    //const long IN_BACKWARD = 0x07473100;                  // if do not work remove + 0x8 testing
    //const long IN_WALK = 0x07473068;                      // [Buttons] -> in_walk
    //const long IN_RELOAD = 0x07472f18;                    // [Buttons] -> in_reload
    const long IN_RELOAD = 0x07472f18;  // [Buttons] -> in_reload

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //
    const long m_vecAbsOrigin = 0x017c;                     //DataMap.CBaseViewModel.m_vecAbsOrigin
    const long m_itemId = 0x1668;                           //RecvTable.DT_PropSurvival.m_customScriptInt
    const long m_iObserverMode = 0x3534;                    //m_iObserverMode
    const long host_timescale = 0x017b9080;                 // ConVars_host_timescale for FakeDuck

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Grapple   
    const long m_grapple = 0x2ce8;                          //[RecvTable.DT_Player] m_grapple     
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
