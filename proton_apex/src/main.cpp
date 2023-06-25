#include "../../rx/rx.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "handling.h"
#include <iostream>
#include <sstream>
#include "math.h"
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <map>


/* ------------------ START CONFIGURATIONS ------------------ */

/*
#define AIMKEY 111						// keys: 107 = mouse1, 108 = mouse2, 109 = mouse3, 110 = mouse4, 111 = mouse5, 80 = LAlt
#define AIMFOV_ADS 2.4f					// ADS fov (aiming) 								(10 = agressive, 7 = moderated, 3 = safe)
#define AIMFOV_HIPFIRE 8.4f				// Hipfire fov (not aiming)							(15 = agressive, 10 = moderated, 5 = safe)
#define AIMSMOOTH 19.4f					// Speed that the aim will lock on the enemies.	    (8 = agressive, 15 = moderated, 20 = safe)
#define ITEM_ESP 1						// Enable or disable ESP item
#define AIMBOT_ENABLED 1				// Enable or disable aimbot
std::chrono::milliseconds sleep(15); 	// aim assist sleep time in miliseconds, increasing this value will turn aimbot more `safe`
float maxdistance = 160.0f;				// aim assist maximum range in meters
*/

void readConfig(std::map<std::string, std::string>& config) {
    std::ifstream configFile("config.cfg");
    if (!configFile) {
        std::cout << "Config file not found. Creating a new one with default values." << std::endl;
        config["AIMKEY"] = "107";
        config["AIMKEY2"] = "110";
        config["AIMFOV_ADS"] = "2.4";
        config["AIMFOV_HIPFIRE"] = "8.4";
        config["AIMSMOOTH"] = "19.4";
        config["ITEM_ESP"] = "1";
        config["AIMBOT_ENABLED"] = "1";
        config["SLEEP"] = "15";
        config["MAXDISTANCE"] = "160.0";
		config["BONE_LIST"] = "2,3,5,8";

        std::ofstream newConfigFile("config.cfg");
        if (newConfigFile) {
            for (const auto& entry : config) {
                newConfigFile << entry.first << "=" << entry.second << std::endl;
            }
        } else {
            std::cout << "Error creating config file." << std::endl;
        }
    } else {
        std::string line;
        while (std::getline(configFile, line)) {
            size_t equalPos = line.find('=');
            if (equalPos != std::string::npos) {
                std::string key = line.substr(0, equalPos);
                std::string value = line.substr(equalPos + 1);
                config[key] = value;
            }
        }
    }
}

/* ------------------ END CONFIGURATIONS ------------------ */


/* ------------------ START OFFSETS ------------------ */

int m_iHealth = 0x043c;                             //RecvTable.DT_Player.m_iHealth
int m_iTeamNum = 0x044c;                         //RecvTable.DT_BaseEntity.m_iTeamNum
int m_iViewAngles = 0x25b4 - 0x14;               //m_ammoPoolCapacity - 0x14
int m_iCameraAngles = 0x1c60 + 0x2EC;            //m_zoomFullStartTime + 0x2EC
int m_bZooming = 0x1c51;                         //m_bZooming
int m_iBoneMatrix = 0x0e98 + 0x50 - 0x8;         //m_nForceBone + 0x50 - 0x8
int m_iWeapon = 0x1a14;                          //m_latestPrimaryWeapons
int m_vecAbsOrigin = 0x014c;                     //DataMap.CBaseViewModel.m_vecAbsOrigin
int m_playerData = 0x16b0;                       //RecvTable.DT_WeaponX.m_playerData
int m_lifeState = 0x0798;                        //RecvTable.DT_Player.m_lifeState
int m_itemId = 0x1638;                           //RecvTable.DT_PropSurvival.m_customScriptInt
int m_gameMode = 0x0223c7e0;                     //mp_gamemode
int m_localplayer = 0x022036C0 + 0x8;            //.?AVC_GameMovement@@ + 0x8) ???
int m_sensitivity = 0x021efa40;                  //mouse_sensitivity
int m_bulletSpeed = 0x1F9c;                      //CWeaponX!m_flProjectileSpeed        or        WeaponSettingsMeta.base + WeaponSettings.projectile_launch_speed
int m_bulletGravity = m_bulletSpeed + 0x8;       //CWeaponX!m_flProjectileSpeed + 0x8
int m_muzzle = 0x1f50;                           //CPlayer!camera_origin
int m_iObserverMode = 0x34f4;                    //m_iObserverMode

#define in_Attack 0x0743d3b0                     //[Buttons] -> in_attack
#define m_bleedoutState 0x2750
//#define OFFSET_YAW 0x22c4                      //m_currentFramePlayer.m_ammoPoolCount


/* ------------------ END OFFSETS OFFSETS ------------------ */



int itemWorkaround = 0;

int GetApexProcessId(void)
{
	int pid = 0;
	rx_handle snapshot = rx_create_snapshot(RX_SNAP_TYPE_PROCESS, 0);

	RX_PROCESS_ENTRY entry;

	while (rx_next_process(snapshot, &entry))
	{
		if (!strcmp(entry.name, "wine64-preloader"))
		{
			rx_handle snapshot_2 = rx_create_snapshot(RX_SNAP_TYPE_LIBRARY, entry.pid);

			RX_LIBRARY_ENTRY library_entry;

			while (rx_next_library(snapshot_2, &library_entry))
			{
				if (!strcmp(library_entry.name, "easyanticheat_x64.dll"))
				{
					pid = entry.pid;
					break;
				}
			}
			rx_close_handle(snapshot_2);

			//
			// process found
			//
			if (pid != 0)
			{
				break;
			}
		}
	}
	rx_close_handle(snapshot);

	return pid;
}

QWORD GetApexBaseAddress(int pid)
{
	rx_handle snapshot = rx_create_snapshot(RX_SNAP_TYPE_LIBRARY, pid);

	RX_LIBRARY_ENTRY entry;
	DWORD counter = 0;
	QWORD base = 0;

	while (rx_next_library(snapshot, &entry))
	{
		const char *sub = strstr(entry.name, "memfd:wine-mapping");

		if ((entry.end - entry.start) == 0x1000 && sub)
		{
			if (counter == 0)
				base = entry.start;
		}

		if (sub)
		{
			counter++;
		}

		else
		{
			counter = 0;
			base = 0;
		}

		if (counter >= 200)
		{
			break;
		}
	}

	rx_close_handle(snapshot);

	return base;
}

typedef struct
{
	uint8_t pad1[0xCC];
	float x;
	uint8_t pad2[0xC];
	float y;
	uint8_t pad3[0xC];
	float z;
} matrix3x4_t;



int itemID;
int mode;
int iTeamControl;
int iLocControl;

QWORD GetClientEntity(rx_handle game_process, QWORD entity, QWORD index)
{

	index = index + 1;
	index = index << 0x5;

	return rx_read_i64(game_process, (index + entity) - 0x280050);
}

QWORD get_interface_function(rx_handle game_process, QWORD ptr, DWORD index)
{
	return rx_read_i64(game_process, rx_read_i64(game_process, ptr) + index * 8);
}

vec3 GetBonePosition(rx_handle game_process, QWORD entity_address, int index)
{
	vec3 position;
	rx_read_process(game_process, entity_address + m_vecAbsOrigin, &position, sizeof(position));

	QWORD bonematrix = rx_read_i64(game_process, entity_address + m_iBoneMatrix);

	matrix3x4_t matrix;
	rx_read_process(game_process, bonematrix + (0x30 * index), &matrix, sizeof(matrix3x4_t));

	vec3 bonepos;
	bonepos.x = matrix.x + position.x;
	bonepos.y = matrix.y + position.y;
	bonepos.z = matrix.z + position.z;

	return bonepos;
}

BOOL IsButtonDown(rx_handle game_process, QWORD IInputSystem, int KeyCode)
{
	KeyCode = KeyCode + 1;
	DWORD a0 = rx_read_i32(game_process, IInputSystem + ((KeyCode >> 5) * 4) + 0xb0);
	return (a0 >> (KeyCode & 31)) & 1;
}

int dump_table(rx_handle game_process, QWORD table, const char *name)
{

	for (DWORD i = 0; i < rx_read_i32(game_process, table + 0x10); i++)
	{

		QWORD recv_prop = rx_read_i64(game_process, table + 0x8);
		if (!recv_prop)
		{
			continue;
		}

		recv_prop = rx_read_i64(game_process, recv_prop + 0x8 * i);
		char recv_prop_name[260];
		{
			QWORD name_ptr = rx_read_i64(game_process, recv_prop + 0x28);
			rx_read_process(game_process, name_ptr, recv_prop_name, 260);
		}

		if (!strcmp(recv_prop_name, name))
		{
			return rx_read_i32(game_process, recv_prop + 0x4);
		}
	}

	return 0;
}

int main(void)
{
	std::map<std::string, std::string> config;
    readConfig(config);

    // Convert the configuration values to the desired types
    #define AIMKEY std::stoi(config["AIMKEY"])
    #define AIMKEY2 std::stoi(config["AIMKEY2"])
    #define AIMFOV_ADS std::stof(config["AIMFOV_ADS"])
    #define AIMFOV_HIPFIRE std::stof(config["AIMFOV_HIPFIRE"])
    #define AIMSMOOTH std::stof(config["AIMSMOOTH"])
    #define ITEM_ESP std::stoi(config["ITEM_ESP"])
    #define AIMBOT_ENABLED std::stoi(config["AIMBOT_ENABLED"])
    std::chrono::milliseconds sleep(std::stoi(config["SLEEP"]));
    float maxdistance = std::stof(config["MAXDISTANCE"]);
	
	// Parse bone_list configuration
	std::string boneListStr = config["BONE_LIST"];
	std::vector<int> boneList;
	std::istringstream iss(boneListStr);
	std::string bone;
	while (std::getline(iss, bone, ',')) {
		boneList.push_back(std::stoi(bone));
		}

	int bone_list[boneList.size()];
	std::copy(boneList.begin(), boneList.end(), bone_list);

	int pid = GetApexProcessId();

	if (pid == 0)
	{
		printf("[-] r5apex.exe was not found\n");
		return 0;
	}

	rx_handle r5apex = rx_open_process(pid, RX_ALL_ACCESS);
	if (r5apex == 0)
	{
		printf("[-] unable to attach r5apex.exe\n");
		return 0;
	}

	printf("[+] r5apex.exe pid [%d]\n", pid);

	//
	// get base address
	// in case this function doesn't work, use QWORD base_module = 0x140000000;
	//

	QWORD base_module = 0x140000000;
	if (base_module == 0)
	{
		return 0;
		printf("[+] r5apex.exe base [0x%lx]\n", base_module);
	}

	printf("[+] r5apex.exe base [0x%lx]\n", base_module);

	DWORD dwVisibleTime = 0;

	QWORD base_module_dump = rx_dump_module(r5apex, base_module);

	if (base_module_dump == 0)
	{
		printf("[-] failed to dump r5apex.exe\n");
		rx_close_handle(r5apex);
		return 0;
	}

	QWORD IClientEntityList = 0;
	{
		char pattern[] = "\x4C\x8B\x15\x00\x00\x00\x00\x33\xF6";
		char mask[] = "xxx????xx";

		// IClientEntityList = 0x1a203b8 + base_module + 0x280050;
		IClientEntityList = rx_scan_pattern(base_module_dump, pattern, mask, 9);
		if (IClientEntityList)
		{
			IClientEntityList = ResolveRelativeAddressEx(r5apex, IClientEntityList, 3, 7);
			IClientEntityList = IClientEntityList + 0x08;
		}
	}


	QWORD IInputSystem = 0;
	{
		// 48 8B 05 ? ? ? ? 48 8D 4C  24 20 BA 01 00 00 00 C7
		char pattern[] = "\x48\x8B\x05\x00\x00\x00\x00\x48\x8D\x4C\x24\x20\xBA\x01\x00\x00\x00\xC7";
		char mask[] = "xxx????xxxxxxxxxxx";

		IInputSystem = rx_scan_pattern(base_module_dump, pattern, mask, 18);
		IInputSystem = ResolveRelativeAddressEx(r5apex, IInputSystem, 3, 7);
		IInputSystem = IInputSystem - 0x10;
	}

	QWORD GetAllClasses = 0;
	{
		// 48 8B 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 89 74 24 20
		char pattern[] = "\x48\x8B\x05\x00\x00\x00\x00\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x74\x24\x20";
		char mask[] = "xxx????xxxxxxxxxxxxxx";
		GetAllClasses = rx_scan_pattern(base_module_dump, pattern, mask, 21);
		GetAllClasses = ResolveRelativeAddressEx(r5apex, GetAllClasses, 3, 7);
		GetAllClasses = rx_read_i64(r5apex, GetAllClasses);
	}

	QWORD sensitivity = 0;
	sensitivity = base_module + m_sensitivity; //mouse_sensitivity

	{

		char pattern[] = "\x75\x0F\xF3\x44\x0F\x10\xBF";
		char mask[] = "xxxxxxx";
		QWORD temp_address = rx_scan_pattern(base_module_dump, pattern, mask, 7);
		if (temp_address)
		{

			QWORD bullet_gravity = temp_address + 0x02;
			bullet_gravity = bullet_gravity + 0x05;

			QWORD bullet_speed = temp_address - 0x6D;
			bullet_speed = bullet_speed + 0x04;

		}
	}

	

	{
		// 48 8B CE  ? ? ? ? ? 84 C0 0F 84 BA 00 00 00
		char pattern[] = "\x48\x8B\xCE\x00\x00\x00\x00\x00\x84\xC0\x0F\x84\xBA\x00\x00\x00";
		char mask[] = "xxx?????xxxxxxxx";
		QWORD vis_time = rx_scan_pattern(base_module_dump, pattern, mask, 16);
		if (vis_time)
		{
			vis_time = vis_time + 0x10;
			dwVisibleTime = rx_read_i32(r5apex, vis_time + 0x4);
			 dwVisibleTime = 0x1A70;	//lastVisibleTime
		}
	}

	rx_free_module(base_module_dump);

	while (GetAllClasses)
	{

		QWORD recv_table = rx_read_i64(r5apex, GetAllClasses + 0x18);
		QWORD recv_name = rx_read_i64(r5apex, recv_table + 0x4C8);

		char name[260];
		rx_read_process(r5apex, recv_name, name, 260);

		if (!strcmp(name, "DT_Player"))
		{
			m_bZooming = dump_table(r5apex, recv_table, "m_bZooming");
			m_lifeState = dump_table(r5apex, recv_table, "m_lifeState");
			m_iCameraAngles = dump_table(r5apex, recv_table, "m_zoomFullStartTime") + 0x2EC;
		}

		

		if (!strcmp(name, "DT_BaseEntity"))
		{
			m_iTeamNum = dump_table(r5apex, recv_table, "m_iTeamNum");
			
		}

		if (!strcmp(name, "DT_BaseCombatCharacter"))
		{
			m_iWeapon = dump_table(r5apex, recv_table, "m_latestPrimaryWeapons");
		}

		if (!strcmp(name, "DT_BaseAnimating"))
		{
			m_iBoneMatrix = dump_table(r5apex, recv_table, "m_nForceBone") + 0x50 - 0x8;
		}

		if (!strcmp(name, "DT_WeaponX"))
		{
			m_playerData = dump_table(r5apex, recv_table, "m_playerData");
		}

		GetAllClasses = rx_read_i64(r5apex, GetAllClasses + 0x20);
	}


	DWORD previous_tick = 0;
	float lastvis_aim[70];
	memset(lastvis_aim, 0, sizeof(lastvis_aim));

	if (IClientEntityList == 0)
	{
		printf("[-] IClientEntityList not found\n");
		goto ON_EXIT;
	}

	if (IInputSystem == 0)
	{
		printf("[-] IInputSystem not found\n");
		goto ON_EXIT;
	}

	if (sensitivity == 0)
	{
		printf("[-] sensitivity not found\n");
		goto ON_EXIT;
	}




	if (dwVisibleTime == 0)
	{
		printf("[-] dwVisibleTime not found\n");
		goto ON_EXIT;
	}


	printf("[+] IClientEntityList: %lx\n", IClientEntityList - base_module);
	printf("[+] IInputSystem: %lx\n", IInputSystem - base_module);
	printf("[+] sensitivity: %lx\n", sensitivity - base_module);
	printf("[+] dwVisibleTime: %x\n", dwVisibleTime);
	printf("[+] m_iViewAngles: %x\n", m_iViewAngles);
	printf("[+] m_bZooming: %x\n", m_bZooming);
	printf("[+] m_iCameraAngles: %x\n", m_iCameraAngles);
	printf("[+] m_lifeState: %x\n", m_lifeState);
	printf("[+] m_iTeamNum: %x\n", m_iTeamNum);
	printf("[+] m_vecAbsOrigin: %x\n", m_vecAbsOrigin);
	printf("[+] m_iWeapon: %x\n", m_iWeapon);
	printf("[+] m_iBoneMatrix: %x\n", m_iBoneMatrix);
	printf("[+] m_playerData: %x\n", m_playerData);

	fflush(stdout);

	// printf("[+] GameMode: %d", mode);

	while (1)
	{
		float fovAds = AIMFOV_ADS;
		float fovHipfire = AIMFOV_HIPFIRE;
		itemWorkaround++;
		//printf("TESTEEEE -- %i",itemWorkaround);
		
		if (!rx_process_exists(r5apex))
		{
			break;
		}

		uint64_t gameModePtr = rx_read_i32(r5apex, base_module + 0x01e87f30 + 0x58);
		int gameMode = rx_read_int(r5apex, gameModePtr);
		// printf("\r[+] Game Mode Int: %d", gameMode);
		// fflush(stdout);

		QWORD localplayer = rx_read_i64(r5apex, base_module + m_localplayer);

		if (localplayer == 0)
		{
			previous_tick = 0;
			memset(lastvis_aim, 0, sizeof(lastvis_aim));
			continue;
		}

		

		DWORD local_team = rx_read_i32(r5apex, localplayer + m_iTeamNum);

		float fl_sensitivity = rx_read_float(r5apex, sensitivity + 0x68);
		DWORD weapon_id = rx_read_i32(r5apex, localplayer + m_iWeapon) & 0xFFFF;
		QWORD weapon = GetClientEntity(r5apex, IClientEntityList, weapon_id - 1);

		float bulletSpeed = rx_read_float(r5apex, weapon + m_bulletSpeed);
		float bulletGravity = rx_read_float(r5apex, weapon + m_bulletGravity);

		vec3 muzzle;
		rx_read_process(r5apex, localplayer + m_muzzle, &muzzle, sizeof(vec3)); //CPlayer!camera_origin

		float target_fov = 360.0f;
		QWORD target_entity = 0;

		vec3 local_position;
		rx_read_process(r5apex, localplayer + m_vecAbsOrigin, &local_position, sizeof(vec3));



		for (int i = 0; i < 70; i++)
		{
			QWORD entity = GetClientEntity(r5apex, IClientEntityList, i);

			int EntTeam = rx_read_i32(r5apex, entity + m_iTeamNum);
			if (EntTeam % 2)
			{
				iTeamControl = 1;
			}
			else
			{
				iTeamControl = 2;
			}
			int LocTeam = rx_read_i32(r5apex, localplayer + m_iTeamNum);
			if (LocTeam % 2)
			{
				iLocControl = 1;
			}
			else
			{
				iLocControl = 2;
			}

			if (gameMode == 1953394531)
			{
				if (iTeamControl == iLocControl)
					continue;
			}

			if (entity == 0)
				continue;

			if (entity == localplayer)
				continue;
			

			if (rx_read_i32(r5apex, entity + m_iHealth) == 0)
			{
				lastvis_aim[i] = 0;
				continue;
			}

			/*	if (rx_read_i32(r5apex, entity + m_iName) != 125780153691248)
				{
					continue;
				} */

			if (rx_read_i32(r5apex, entity + m_iTeamNum) == local_team)
			{
				continue;
			}

			// if (rx_read_i32(r5apex, entity + m_bleedoutState) == 0)
			//{
			// continue;
			//}

			if (rx_read_i32(r5apex, entity + m_lifeState) != 0)
			{
				lastvis_aim[i] = 0;
				continue;
			}

			vec3 head = GetBonePosition(r5apex, entity, 2);
			

			vec3 velocity;
			rx_read_process(r5apex, entity + m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

			float fl_time = vec_distance(head, muzzle) / bulletSpeed;
			head.z += (700.0f * bulletGravity * 0.5f) * (fl_time * fl_time);

			velocity.x = velocity.x * fl_time;
			velocity.y = velocity.y * fl_time;
			velocity.z = velocity.z * fl_time;

			head.x += velocity.x;
			head.y += velocity.y -40;
			head.z += velocity.z;

			vec3 target_angle = CalcAngle(muzzle, head);
			vec3 breath_angles;

			rx_read_process(r5apex, localplayer + m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

			float last_visible = rx_read_float(r5apex, entity + dwVisibleTime);
			// glow enable
			rx_write_i32(r5apex, entity + 0x2C4, 1512990053);
			rx_write_i32(r5apex, entity + 0x3c8, 1);
			rx_write_i32(r5apex, entity + 0x3d0, 2);
			
			if (last_visible != 0.00f)
			{

				float fov = get_fov(breath_angles, target_angle);

				//set default glow color - white
				//rx_write_float(r5apex, entity + 0x1D0, 5.0f);	//Red
				//rx_write_float(r5apex, entity + 0x1D4, 5.0f);	//Green
				rx_write_float(r5apex, entity + 0x1D8, 100.0f);	//Blue
				
				if (fov < target_fov && last_visible > lastvis_aim[i]) // i think this if is not working, always false
				{

					target_fov = fov;
					target_entity = entity;
					lastvis_aim[i] = last_visible;

					// 
					rx_write_float(r5apex, entity + 0x3B4, 99999999.0f); // glow distance

					// Health based glow...
					/*if (rx_read_i32(r5apex, entity + 0x0170) <= 10)
					{
						// green - VERY LOW SHIELD
						rx_write_float(r5apex, entity + 0x1D0, 0.0f);
						rx_write_float(r5apex, entity + 0x1D4, 100.0f);
						rx_write_float(r5apex, entity + 0x1D8, 0.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 50)
					{
						// white - 2 BARS
						rx_write_float(r5apex, entity + 0x1D0, 255.0f);
						rx_write_float(r5apex, entity + 0x1D4, 255.0f);
						rx_write_float(r5apex, entity + 0x1D8, 255.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 75)
					{
						// BLUE - 3 BARS
						rx_write_float(r5apex, entity + 0x1D0, 0.0f);
						rx_write_float(r5apex, entity + 0x1D4, 117.0f);
						rx_write_float(r5apex, entity + 0x1D8, 209.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 100)
					{
						// PURPLE - 4 BARS
						rx_write_float(r5apex, entity + 0x1D0, 126.0f);
						rx_write_float(r5apex, entity + 0x1D4, 0.0f);
						rx_write_float(r5apex, entity + 0x1D8, 255.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 125)
					{
						// RED = 5 BARS
						rx_write_float(r5apex, entity + 0x1D0, 255.0f);
						rx_write_float(r5apex, entity + 0x1D4, 0.0f);
						rx_write_float(r5apex, entity + 0x1D8, 0.0f);
					}
				}
				else
				{

					rx_write_float(r5apex, entity + 0x3B4, 99999999.0f); // glow distance

					if (rx_read_i32(r5apex, entity + 0x0170) <= 10)
					{
						// green - VERY LOW SHIELD
						rx_write_float(r5apex, entity + 0x1D0, 0.0f);
						rx_write_float(r5apex, entity + 0x1D4, 100.0f);
						rx_write_float(r5apex, entity + 0x1D8, 0.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 50)
					{
						// white - 2 BARS
						rx_write_float(r5apex, entity + 0x1D0, 255.0f);
						rx_write_float(r5apex, entity + 0x1D4, 255.0f);
						rx_write_float(r5apex, entity + 0x1D8, 255.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 75)
					{
						// BLUE - 3 BARS
						rx_write_float(r5apex, entity + 0x1D0, 0.0f);
						rx_write_float(r5apex, entity + 0x1D4, 117.0f);
						rx_write_float(r5apex, entity + 0x1D8, 209.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 100)
					{
						// PURPLE - 4 BARS
						rx_write_float(r5apex, entity + 0x1D0, 126.0f);
						rx_write_float(r5apex, entity + 0x1D4, 0.0f);
						rx_write_float(r5apex, entity + 0x1D8, 255.0f);
					}
					else if (rx_read_i32(r5apex, entity + 0x0170) <= 125)
					{
						// RED = 5 BARS
						rx_write_float(r5apex, entity + 0x1D0, 255.0f);
						rx_write_float(r5apex, entity + 0x1D4, 0.0f);
						rx_write_float(r5apex, entity + 0x1D8, 0.0f);
					}*/
				}
			}
		}

		if (target_entity && (IsButtonDown(r5apex, IInputSystem, AIMKEY) || IsButtonDown(r5apex, IInputSystem, AIMKEY2)) && AIMBOT_ENABLED == 1)
		{


			
			if (rx_read_i32(r5apex, target_entity + m_iHealth) == 0)
				continue;
			if (rx_read_i32(r5apex, target_entity + m_bleedoutState) > 0) // ignore knock
				continue;
			
		
			vec3 enmPos;

			rx_read_process(r5apex, localplayer + m_vecAbsOrigin, &local_position, sizeof(vec3));
			rx_read_process(r5apex, target_entity + m_vecAbsOrigin, &enmPos, sizeof(vec3)); // enemy position



			//enmPos.x = 31518;
			//enmPos.y = -6712;
			//enmPos.z = -29235;


			float distance = ((CalcDistance(local_position, enmPos) / 100) * 2);   // need to verify
			//printf("  	distance %f", ((CalcDistance(local_position, enmPos))/100)*2);
			bool far = (distance >= maxdistance);

			if (far)
			{
				//printf(" Cancelling ");
				continue;
			}

			//printf(" Continue ");

			vec3 target_angle = {0, 0, 0};
			float fov = 360.0f;
			// alteration hitbox
			// int bone_list[] = {2, 3, 5, 8};
			//int bone_list[] = {5,5,5,5}; // chest

			vec3 breath_angles;
			rx_read_process(r5apex, localplayer + m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

			for (int i = 0; i < 4; i++)
			{
				vec3 head = GetBonePosition(r5apex, target_entity, bone_list[i]);

				vec3 velocity;
				rx_read_process(r5apex, target_entity + m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

				float fl_time = vec_distance(head, muzzle) / bulletSpeed;

				head.z += (700.0f * bulletGravity * 0.5f) * (fl_time * fl_time);

				velocity.x = velocity.x * fl_time;
				velocity.y = velocity.y * fl_time;
				velocity.z = velocity.z * fl_time;

				head.x += velocity.x;
				head.y += velocity.y;
				head.z += velocity.z;

				vec3 angle = CalcAngle(muzzle, head);
				float temp_fov = get_fov(breath_angles, angle);
				if (temp_fov < fov)
				{
					fov = temp_fov;
					target_angle = angle;
				}
			}

			DWORD weapon_id = rx_read_i32(r5apex, localplayer + m_iWeapon) & 0xFFFF;
			QWORD weapon = GetClientEntity(r5apex, IClientEntityList, weapon_id - 1);
			float zoom_fov = rx_read_float(r5apex, weapon + m_playerData + 0xb8);

			if (rx_read_i8(r5apex, localplayer + m_bZooming))
			{
				fl_sensitivity = (zoom_fov / 90.0f) * fl_sensitivity;
				fovAds = AIMFOV_ADS;
			}else{
				fovAds = fovHipfire;
			}



			if (fov <= fovAds)
			{

				vec3 angles;
				angles.x = breath_angles.x - target_angle.x;
				angles.y = breath_angles.y - target_angle.y;
				angles.z = 0;
				vec_clamp(&angles);

				float x = angles.y;
				float y = angles.x;
				x = (x / fl_sensitivity) / 0.022f;
				y = (y / fl_sensitivity) / -0.022f;

				float sx = 0.0f, sy = 0.0f;

				float smooth = AIMSMOOTH;

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
				}
				else
				{
					sx = x;
					sy = y;
				}

				if (qabs((int)sx) > 100)
					continue;

				if (qabs((int)sy) > 100)
					continue;

				DWORD current_tick = rx_read_i32(r5apex, IInputSystem + 0xcd8);
				if (current_tick - previous_tick > aim_ticks)
				{
					previous_tick = current_tick;
					typedef struct
					{
						int x, y;
					} mouse_data;
					mouse_data data;

					data.x = (int)sx;
					data.y = (int)sy;
					rx_write_process(r5apex, IInputSystem + 0x1DB0, &data, sizeof(data));
					//printf(" x %i y %i",data.x,data.y);
					std::this_thread::sleep_for(sleep);
				}
			}
		}

		if(itemWorkaround > 5001){
			itemWorkaround = 0;
		}

		

		//item glow
		if (itemWorkaround == 5000 && ITEM_ESP == 1)
		//printf("GLOW -- %i",itemWorkaround);
		{
			for (int k = 0; k < 10000; k++)
			{
				QWORD entity = GetClientEntity(r5apex, IClientEntityList, k);
				itemID = rx_read_int(r5apex, entity + m_itemId);

				//https://www.unknowncheats.me/forum/apex-legends/319804-apex-legends-reversal-structs-offsets-572.html

				switch (itemID)
				{
				//weapons
				case 23: 	 // VK-47 Flatline
				case 69:	 // R-301 Carbine
				case 49: 	 // r99
				case 90: 	 //wingman
				case 89:	 //PK
				//case 7:	 //L-Star

				//shields
				//case 175: // Evo Shield 2
				case 182: // Evo Shield 3
				case 183: // Evo Shield 4
				//case 170: // Shield 2
				case 178: // Shield 3
				case 179: // Shield 4

				//helmets
				//case 166: // Helmet 2
				case 172: // Helmet 3
				case 173: // Helmet 4

				//backpacks
				case 190: // Backpack 2
				case 191: // Backpack 3
				case 192: // Backpack 4

				//attachments
				//case 222: //Extended Sniper Mag 3
				//case 223: //Extended Sniper Mag 4
				case 216: //Extended Light Mag 3
				case 217: //Extended Light Mag 4
				case 224: //Energy Magazine 3
				case 225: //Energy Magazine 4
				case 220: //Heavy Magazine 3
				case 221: //Heavy Magazine 4
				//case 203: //Barrel Stabilizer 3
				//case 204: //Barrel Stabilizer 4
				//case 232: //Sniper Stock 3
				case 232: //Shotgun Bolt 3
				case 233: //Shotgun Bolt 4

				//scopes
				//case 197: //1x HCOG Classic
				//case 198: //2x HCOG Bruiser
				//case 197: //6x sniper
				//case 199: //4x-8x Variable Sniper

				//miscellaneous
				case 188: //Knockdown Shield 4
				case 168: //Shield Battery
				case 169: //Shile Cell
				case 242: //Skullpiercer
				case 244: //Hammerpoint
				//case 258: //Treasure Box
				
					rx_write_i32(r5apex, entity + 0x02c0, 1363184265);
					break;
				}


			if (itemID == 27 || itemID == 73 || itemID == 43 || itemID == 83 || itemID == 98) 
                {
                    rx_write_i32(r5apex, entity + 0x3C8, 1);
                    rx_write_i32(r5apex, entity + 0x3D0, 2);
                    rx_write_i32(r5apex, entity + 0x2C4, 1512990053);
                    rx_write_float(r5apex, entity + 0x1D0, 61.f);
                    rx_write_float(r5apex, entity + 0x1D4, 2.f);
                    rx_write_float(r5apex, entity + 0x1D8, 2.f);
 
 
                }


			}
		}
	}

ON_EXIT:
	rx_close_handle(r5apex);
}
