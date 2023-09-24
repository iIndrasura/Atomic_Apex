#include "main.hpp"

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
//#define map_radar_toggle_key 40
//#define ITEM_ESP_TOGGLE 26

// Variable Decleration
int mode;

int main(void)
{
    std::map<std::string, std::string> config;
    readConfig(config);

    // Initialize configuration values based on the read configuration
    InitConfigValues(config);
	
	/* ------------------ Terminal Output ------------------ */
	Menu::displayHeader();			//Heading
	Menu::loadBar();				//Loading bar

	// Toggle
	//bool mapRadarActive = true;
	//bool itemlobaESPactive = true;

	// Class Instance
	Aimbot aimbot;
	Glow glow(aimbot);
	ItemESP itemESP;
	TriggerBot triggerbot;
	NoRecoil rcs;
	Chams chams;
	SpectatorCount specCount;
	SkinChanger skinChanger;
	AutoGrapple Auto_Grapple;
	Radar radar;

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
			dwVisibleTime = 0x19ad + 0x3;							//lastVisibleTime - [WeaponSettingsMeta].Base
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

	
	//std::cout << "[+] IClientEntityList: " << std::hex << IClientEntityList - base_module << std::endl;
	// std::cout << "[+] IInputSystem: " << std::hex << IInputSystem - base_module << std::endl;
	// std::cout << "[+] sensitivity: " << std::hex << sensitivity - base_module << std::endl;
	//std::cout << "[+] dwVisibleTime: " << std::hex << dwVisibleTime << std::endl;
	// //std::cout << "[+] m_iViewAngles: " << std::hex << m_iViewAngles << std::endl;
	// std::cout << "[+] m_bZooming: " << std::hex << m_bZooming << std::endl;
	// std::cout << "[+] m_iCameraAngles: " << std::hex << m_iCameraAngles << std::endl;
	// std::cout << "[+] m_lifeState: " << std::hex << m_lifeState << std::endl;
	// std::cout << "[+] m_iTeamNum: " << std::hex << m_iTeamNum << std::endl;
	// //std::cout << "[+] m_vecAbsOrigin: " << std::hex << m_vecAbsOrigin << std::endl;
	// std::cout << "[+] m_iWeapon: " << std::hex << m_iWeapon << std::endl;
	// std::cout << "[+] m_iBoneMatrix: " << std::hex << m_iBoneMatrix << std::endl;
	// std::cout << "[+] m_playerData: " << std::hex << m_playerData << std::endl;

	// printf("[+] GameMode: %d", mode);
	
	
	// Terminal Menu display boolean values of config
	Menu::displayConfigValues();


	while (1)
	{
		if (!rx_process_exists(r5apex))
		{
			break;
		}

		uint64_t gameModePtr = rx_read_i32(r5apex, base_module + 0x01e87f30 + 0x58);
		int gameMode = rx_read_int(r5apex, gameModePtr);
		// printf("\r[+] Game Mode Int: %d", gameMode);
		// fflush(stdout);

		// Glow
		glow.ActivateGlow(r5apex, IClientEntityList, sensitivity, gameMode/* , m_iTeamNum, m_iWeapon, m_lifeState */);
		//glow.ActivateTrainingGlow(r5apex );

		// Aimbot
		aimbot.ActivateAimbot(r5apex, IInputSystem, IClientEntityList, sensitivity, m_playerData, m_bZooming);

		// No recoil
		rcs.ApplyNoRecoil1(r5apex, IInputSystem);

		// Chams	
		chams.ApplyChamsLogic(r5apex);

		//Trigger bot
		triggerbot.ApplyTriggerBot(r5apex, IClientEntityList, IInputSystem, mouseInput);

		// Skin chnager
		skinChanger.ApplySkinChanger(r5apex, IClientEntityList);

		// Auto Grapple
		Auto_Grapple.ApplyAutoGrapple(r5apex);

		// Spec Count
		// specCount.UpdateSpectatorCount(r5apex, IClientEntityList);

		// Radar
		radar.UpdateRadar(r5apex, IClientEntityList);

		// Item ESP, Glow style for weapon and Loba style for consumables/ items
		itemESP.ApplyItemESP(r5apex, IClientEntityList);
	}

ON_EXIT:
	rx_close_handle(keyboardInput);
	rx_close_handle(mouseInput);
	rx_close_handle(r5apex);
}
