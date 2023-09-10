#include "../../rx/rx.h"
#include "math.h"
#include "handling.h"
#include "offsets.cpp"
#include "readConfig.h"
#include "level.h"
#include "process.h"
#include "player.h"
#include "localplayer.h"
#include "menu.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <iomanip>
#include <random>

//#include <stdint.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/extensions/XTest.h>
//#include <unistd.h>

/* ------------------ CONFIGURATIONS ------------------ */
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

/* ------------------ START OFFSETS Global Declaration ------------------ */
int m_gameMode = OFFSETS::GAMEMODE;
int m_iWeapon = OFFSETS::iWEAPON;
int m_bZooming = OFFSETS::bZOOMING;
int m_lifeState = OFFSETS::LIFE_STATE;
int m_iTeamNum = OFFSETS::TEAM_NUMBER;
int m_iCameraAngles = OFFSETS::CAMERA_ANGLES;
int m_playerData = OFFSETS::PLAYER_DATA;
int m_iBoneMatrix = OFFSETS::BONE_MATRIX;

/* ------------------ Define default keycode ------------------ */
#define bunny_key 110
//#define auto_grapple_key 27
//#define map_radar_toggle_key 40
//#define ITEM_ESP_TOGGLE 26

/* ------------------ Variable Decleration ------------------ */
int itemWorkaround = 0;
int itemID;
int mode;
int iTeamControl;
int iLocControl;

//constexpr int MAX_ENTITIES = 70;

/* ------------------ ------------------ ------------------ */

int main(void)
{
	//Read Config.ini File
	std::map<std::string, std::string> config;
	readConfig(config);
	
	/* ------------------ Terminal Output ------------------ */
	void displayHeader();		//Heading
	void loadBar();				//Loading bar

	/* ------------------ Initialize temporary/local Value ------------------ */
	// Recoil Control Values
	//double m_previousPunchPitch = 0.0f;
	//double m_previousPunchYaw = 0.0f;
	const float maxAngleChangePerFrame = 1.0f; // Adjust as needed
    const float smoothFactor = 0.2f; // Adjust as needed
    vec3 oldPunch = { 0.0f, 0.0f, 0.0f };
	vec3 smoothedAngle = { 0.0f, 0.0f, 0.0f };

	//float my;
    //float mx;

	//Spec Count value
	int previousSpectatorCount = -1;

	// Toggle
	//bool mapRadarActive = true;
	//bool itemlobaESPactive = true;

	// Aim Tick
	DWORD previous_tick = 0;
	float lastvis_aim[70];
	memset(lastvis_aim, 0, sizeof(lastvis_aim));

	// VisCheck values for Concept 1 and 2
	//float entNewVisTime = 0;
	//float entOldVisTime[100];
	//int visCooldownTime[100];

	// Visible time structure for every entity
	VisibilityDataStr entityVisibilityData[70];
	MouseMovementData mouseData;

	//Triggerbot
	int loopsSinceLastCrosshair[70] = {0};
	LastCrosshairDataStr temp_crosshairData[70];
	bool bTrigger_cache = false;

	//used to help with the buffer (20 loops) for the lastVisibleTime
    int loopsSinceLastVisible[70] = {0};

	/* ------------------ Class instance ------------------ */
	Player playerClass;
	Level levelClass;
	LocalPlayer localplayerClass;

	/* ------------------ sleep duration ------------------ */
	// Seed the random number generator with the current time
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define a random distribution for the sleep duration
    std::uniform_int_distribution<> sleepDist(100, 500); 
	// Adjust the range as needed (100ms to 500ms in this example)

	/* ------------------ Get Process ID (GetApexProcessId) ------------------ */
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

	/* ------------------ Input Handler Keyboard/ Mouse ------------------ */
	//Mouse Input
	rx_handle mouseInput = rx_open_input(RX_INPUT_TYPE_MOUSE, RX_INPUT_MODE_SEND);
	/*	Ex:
		rx_send_input_key(mouseInput, RX_BTN_LEFT, true);		// Simulate a left mouse button press
		std::this_thread::sleep_for(std::chrono::seconds(5));	// Seconds Add a short delay to keep the key pressed
		std::this_thread::sleep_for(std::chrono::milliseconds(250))	//milliseconds
		rx_send_input_key(mouseInput, RX_BTN_LEFT, false);		// Simulate releasing the left mouse button
	*/
	//Keyboard Input
	rx_handle keyboardInput = rx_open_input(RX_INPUT_TYPE_KEYBOARD, RX_INPUT_MODE_SEND);
	/*	Ex:
		rx_send_input_key(keyboardInput, RX_KEY_A, true);		// Simulate pressing the "A" key
		std::this_thread::sleep_for(std::chrono::seconds(5));	// Add a short delay to keep the key pressed
		rx_send_input_key(keyboardInput, RX_KEY_A, false);		// Simulate releasing the "A" key
	*/


	/* ------------------ Get Base Address (GetApexBaseAddress) ------------------ */
	// base_module = GetApexBaseAddress();
	// in case this function doesn't work, use QWORD base_module = 0x140000000;

	QWORD base_module = 0x140000000;		// Manual Declaration
	
	if (base_module == 0)
	{
		return 0;
		printf("[+] r5apex.exe base [0x%lx]\n", base_module);
	}

	//printf("[+] r5apex.exe base [0x%lx]\n", base_module);
	
	/* ------------------ Base Module Dump ------------------ */
	QWORD base_module_dump = rx_dump_module(r5apex, base_module);

	if (base_module_dump == 0)
	{
		printf("[-] failed to dump r5apex.exe\n");
		rx_close_handle(r5apex);
		return 0;
	}

	/* ------------------ IDA Pattern Scan Values ------------------ */

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
	sensitivity = base_module + OFFSETS::m_sensitivity; //mouse_sensitivity
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

	DWORD dwVisibleTime = 0;
	{
		// 48 8B CE  ? ? ? ? ? 84 C0 0F 84 BA 00 00 00
		char pattern[] = "\x48\x8B\xCE\x00\x00\x00\x00\x00\x84\xC0\x0F\x84\xBA\x00\x00\x00";
		char mask[] = "xxx?????xxxxxxxx";
		QWORD vis_time = rx_scan_pattern(base_module_dump, pattern, mask, 16);
		if (vis_time)
		{
			vis_time = vis_time + 0x10;
			dwVisibleTime = rx_read_i32(r5apex, vis_time + 0x4);
			 dwVisibleTime = 0x1AA0;							//lastVisibleTime - [WeaponSettingsMeta].Base
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

	/*
	std::cout << "[+] IClientEntityList: " << std::hex << IClientEntityList - base_module << std::endl;
	std::cout << "[+] IInputSystem: " << std::hex << IInputSystem - base_module << std::endl;
	std::cout << "[+] sensitivity: " << std::hex << sensitivity - base_module << std::endl;
	//std::cout << "[+] dwVisibleTime: " << std::hex << dwVisibleTime << std::endl;
	//std::cout << "[+] m_iViewAngles: " << std::hex << m_iViewAngles << std::endl;
	std::cout << "[+] m_bZooming: " << std::hex << m_bZooming << std::endl;
	std::cout << "[+] m_iCameraAngles: " << std::hex << m_iCameraAngles << std::endl;
	std::cout << "[+] m_lifeState: " << std::hex << m_lifeState << std::endl;
	std::cout << "[+] m_iTeamNum: " << std::hex << m_iTeamNum << std::endl;
	//std::cout << "[+] m_vecAbsOrigin: " << std::hex << m_vecAbsOrigin << std::endl;
	std::cout << "[+] m_iWeapon: " << std::hex << m_iWeapon << std::endl;
	std::cout << "[+] m_iBoneMatrix: " << std::hex << m_iBoneMatrix << std::endl;
	std::cout << "[+] m_playerData: " << std::hex << m_playerData << std::endl;

	// printf("[+] GameMode: %d", mode);
	*/
	
	// Terminal Menu display boolean values of config
	void displayConfigValues(const std::map<std::string, std::string>& config);


	while (1)
	{
		//reloadConfigOnUpdate();

		float fovAds = AIMFOV_ADS;
		float fovHipfire = AIMFOV_HIPFIRE;
		float fovdeadzone = AIMFOV_DEADZONE;
		
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

		QWORD localplayer = rx_read_i64(r5apex, base_module + OFFSETS::m_localplayer);

		if (localplayer == 0)
		{
			previous_tick = 0;
			memset(lastvis_aim, 0, sizeof(lastvis_aim));
			continue;
		}


		DWORD local_team = rx_read_i32(r5apex, localplayer + m_iTeamNum);

		float fl_sensitivity = rx_read_float(r5apex, sensitivity + 0x68);
		DWORD weapon_id = rx_read_i32(r5apex, localplayer + m_iWeapon) & 0xFFFF;
		QWORD weapon = playerClass.GetClientEntity(r5apex, IClientEntityList, weapon_id - 1);

		float bulletSpeed = rx_read_float(r5apex, weapon + OFFSETS::m_bulletSpeed);
		float bulletGravity = rx_read_float(r5apex, weapon + OFFSETS::m_bulletGravity);

		vec3 muzzle;
		rx_read_process(r5apex, localplayer + OFFSETS::m_muzzle, &muzzle, sizeof(vec3)); //CPlayer!camera_origin

		float target_fov = 360.0f;
		QWORD target_entity = 0;

		vec3 local_position;
		rx_read_process(r5apex, localplayer + OFFSETS::m_vecAbsOrigin, &local_position, sizeof(vec3));
		
		/*
		// this vector is used to store entity data,
		// Efficiency: Populating the entities vector in advance allows for more efficient access to the entity data during subsequent iterations of the loop. 
		// Without this vector, you would need to repeatedly call playerClass.GetClientEntity(IClientEntityList, i) in the loop for each entity, 
		// which could be less efficient due to the overhead of multiple function calls.
		
		// This will store entity data in every iteration
		std::vector<QWORD> entities;
		for (int i = 0; i < MAX_ENTITIES; i++)
        {
            QWORD entity = playerClass.GetClientEntity(r5apex, IClientEntityList, i);
            entities.push_back(entity);
        }
		*/

		if (GLOW_ENABLED == 1 && (levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)) )
		{
			/*
			// If using Vector to store entity
			for (int i = 0; i < entities.size(); i++)
			{
				QWORD entity = entities[i];
				*/
			for (int i = 0; i < 70; i++)
			{
				QWORD entity = playerClass.GetClientEntity(r5apex, IClientEntityList, i);
				//is Entity valid
				if (entity == 0)
					continue;


				QWORD EntityHandleAddress = rx_read_i64(r5apex, entity + OFFSETS::NAME);
				// Read the string from EntityHandleAddress
				std::string Identifier = rx_read_string(r5apex, EntityHandleAddress);

				const char* IdentifierC = Identifier.c_str();
				// Compare the C-style string to "player"
				if (strcmp(IdentifierC, "player") != 0) 
				{
					//Entity team ID/Number
					int EntTeam = rx_read_i32(r5apex, entity + m_iTeamNum);
					if (EntTeam % 2)
					{
						iTeamControl = 1;
					}
					else
					{
						iTeamControl = 2;
					}

					//LocalPlayer Team ID/Number
					int LocTeam = rx_read_i32(r5apex, localplayer + m_iTeamNum);
					if (LocTeam % 2)
					{
						iLocControl = 1;
					}
					else
					{
						iLocControl = 2;
					}

					//GameMode check
					if (gameMode == 1953394531)
					{
						if (iTeamControl == iLocControl)
							continue;
					}

					// If entity is local player then skip this iteration
					if (entity == localplayer)
						continue;

					/*	if (rx_read_i32(r5apex, entity + m_iName) != 125780153691248)
						{
							continue;
						} */

					// if entity TeamId is equal to Local TeamID then skip this iteration
					// if (rx_read_i32(r5apex, entity + m_iTeamNum) == local_team)
					if (EntTeam == LocTeam)
					{
						continue;
					}

					/*
					//entity position
					vec3 enmPos_glow;
					rx_read_process(r5apex, entity + OFFSETS::m_vecAbsOrigin, &enmPos_glow, sizeof(vec3));
					// calculate distance between local player and entity and scale the value to match the unit
					float glow_distance = ((CalcDistance(local_position, enmPos_glow) / 100) * 2);
					
					// bool to check entity distance and max glow distance in config
					bool enmfar_glow = (glow_distance >= GLOW_RANGE);
					// if distance is greater than max glow distance in config this iteration will be skipped
					if (enmfar_glow)
					{
						//printf(" Cancelling ");
						continue;
					}
					*/

					//Entity Health
					if (rx_read_i32(r5apex, entity + OFFSETS::m_iHealth) == 0)
					{
						lastvis_aim[i] = 0;
						continue;
					}

					// Get entity knocked state
					int entKnockedState = rx_read_i32(r5apex, entity + OFFSETS::m_bleedoutState);
					
					if (rx_read_i32(r5apex, entity + m_lifeState) != 0)
					{
						lastvis_aim[i] = 0;
						continue;
					}
					
					//Logic to change entity color when in FOV or AimLock
					vec3 hitbox = playerClass.GetBonePosition(r5apex, entity, 2);

					vec3 velocity;
					rx_read_process(r5apex, entity + OFFSETS::m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

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
					rx_read_process(r5apex, localplayer + OFFSETS::m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

					float entNewVisTime = rx_read_float(r5apex, entity + dwVisibleTime);

					// Get the shield value to change the color of the glow
					int shield = rx_read_i32(r5apex, entity + OFFSETS::m_shieldHealth);

					//Initialize R, G, and B with default values
					int R, G, B;

					/*
					if (EntTeam != LocTeam)
					{
					*/
					// Glow enable
					rx_write_i32(r5apex, entity + 0x3F8, 1);
					rx_write_i32(r5apex, entity + 0x400, 2);
						
					// Glow Mode
					//rx_write_i32(r5apex, entity + 0x2F4, 1512990053);
					GlowMode glowModeData = { 101, 101, 46, 90 };
					rx_write_array(r5apex, entity + 0x2F4, (char*)&glowModeData, sizeof(GlowMode));

					// Current entity Last Visible check
					bool Visible = playerClass.IsEntityVisible(r5apex, entity, entityVisibilityData[i]);

					// Local Player Field of View
					float fov = get_fov(breath_angles, target_angle);

					if (entKnockedState == 0)
					{
						/*
						if (entNewVisTime != 0.00f && fov < target_fov && entNewVisTime > lastvis_aim[i])
						{

							target_fov = fov;
							target_entity = entity;
							lastvis_aim[i] = entNewVisTime;

							rx_write_float(r5apex, entity + 0x3E4, 99999999.0f);

							rx_write_float(r5apex, entity + 0x200, 255.0f);   // Red
							rx_write_float(r5apex, entity + 0x204, 0.0f); // Green
							rx_write_float(r5apex, entity + 0x208, 255.0f);  // Blue
						}
						// Normal Glow Mode with Visible Check, does not work with Health Glow
						else */
						if (GLOW_ENEMY == 1 && GLOW_HEALTH == 0)
						{
							if (!Visible) 
							{ 
								//if not visible  
								if (loopsSinceLastVisible[i] < 20) { //if they have been visible 20 loops ago
									loopsSinceLastVisible[i]++; 
								} 
								else 
								{
									rx_write_float(r5apex, entity + 0x200, INVISIBLE_RED_VALUE);   // Red
									rx_write_float(r5apex, entity + 0x204, INVISIBLE_GREEN_VALUE); // Green
									rx_write_float(r5apex, entity + 0x208, INVISIBLE_BLUE_VALUE);  // Blue

									continue;
								}
							} 
							else if (Visible) 
							{
								loopsSinceLastVisible[i] = 0; //only if truely visible

								target_fov = fov;
								target_entity = entity;
								lastvis_aim[i] = entNewVisTime;

								rx_write_float(r5apex, entity + 0x3E4, 99999999.0f);

								rx_write_float(r5apex, entity + 0x200, VISIBLE_RED_VALUE);   // Red
								rx_write_float(r5apex, entity + 0x204, VISIBLE_GREEN_VALUE); // Green
								rx_write_float(r5apex, entity + 0x208, VISIBLE_BLUE_VALUE);  // Blue
							}
						}
						// Health Based glow ESP, No Visible Check
						else if (GLOW_HEALTH == 1)
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
								
							rx_write_float(r5apex, entity + 0x200, R); // Red
							rx_write_float(r5apex, entity + 0x204, G); // Green
							rx_write_float(r5apex, entity + 0x208, B); // Blue
						}
					}
					else
					{
						// If Entity/ Player is in Knockdown State
						rx_write_float(r5apex, entity + 0x200, 10.0f);   // Red
						rx_write_float(r5apex, entity + 0x204, 10.0f); // Green
						rx_write_float(r5apex, entity + 0x208, 10.0f);  // Blue
					}
				/*
					}
					else if (GLOW_SQUARD == 1)
					{	
						// Apply Color to Team/ Squard
						rx_write_float(r5apex, entity + 0x200, squard_redValue);   // Red
						rx_write_float(r5apex, entity + 0x204, squard_greenValue); // Green
						rx_write_float(r5apex, entity + 0x208, squard_blueValue);  // Blue
					}
				*/
				}
			}
		}

		/*	// Visibility Check...................................................
				// concept 1..............................................
						// (entNewVisTime > entOldVisTime[i]) || (entNewVisTime < 0.0f && entOldVisTime[i] > 0.0f) (if visible)
						// entOldVisTime[i] = entNewVisTime	(at the end after applying color)

				// concept 2..............................................
						// entNewVisTime != entOldVisTime[i]
						// entOldVisTime[i] = entNewVisTime; (if visible)
						// visCooldownTime[i] <= 0
						// if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1; (at the end after applying color)

						if (GLOW_ENEMY == 1 && GLOW_HEALTH == 0)
						{
							// Is visible
							//if ( entNewVisTime != entOldVisTime[i] )		// Concept 1
							if ((entNewVisTime > entOldVisTime[i]) || (entNewVisTime < 0.0f && entOldVisTime[i] > 0.0f)) // Concept 2
							{
								if(Visible)
								{
									visCooldownTime[i] = 1; 	// Concept 1
									// low values mean less latency, increase if you observe the color changes on visible enemies

									if (entKnockedState == 0)
									{
										R = 0; G = 60; B = 0;
									}
									else
									{
										R = 10; G = 10; B = 10;
									}

									entOldVisTime[i] = entNewVisTime;	// Concept 1
								}
							}
							else
							{
								if (visCooldownTime[i] <= 0)	// Concept 1
								{
									if (entKnockedState == 0)
									{
										R = 60; G = 0; B = 0;
									}
									else
									{
										R = 10; G = 10; B = 10;
									}
								}
							}

							if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1;		// Concept 1
							entOldVisTime[i] = entNewVisTime;		// Concept 2
						}
		*/


		if (target_entity && (IsButtonDown(r5apex, IInputSystem, AIMKEY) || IsButtonDown(r5apex, IInputSystem, AIMKEY2)) && AIMBOT_ENABLED == 1)
		{
			if (rx_read_i32(r5apex, target_entity + OFFSETS::m_iHealth) == 0)
				continue;
			if (rx_read_i32(r5apex, target_entity + OFFSETS::m_bleedoutState) > 0) // ignore knock
				continue;


			vec3 enmPos;

			rx_read_process(r5apex, localplayer + OFFSETS::m_vecAbsOrigin, &local_position, sizeof(vec3)); // local Player Position
			rx_read_process(r5apex, target_entity + OFFSETS::m_vecAbsOrigin, &enmPos, sizeof(vec3)); // enemy position


			//enmPos.x = 31518;
			//enmPos.y = -6712;
			//enmPos.z = -29235;


			float distance = ((CalcDistance(local_position, enmPos) / 100) * 2);   // need to verify
			//printf("  	distance %f", ((CalcDistance(local_position, enmPos))/100)*2);
			bool far = (distance >= AIMBOT_MAXDISTANCE);

			if (far)
			{
				//printf(" Cancelling ");
				continue;
			}

			//printf(" Continue ");

			// prediction
			vec3 target_angle = {0, 0, 0};
			float fov = 360.0f;

			// alteration hitbox
			// int bone_list[] = {2, 3, 5, 8};
			//int bone_list[] = {5,5,5,5}; // chest

			vec3 breath_angles;
			rx_read_process(r5apex, localplayer + OFFSETS::m_iViewAngles - 0x10, &breath_angles, sizeof(vec3));

			for (int bone : BONE_LIST)
			{
				vec3 hitbox = playerClass.GetBonePosition(r5apex, target_entity, bone);

				vec3 velocity;
				rx_read_process(r5apex, target_entity + OFFSETS::m_vecAbsOrigin - 0xC, &velocity, sizeof(vec3));

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

			DWORD weapon_id = rx_read_i32(r5apex, localplayer + m_iWeapon) & 0xFFFF;
			QWORD weapon = playerClass.GetClientEntity(r5apex, IClientEntityList, weapon_id - 1);
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

				//angles = ApplyDeadzone(angles, fovdeadzone, fovdeadzone); // Apply deadzone logic using the defined fovdeadzone value
				//angles = ApplyDeadzone(angles, vec2(fovdeadzone, fovdeadzone));

				/*
				vec3 deadzone = { fovdeadzone, fovdeadzone, 0.0f };
				angles = ApplyDeadzone(angles, deadzone);
				*/

				angles = ApplyDeadzone(angles, fovdeadzone);

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
					std::this_thread::sleep_for(AIMBOT_SLEEP);
				}
			}
		}

		if (NORECOIL_ENABLED == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)))
		{

			vec3 viewAngles;
			//rx_read_process(r5apex, target_entity + OFFSETS::m_iViewAngles, &viewAngles, sizeof(vec3));
			viewAngles.x = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles); // Read x
			viewAngles.y = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles + sizeof(float)); // Read y
			viewAngles.z = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles + 2 * sizeof(float)); // Read z

			vec3 punchAngle;
			punchAngle.x = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch); // Read x
			punchAngle.y = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + sizeof(float)); // Read y
			punchAngle.z = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + 2 * sizeof(float)); // Read z

			if( !localplayerClass.isInAttack(r5apex) )
			{
				// Reset the oldPunch values to zero when the condition is false.
				oldPunch.x = 0.0f;
				oldPunch.y = 0.0f;
				oldPunch.z = 0.0f;
			}
			else
			{

				vec3 newAngle;
				newAngle.x = viewAngles.x + (oldPunch.x - punchAngle.x) * (NORECOIL_STRENGTH / 100.f);
				newAngle.y = viewAngles.y + (oldPunch.y - punchAngle.y) * (NORECOIL_STRENGTH / 100.f);
				
				//newAngle.z = viewAngles.z; // Keep the z component the same
				//newAngle.z = 0; // Set z to zero

				/// Apply exponential smoothing to the new angles
				smoothedAngle.x = smoothFactor * newAngle.x + (1.0f - smoothFactor) * smoothedAngle.x;
				smoothedAngle.y = smoothFactor * newAngle.y + (1.0f - smoothFactor) * smoothedAngle.y;

				// Clamp the smoothedAngle
            	vec_clamp(&smoothedAngle);

				// Limit angle changes for smoother aiming
				smoothedAngle.x = std::max(std::min(smoothedAngle.x, viewAngles.x + maxAngleChangePerFrame), viewAngles.x - maxAngleChangePerFrame);
				smoothedAngle.y = std::max(std::min(smoothedAngle.y, viewAngles.y + maxAngleChangePerFrame), viewAngles.y - maxAngleChangePerFrame);

				// Keep the z component of view angles the same
				smoothedAngle.z = viewAngles.z;

				//vec3 newAngle;
                //newAngle.x = lerp(viewAngles.x, viewAngles.x + (oldPunch.x - punchAngle.x) * (rcsIntensity / 100.f), smoothFactor);
                //newAngle.y = lerp(viewAngles.y, viewAngles.y + (oldPunch.y - punchAngle.y) * (rcsIntensity / 100.f), smoothFactor);

                // Limit angle changes for smoother aiming
                //newAngle.x = std::max(std::min(newAngle.x, viewAngles.x + maxAngleChangePerFrame), viewAngles.x - maxAngleChangePerFrame);
                //newAngle.y = std::max(std::min(newAngle.y, viewAngles.y + maxAngleChangePerFrame), viewAngles.y - maxAngleChangePerFrame);

                // Keep the z component of view angles the same
                //newAngle.z = viewAngles.z;

				//vec_clamp(&newAngle);

				rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles, smoothedAngle.x); // Write x
				rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles + sizeof(float), smoothedAngle.y); // Write y
				rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles + 2 * sizeof(float), smoothedAngle.z);

				// Update oldPunch outside the inner block
				oldPunch.x = punchAngle.x;
				oldPunch.y = punchAngle.y;
				oldPunch.z = punchAngle.z;
			}
		}

		/*
		if (NORECOIL_ENABLED == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex))) 
		{
            //vec3 viewAngles;
            //rx_read_process(r5apex, localplayer + OFFSETS::m_iViewAngles, &viewAngles, sizeof(vec3));

            //vec3 weaponPunch;
            //rx_read_process(r5apex, localplayer + OFFSETS::m_iAimPunch, &weaponPunch, sizeof(vec3));

			vec3 viewAngles;
			viewAngles.x = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles); // Read x
			viewAngles.y = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles + sizeof(float)); // Read y
			viewAngles.z = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles + 2 * sizeof(float)); // Read z

			vec3 weaponPunch;
			weaponPunch.x = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch); // Read x
			weaponPunch.y = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + sizeof(float)); // Read y
			weaponPunch.z = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + 2 * sizeof(float)); // Read z

            if (!localplayerClass.isInAttack(r5apex)) {
                oldPunch.x = weaponPunch.x;
				oldPunch.y = weaponPunch.y;
				oldPunch.z = weaponPunch.z;
            } else {
                // Calculate new view angles with recoil compensation
                vec3 newAngle;
                newAngle.x = lerp(viewAngles.x, viewAngles.x + (oldPunch.x - weaponPunch.x) * (rcsIntensity / 100.f), smoothFactor);
                newAngle.y = lerp(viewAngles.y, viewAngles.y + (oldPunch.y - weaponPunch.y) * (rcsIntensity / 100.f), smoothFactor);

                // Limit angle changes for smoother aiming
                newAngle.x = std::max(std::min(newAngle.x, viewAngles.x + maxAngleChangePerFrame), viewAngles.x - maxAngleChangePerFrame);
                newAngle.y = std::max(std::min(newAngle.y, viewAngles.y + maxAngleChangePerFrame), viewAngles.y - maxAngleChangePerFrame);

                // Keep the z component of view angles the same
                newAngle.z = viewAngles.z;

                // Clamp angles (You need to provide the vec_clamp function)
                vec_clamp(&newAngle);

                // Write the clamped newAngle back to the game's memory
                //rx_write_process(r5apex, localplayer + OFFSETS::m_iViewAngles, &newAngle, sizeof(vec3));

				rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles, newAngle.x); // Write x
				rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles + sizeof(float), newAngle.y); // Write y
				//rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles + 2 * sizeof(float), newAngle.z);

                // Update oldPunch outside the inner block
                oldPunch.x = weaponPunch.x;
				oldPunch.y = weaponPunch.y;
				oldPunch.z = weaponPunch.z;
            }
        }
		*/
		
		/*
		if (NORECOIL2_ENABLED == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)))
		{
			if( localplayerClass.isInAttack(r5apex) && (IsButtonDown(r5apex, IInputSystem, AIMKEY) || IsButtonDown(r5apex, IInputSystem, AIMKEY2)))
			{	
				float punchPitch = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch);
				if (punchPitch != 0) {
					double pitch = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles);
					double punchPitchDelta = (punchPitch - m_previousPunchPitch) * norecoilPitchStrength;
					if (pitch - punchPitchDelta > 90 || pitch - punchPitchDelta < -90) {		// main value 90 and -90 .... testing with 89 and -89
						continue;
					}

					rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles, pitch - punchPitchDelta);
					m_previousPunchPitch = punchPitch;
				}

				float punchYaw = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + sizeof(float));
				if (punchYaw != 0) {
					double yaw = rx_read_float(r5apex, localplayer + OFFSETS::m_iViewAngles + sizeof(float));
					double punchYawDelta = (punchYaw - m_previousPunchYaw) * norecoilYawStrength;
					if (yaw - punchYawDelta > 180 || yaw - punchYawDelta < -180) {
						continue;
					}

					rx_write_float(r5apex, localplayer + OFFSETS::m_iViewAngles + sizeof(float), yaw - punchYawDelta);
					m_previousPunchYaw = punchYaw;
				}
			}
			else
			{
				double m_previousPunchPitch = 0.0f;
				double m_previousPunchYaw = 0.0f;
			}
		}
		*/

		/*
		if (NORECOIL2_ENABLED == 1) 
		{
			vec3 weaponPunch;
			//rx_read_process(r5apex, target_entity + OFFSETS::m_iAimPunch, &weaponPunch, sizeof(vec3));
			weaponPunch.x = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch); // Read x
			weaponPunch.y = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + sizeof(float)); // Read y
			weaponPunch.z = rx_read_float(r5apex, localplayer + OFFSETS::m_iAimPunch + 2 * sizeof(float)); // Read z

			// Read mouse sensitivity
			float sens = rx_read_float(r5apex, sensitivity + 0x68);

			// Isolate the weapon recoil by lookin when recoil pitch is negative
			// Other forms of recoil tend to have positive recoil pitch
			if (weaponPunch.x > 0.0f) {
				weaponPunch = { 0.0f, 0.0f, 0.0f };
			}

			// Initialize RCS when starting to attack
			if (!localplayerClass.isInAttack(r5apex)) {
				my = 0.0f;
				mx = 0.0f;
				//oldPunch = weaponPunch;
				oldPunch.x = weaponPunch.x;
				oldPunch.y = weaponPunch.y;
				oldPunch.z = weaponPunch.z;
			} 
			else 
			{
				// Pitch RCS only when it is increasing
				// It's unclear why this is so much more effective
				float pitch = std::max(0.0f, oldPunch.x - weaponPunch.x);
				float yaw = weaponPunch.y - oldPunch.y;
				//oldPunch = weaponPunch;
				oldPunch.x = weaponPunch.x;
				oldPunch.y = weaponPunch.y;
				oldPunch.z = weaponPunch.z;

				// Modulate the intensity by the user's sensitivity stuff
				float scale = sens * (1.0f / 0.022f);

				// Accumulate aiming deltas
				my += pitch * norecoilPitchStrength * scale;
				mx += yaw * norecoilYawStrength * scale;

				// Mouse movement is quantized, and keep track of residuals
				float myRounded = std::round(my);
				float mxRounded = std::round(mx);
				my -= myRounded;
				mx -= mxRounded;

				//mouseMov_data mouseData;
				mouseData.x = static_cast<int>(mxRounded);
				mouseData.y = static_cast<int>(myRounded);

				if (mouseData.x != 0 || mouseData.y != 0) {
					// Use rx_write_process to send mouseData to a memory address (IInputSystem + 0x1DB0)
					rx_write_process(r5apex, IInputSystem + 0x1DB0, &mouseData, sizeof(mouseData));
				}
			}
		}
		*/

		if (NORECOIL2_ENABLED == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex))) 
		{
			vec3 viewAngles;
			rx_read_process(r5apex, target_entity + OFFSETS::m_iViewAngles, &viewAngles, sizeof(vec3));

			vec3 weaponPunch;
			rx_read_process(r5apex, target_entity + OFFSETS::m_iAimPunch, &weaponPunch, sizeof(vec3));
			

			// Read mouse sensitivity
			float sens = rx_read_float(r5apex, sensitivity + 0x68);

			// Initialize RCS when starting to attack
			if (!localplayerClass.isInAttack(r5apex)) {
				// Player is not actively firing, reset oldPunch
				oldPunch = weaponPunch;
			} 
			else
			{
				// Calculate the desired view angles after recoil compensation
				vec3 desiredViewAngles;
				desiredViewAngles.x = viewAngles.x - (oldPunch.x - weaponPunch.x) * (sens / 0.022f);
				desiredViewAngles.y = viewAngles.y - (oldPunch.y - weaponPunch.y) * (sens / -0.022f);

				// Calculate the difference between current and desired view angles
				vec3 angleDifference;
				angleDifference.x = desiredViewAngles.x - viewAngles.x;
				angleDifference.y = desiredViewAngles.y - viewAngles.y;

				// Translate angle difference into mouse movement
				float mouseXDelta = (angleDifference.y); // Adjust as needed
				float mouseYDelta = (angleDifference.x); // Adjust as needed

				float smooth = sens; // Adjust for desired smoothing factor

				if (smooth >= 1.0f) {
					// Apply smoothing logic
					float smoothingFactor = 1.0f / smooth;

					if (mouseXDelta < 0) {
						mouseXDelta = (mouseXDelta + mouseXDelta * smoothingFactor);
					} else {
						mouseXDelta = (mouseXDelta - mouseXDelta * smoothingFactor);
					}

					if (mouseYDelta < 0) {
						mouseYDelta = (mouseYDelta + mouseYDelta * smoothingFactor);
					} else {
						mouseYDelta = (mouseYDelta - mouseYDelta * smoothingFactor);
					}
				}

				// Apply mouse movement to compensate for recoil
				MouseMovementData mouseData;
				mouseData.x = (int)mouseXDelta;
				mouseData.y = (int)mouseYDelta;

				// Write the mouse movement data to the game
				rx_write_process(r5apex, IInputSystem + 0x1DB0, &mouseData, sizeof(MouseMovementData));

				// Store the current weaponPunch as oldPunch for the next calculation
				oldPunch = weaponPunch;
			}
		}



		if (WEAPON_CHAMS == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)) )
		{
			//uintptr_t MyLocalplayer = rx_read_i64(r5apex, base_module + OFFSETS::m_localplayer);

			uintptr_t ViewModelHandle = rx_read_i64(r5apex, localplayer + OFFSETS::m_hViewModels)& 0xFFFF;
			uintptr_t ViewModelPtr = rx_read_i64(r5apex, base_module + OFFSETS::m_cl_entitylist + ViewModelHandle * 0x20);

			rx_write_i32(r5apex, ViewModelPtr + 0x3F8, 1);
			rx_write_i32(r5apex, ViewModelPtr + 0x400, 2);

			// GlowMode structure
			// glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
			// different types: { 135,135,80,0 }
			//					{ 101,102,96,90 }
			//GlowMode glowModeData = { 101, 101, 80, 0 };
			GlowMode glowModeData = { 101, 101, CHAM_BORDER, 0 };
			rx_write_array(r5apex, ViewModelPtr + 0x2F4, (char*)&glowModeData, sizeof(GlowMode));
			//rx_write_i32(r5apex, ViewModelPtr + 0x2F4, 1512990053);

			rx_write_float(r5apex, ViewModelPtr + 0x200, CHAMS_RED_VALUE);
			rx_write_float(r5apex, ViewModelPtr + 0x204, CHAMS_GREEN_VALUE);
			rx_write_float(r5apex, ViewModelPtr + 0x208, CHAMS_BLUE_VALUE);
		}

		if(ARMS_CHAMS == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)))
		{
			uintptr_t ViewModelArmHandle = rx_read_i64(r5apex, localplayer + OFFSETS::arm_ViewModels) & 0xFFFF;		//0x2d6C
			uintptr_t ViewModelArm = rx_read_i64(r5apex, base_module + OFFSETS::m_cl_entitylist + ViewModelArmHandle * 0x20);

			rx_write_i32(r5apex, ViewModelArm + 0x3F8, 1);
			rx_write_i32(r5apex, ViewModelArm + 0x400, 2);

			//GlowMode glowModeData = { 101, 101, 80, 0 };
			GlowMode glowModeData = { 101, 101, CHAM_BORDER, 0 };
			rx_write_array(r5apex, ViewModelArm + 0x2F4, (char*)&glowModeData, sizeof(GlowMode));

			rx_write_float(r5apex, ViewModelArm + 0x200, CHAMS_RED_VALUE);
			rx_write_float(r5apex, ViewModelArm + 0x204, CHAMS_GREEN_VALUE);
			rx_write_float(r5apex, ViewModelArm + 0x208, CHAMS_BLUE_VALUE);
		}

		if (MAP_RADAR == 1 && levelClass.isPlayable(r5apex) )
		{	
			/*
			if((IsButtonDown(r5apex, IInputSystem, map_radar_toggle_key)))
			{
				mapRadarActive = !mapRadarActive;
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
			}

			if(mapRadarActive)
			{
				*/
				//uintptr_t pLocal = rx_read_i64(r5apex, base_module + OFFSETS::m_localplayer);
				//int defTeam = rx_read_i32(r5apex, localplayer + m_iTeamNum);

				// Set the team to 1
				for (uintptr_t i = 0; i <= 8000; i++) //80000 , 8000
				{
					rx_write_i32(r5apex, localplayer + m_iTeamNum, 1);
				}
				// Restore the original team
				for (uintptr_t i = 0; i <= 8000; i++) //80000
				{
					rx_write_i32(r5apex, localplayer + m_iTeamNum, local_team);
				}
		}

		/*
		if ( TRIGGER_ENABLED==1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)))
		{
			//const long server_time = 0x1415FA4D8;  // 48 8B 05 ?? ?? ?? ?? F3 0F 58 70 10
			float currentServerTime = rx_read_float(r5apex, localplayer + OFFSETS::server_time);

			for (int h = 0; h < 70; h++) 
			{
				QWORD pEntity = playerClass.GetClientEntity(r5apex, IClientEntityList, h);

				if (!localplayerClass.isInAttack(r5apex) && currentServerTime > 0)
				{
					float crosshairTargetStartTime = rx_read_float(r5apex, pEntity + OFFSETS::START_CROSSHAIR_TIME);
					float lastCrosshairTargetTime = rx_read_float(r5apex, pEntity + OFFSETS::LAST_CROSSHAIR_TIME);

					if ( (crosshairTargetStartTime > 0) && (lastCrosshairTargetTime > crosshairTargetStartTime) && (lastCrosshairTargetTime >= currentServerTime - 0.1F) ) 
					{
						// check if we are on target long enough already to activate trigger
						if (lastCrosshairTargetTime > crosshairTargetStartTime + 0.05F) 
						{
							if(IsButtonDown(r5apex, IInputSystem, TRIGGER_KEY))
							{
								rx_send_input_key(mouseInput, RX_BTN_LEFT, true);
								std::this_thread::sleep_for(std::chrono::seconds(1));
								rx_send_input_key(mouseInput, RX_BTN_LEFT, false);
							}
						}
					}
				}
			}
		}
		*/

		if ( TRIGGER_ENABLED==1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)))
		{
			DWORD weapon_id = rx_read_i32(r5apex, localplayer + m_iWeapon) & 0xFFFF;
			QWORD weapon = playerClass.GetClientEntity(r5apex, IClientEntityList, weapon_id - 1);

			for (int p = 0; p < 70; p++) 
			{
				QWORD pEntity = playerClass.GetClientEntity(r5apex, IClientEntityList, p);

				bool Crosshair = playerClass.isCrosshair(r5apex, pEntity, temp_crosshairData[p]);

				//IsButtonDown(r5apex, IInputSystem, TRIGGER_KEY)
				auto tmp_triggerbot = false;
				if (!Crosshair)
				{ 
					//if not Crosshair  
					if (loopsSinceLastCrosshair[p] < 5) { //if they have been Crosshair 20 loops ago
						loopsSinceLastCrosshair[p]++;

						tmp_triggerbot = false;
					}
				}
				else if (Crosshair && (IsButtonDown(r5apex, IInputSystem, TRIGGER_KEY)))
				{
					loopsSinceLastCrosshair[p] = 0; //only if truely Crosshair

					int ammoInClip = rx_read_int(r5apex, weapon + OFFSETS::m_ammoInClip);
					float readyTime = rx_read_float(r5apex, weapon + OFFSETS::m_nextReadyTime);

					if (ammoInClip > 0 && readyTime == 0 )
					{
						rx_send_input_key(mouseInput, RX_BTN_LEFT, true);
						tmp_triggerbot = true;
					}
					else
					{
						tmp_triggerbot = false;
					}
				}
				
				if (bTrigger_cache == true && tmp_triggerbot == false) {
					rx_send_input_key(mouseInput, RX_BTN_LEFT, false);
				}
				bTrigger_cache = tmp_triggerbot;
			}
		}


		if (AUTO_GRAPPLE ==1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)) /* && (IsButtonDown(r5apex, IInputSystem, auto_grapple_key)) */)
		{
			rx_write_i32(r5apex, base_module + OFFSETS::in_jump + 0x8, 4);
			auto no_grapple = rx_read_int(r5apex, localplayer + OFFSETS::m_grapple + OFFSETS::m_grappleAttached);
			if (no_grapple == 1) {
				rx_write_i32(r5apex, base_module + OFFSETS::in_jump + 0x8, 5);
				rx_write_i32(r5apex, base_module + OFFSETS::m_grappleActivateTime, 1);
			}
		}

		if (SPEC_COUNT == 1 && levelClass.isPlayable(r5apex))
        {
			int spectatorcount = 0;

			for (int j = 0; j < 70; j++) {
				QWORD player = playerClass.GetClientEntity(r5apex, IClientEntityList, j);

				float targetangle = rx_read_float(r5apex, player + OFFSETS::spec_YAW);

				float targetyaw = -targetangle; // yaw is inverted
				if (targetyaw < 0)
					targetyaw += 360;
				targetyaw += 90; // yaw is off by 90
				if (targetyaw > 360)
					targetyaw -= 360;

				float localangle = rx_read_float(r5apex, localplayer + OFFSETS::spec_YAW);

				float localyaw = -localangle; // yaw is inverted
				if (localyaw < 0)
					localyaw += 360;
				localyaw += 90; // yaw is off by 90
				if (localyaw > 360)
					localyaw -= 360;

				if (targetyaw == localyaw && rx_read_i32(r5apex, player + OFFSETS::m_iHealth) == 0)
					spectatorcount++;
			}

			// Check if the value has changed
			if (spectatorcount != previousSpectatorCount) 
			{
				// Calculate the length of the text to be cleared (including ANSI escape codes)
				int textLength = snprintf(NULL, 0, "\033[32mSpectator: %-3d\033[0m", spectatorcount);

				// Clear the entire line by overwriting with spaces
				printf("\r%-*s", textLength, " ");

				// Print the updated value with the desired color
				if (spectatorcount >= 1) {
					printf("\033[32mSpectator: %-3d\033[0m", spectatorcount); // Green color
				} else {
					printf("Spectator: %-3d", spectatorcount);
				}

				previousSpectatorCount = spectatorcount;
				fflush(stdout);
			}
		}

		if(itemWorkaround > 5001)
		{
			itemWorkaround = 0;
		}

		//item glow
		if (itemWorkaround == 5000 && ITEM_ESP_ENABLED == 1 /*&& (isTrainingArea(r5apex) || isPlayable(r5apex) || isSpecialMode(r5apex))  &&  (IsButtonDown(r5apex, IInputSystem, ITEM_ESP_TOGGLE)) */)
		//printf("GLOW -- %i",itemWorkaround);
		{
			for (int k = 0; k < 10000; k++)
			{
				QWORD entity = playerClass.GetClientEntity(r5apex, IClientEntityList, k);
				itemID = rx_read_int(r5apex, entity + OFFSETS::m_itemId);

				// Apply ITEM_ESP settings only if ITEM_ESP is enabled and itemID is in ITEM_ESP_IDS
				if (ITEM_ESP == 1 && IsInItemEspIds(itemID, ITEM_ESP_IDS)) {
					rx_write_i32(r5apex, entity + 0x3F8, 1);
					rx_write_i32(r5apex, entity + 0x400, 2);
					rx_write_i32(r5apex, entity + 0x2F4, 1512990053);
					rx_write_float(r5apex, entity + 0x200, 61.f);
					rx_write_float(r5apex, entity + 0x204, 2.f);
					rx_write_float(r5apex, entity + 0x208, 2.f);
				}

				// Apply LOBA_ESP settings only if LOBA_ESP is enabled and itemID is in LOBA_ESP_IDS
				if (LOBA_ESP == 1 && IsInLobaEspIds(itemID, LOBA_ESP_IDS)) {
					//rx_write_i32(r5apex, entity + 0x292, 16256);
					//rx_write_i32(r5apex, entity + 0x30c, 1193322764);
					rx_write_i32(r5apex, entity + 0x2F0, 1363184265);
				}
			}
		}

				/*		
				if (ITEM_ESP == 1 && (itemID == 28 || itemID == 80 || itemID == 49 || itemID == 105 || itemID == 90 || itemID == 134))
				{
					rx_write_i32(r5apex, entity + 0x3F8, 1);
					rx_write_i32(r5apex, entity + 0x400, 2);
					rx_write_i32(r5apex, entity + 0x2F4, 1512990053);
					rx_write_float(r5apex, entity + 0x200, 61.f);
					rx_write_float(r5apex, entity + 0x204, 2.f);
					rx_write_float(r5apex, entity + 0x208, 2.f);
				}

				if (LOBA_ESP == 1 && (itemID == 195 || itemID == 242 || itemID == 241 || itemID == 235 || itemID == 185 || itemID == 205 || itemID == 194))
				{
					rx_write_i32(r5apex, entity + 0x292, 16256);  //0x262
					rx_write_i32(r5apex, entity + 0x30c, 1193322764);	//0x2dc
					//rx_write_i32(r5apex, entity + 0x3F8, 7);
					//rx_write_i32(r5apex, entity + 0x400, 2);
				}
				*/

		

		if (TAPSTRAFE == 1 && (levelClass.isTrainingArea(r5apex) || levelClass.isPlayable(r5apex) || levelClass.isSpecialMode(r5apex)) )
		{
			if (IsButtonDown(r5apex, IInputSystem, bunny_key))
			{
				int randomSleepDuration = sleepDist(gen);

				rx_send_input_key(keyboardInput, RX_KEY_W, true);
				rx_send_input_key(keyboardInput, RX_KEY_SPACE, true);

				// Sleep for the random duration
				std::this_thread::sleep_for(std::chrono::milliseconds(randomSleepDuration));

				rx_send_input_key(keyboardInput, RX_KEY_W, false);
				rx_send_input_key(keyboardInput, RX_KEY_SPACE, false);

				/*
				rx_write_i32(r5apex, base_module + OFFSETS::in_forward + 0x8, 5);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				rx_write_i32(r5apex, base_module + OFFSETS::in_jump + 0x8, 5);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));

				rx_write_i32(r5apex, base_module + OFFSETS::in_jump + 0x8, 4);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				//rx_write_i32(r5apex, base_module + OFFSETS::in_forward + 0x8, 4);
				//std::this_thread::sleep_for(std::chrono::milliseconds(5));
				*/
			}
			
		}

		/*
		if (FAKEDUCK == 1 && (IsButtonDown(r5apex, IInputSystem, FAKEDUCK_KEY)))
		{
				rx_write_float(r5apex, base_module + OFFSETS::host_timescale + 0x68, 0.0f);
			}
			// FakeDuck OFF
			else {
				rx_write_float(r5apex, base_module + OFFSETS::host_timescale + 0x68, 1.0f);
			}
		*/

		//std::this_thread::sleep_for(100);

	}

ON_EXIT:
	rx_close_handle(keyboardInput);
	rx_close_handle(mouseInput);
	rx_close_handle(r5apex);
}
