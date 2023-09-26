#include "readConfig.hpp"

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

// Function to split a string into components
// bool IsInItemEspIds(int itemID, const std::vector<int>& itemEspIds) {
//     return std::find(itemEspIds.begin(), itemEspIds.end(), itemID) != itemEspIds.end();
// }

bool IsInItemEspIds(int itemID, const std::unordered_set<int>& itemEspIds) {
    return itemEspIds.find(itemID) != itemEspIds.end();
}

// using Vector to store data for loba
// bool IsInLobaEspIds(int itemID, const std::vector<int>& lobaEspIds) {
//     return std::find(lobaEspIds.begin(), lobaEspIds.end(), itemID) != lobaEspIds.end();
// }

// using unordered_set to store data for loba
bool IsInLobaEspIds(int itemID, const std::unordered_set<int>& lobaEspIds) {
    return lobaEspIds.find(itemID) != lobaEspIds.end();
}

void splitString(const std::string& input, char delimiter, std::vector<std::string>& output) {
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        output.push_back(token);
    }
}

void readConfig(std::map<std::string, std::string>& config) 
 {
    const std::string configFile = "config.ini";
    std::ifstream file(configFile);

    if (!file) {
        std::cout << "Config file not found. Creating a new one with default values." << std::endl;

        // Create a new config file with default values
        std::ofstream newConfigFile(configFile);
		if (newConfigFile) 
		{
			//AimBot
			newConfigFile << "[AIMBOT]" << std::endl;
			newConfigFile << "AIMBOT_ENABLED=1" << std::endl;
			newConfigFile << "AIMSMOOTH=18.5" << std::endl;
			newConfigFile << "BONE_LIST=0,2,3,5,8" << std::endl;
			newConfigFile << "MAXDISTANCE=160.0" << std::endl;
			newConfigFile << "SLEEP=15" << std::endl;

			//Aim FOV
			newConfigFile << "[AIMFOV]" << std::endl;
			newConfigFile << "AIMFOV_ADS_MAX=5.0" << std::endl;
			newConfigFile << "AIMFOV_ADS_MIN=2.5" << std::endl;
			newConfigFile << "AIMFOV_HIPFIRE=8.5" << std::endl;
			newConfigFile << "AIMFOV_DEADZONE=0.09" << std::endl;

			//Aim Key
			newConfigFile << "[AIMKEY]" << std::endl;
			newConfigFile << "AIMKEY=107" << std::endl;
			newConfigFile << "AIMKEY2=108" << std::endl;

			//No Recoil
			newConfigFile << "[NORECOIL]" << std::endl;
			newConfigFile << "NORECOIL_ENABLED=0" << std::endl;
			// newConfigFile << "NORECOIL_STRENGTH=85.0" << std::endl;

            // newConfigFile << "NORECOIL2_ENABLED=0" << std::endl;
			// newConfigFile << "NORECOIL3_ENABLED=0" << std::endl;

			newConfigFile << "PitchStrength=50.0" << std::endl;
			newConfigFile << "YawStrength=80.0" << std::endl;

			//Trigger Bot
			newConfigFile << "[TRIGGERBOT]" << std::endl;
			newConfigFile << "TRIGGER_ENABLED=1" << std::endl;
            //newConfigFile << "TRIGGER2_ENABLED=0" << std::endl;
			newConfigFile << "TRIGGER_KEY=111" << std::endl;
			//newConfigFile << "TRIGGER_RANGE=60.0" << std::endl; 
			newConfigFile << "TB_IGNORE_KNOCKED=1" << std::endl;

			//GLOW
			newConfigFile << "[GLOW]" << std::endl;
            newConfigFile << "GLOW_ENABLED=1" << std::endl;
			//newConfigFile << "GLOW_ENABLED2=1" << std::endl;
			newConfigFile << "GLOW_ENEMY=1" << std::endl;
			//newConfigFile << "ENEMY_RGB=0,0,100" << std::endl;
            newConfigFile << "ENEMY_VISIBLE=0,255,0" << std::endl;
			newConfigFile << "ENEMY_inVISIBLE=255,0,0" << std::endl;
			newConfigFile << "GLOW_HEALTH=0" << std::endl;
			//newConfigFile << "GLOW_SQUARD=1" << std::endl;
            //newConfigFile << "SQUARD_RGB=50,50,0" << std::endl;
			newConfigFile << "KNOCKED_RGB=255,255,255" << std::endl;
			newConfigFile << "GLOW_DISTANCE=200.0" << std::endl;
			
			//Chams
			newConfigFile << "[CHAMS]" << std::endl;
			newConfigFile << "CHAMS_ENABLED=0" << std::endl;
			newConfigFile << "WEAPON_CHAMS=0" << std::endl;
			newConfigFile << "ARMS_CHAMS=0" << std::endl;
			newConfigFile << "CHAMS_RGB=61,2,2" << std::endl;
			newConfigFile << "CHAM_BORDER=80" << std::endl;

			//ESP
			newConfigFile << "[ESP]" << std::endl;
			newConfigFile << "ITEM_ESP_ENABLED=1" << std::endl;
			//newConfigFile << "ITEM_ESP_TOGGLE=26" << std::endl;
			newConfigFile << "ITEM_ESP=1" << std::endl;
			newConfigFile << "ITEM_ESP_RGB=255,215,0" << std::endl;
			newConfigFile << "ITEM_ESP_IDS=80,90,28,105,134,150" << std::endl;
			newConfigFile << "LOBA_ESP=0" << std::endl;
			newConfigFile << "LOBA_ESP2=1" << std::endl;
            newConfigFile << "LOBA_ESP_IDS=182,183,185,186,199,208,209" << std::endl;

            //SkinChanger
            newConfigFile << "[SKIN_CHANGER]" << std::endl;
            newConfigFile << "SKIN_CHANGER_ENABLED=0" << std::endl;
            newConfigFile << "WeaponSkinID=15" << std::endl;
            newConfigFile << "PlayerSkinID=16" << std::endl;

			//MISC
			newConfigFile << "[MISC]" << std::endl;
			newConfigFile << "RADAR=1" << std::endl;
            newConfigFile << "SPECTATOR_COUNT=1" << std::endl;
			//newConfigFile << "FAKEDUCK=0" << std::endl;
			//newConfigFile << "FAKEDUCK_KEY=80" << std::endl;
			//newConfigFile << "TAPSTRAFE=1" << std::endl; //AUTO_GRAPPLE
            newConfigFile << "AUTO_GRAPPLE=1" << std::endl;
		} else {
			std::cout << "Error creating config file." << std::endl;
		}
    }

    // Read the config file and populate the config map
    file.close();
    file.open(configFile);
	if (file) {
        std::string line;
        std::string section;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == ';') {
                continue; // Ignore empty lines and comments
            } else if (line[0] == '[' && line.back() == ']') {
                section = line.substr(1, line.size() - 2);
            } else {
                size_t equalPos = line.find('=');
                if (equalPos != std::string::npos) {
                    std::string key = section + "." + line.substr(0, equalPos);
                    std::string value = line.substr(equalPos + 1);
                    config[key] = value;
                }
            }
        }
    } else {
        std::cout << "Error reading config file." << std::endl;
    }
}