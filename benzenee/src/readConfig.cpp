#include "readConfig.h"

//void readConfig(std::map<std::string, std::string>& config, std::vector<std::pair<std::string, std::map<std::string, bool>&>>& sections)
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
			newConfigFile << "BONE_LIST=2,3,5,8" << std::endl;
			newConfigFile << "MAXDISTANCE=160.0" << std::endl;
			newConfigFile << "SLEEP=15" << std::endl;

			//Aim FOV
			newConfigFile << "[AIMFOV]" << std::endl;
			newConfigFile << "AIMFOV_ADS=3.0" << std::endl;
			newConfigFile << "AIMFOV_HIPFIRE=8.5" << std::endl;
			newConfigFile << "AIMFOV_DEADZONE=0.10" << std::endl;

			//Aim Key
			newConfigFile << "[AIMKEY]" << std::endl;
			newConfigFile << "AIMKEY=107" << std::endl;
			newConfigFile << "AIMKEY2=108" << std::endl;

			//No Recoil
			newConfigFile << "[NORECOIL]" << std::endl;
			newConfigFile << "NORECOIL_ENABLED=0" << std::endl;
			newConfigFile << "NORECOIL_STRENGTH=85.0" << std::endl;

            newConfigFile << "NORECOIL2_ENABLED=0" << std::endl;
			newConfigFile << "PitchStrength=0.50" << std::endl;
			newConfigFile << "YawStrength=0.50" << std::endl;

			//Trigger Bot
			newConfigFile << "[TRIGGERBOT]" << std::endl;
			newConfigFile << "TRIGGER_ENABLED=1" << std::endl;
            //newConfigFile << "TRIGGER2_ENABLED=0" << std::endl;
			newConfigFile << "TRIGGER_KEY=111" << std::endl;
			//newConfigFile << "TRIGGER_RANGE=60.0" << std::endl;

			//GLOW
			newConfigFile << "[GLOW]" << std::endl;
            newConfigFile << "GLOW_ENABLED=1" << std::endl;
			newConfigFile << "GLOW_ENEMY=1" << std::endl;
			//newConfigFile << "ENEMY_RGB=0,0,100" << std::endl;
            newConfigFile << "ENEMY_VISIBLE=0,60,0" << std::endl;
			newConfigFile << "ENEMY_inVISIBLE=60,0,0" << std::endl;
			newConfigFile << "GLOW_HEALTH=0" << std::endl;
			//newConfigFile << "GLOW_SQUARD=1" << std::endl;
            //newConfigFile << "SQUARD_RGB=50,50,0" << std::endl;
			//newConfigFile << "GLOW_RANGE=200.0" << std::endl;
			
			//Chams
			newConfigFile << "[CHAMS]" << std::endl;
			newConfigFile << "WEAPON_CHAMS=1" << std::endl;
			newConfigFile << "ARMS_CHAMS=1" << std::endl;
			newConfigFile << "CHAMS_RGB=61,2,2" << std::endl;
			newConfigFile << "CHAM_BORDER=80" << std::endl;

			//ESP
			newConfigFile << "[ESP]" << std::endl;
			newConfigFile << "ITEM_ESP_ENABLED=1" << std::endl;
			//newConfigFile << "ITEM_ESP_TOGGLE=26" << std::endl;
			newConfigFile << "ITEM_ESP=1" << std::endl;
			newConfigFile << "LOBA_ESP=1" << std::endl;
			newConfigFile << "ITEM_ESP_IDS=80,90,28,105,134,150" << std::endl;
            newConfigFile << "LOBA_ESP_IDS=182,183,185,186,199,208,209" << std::endl;

			//MISC
			newConfigFile << "[MISC]" << std::endl;
            newConfigFile << "SPECTATOR_COUNT=1" << std::endl;
			newConfigFile << "MAP_RADAR=0" << std::endl;
			//newConfigFile << "FAKEDUCK=0" << std::endl;
			//newConfigFile << "FAKEDUCK_KEY=80" << std::endl;
			newConfigFile << "BHOP=1" << std::endl; //AUTO_GRAPPLE
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


/*
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
                if (equalPos != std::string::npos && !section.empty()) {
                    // Construct keys using section name for section-specific settings
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
*/

/*
void parseConfigFile(std::map<int, int>& itemEspMap, std::map<int, int>& lobaEspMap, std::map<int, int>& filteredItemEspMap, std::map<int, int>& filteredLobaEspMap) {
    std::ifstream configFile("config.ini");
    std::string line;
    std::string section;

    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == ';') {
            continue;  // Skip empty lines and comments
        }
        if (line[0] == '[' && line.back() == ']') {
            section = line.substr(1, line.size() - 2);
            continue;
        }
        std::istringstream iss(line);
        std::string itemName;
        int itemId, itemValue;
        char delimiter, closeBracket;
        if (iss >> itemName >> delimiter >> itemId >> closeBracket >> itemValue && delimiter == '(' && closeBracket == ')') {
            if (section == "ITEM_ESP") {
                itemEspMap[itemId] = itemValue;
                if (itemValue == 1) {
                    filteredItemEspMap[itemId] = itemValue;
                }
            } else if (section == "LOBA_ESP") {
                lobaEspMap[itemId] = itemValue;
                if (itemValue == 1) {
                    filteredLobaEspMap[itemId] = itemValue;
                }
            }
        }
    }
}
*/

// Function to split a string into components
void splitString(const std::string& input, char delimiter, std::vector<std::string>& output) {
    std::istringstream stream(input);
    std::string token;

    while (std::getline(stream, token, delimiter)) {
        output.push_back(token);
    }
}

bool IsInItemEspIds(int itemID, const std::vector<int>& itemEspIds) {
    return std::find(itemEspIds.begin(), itemEspIds.end(), itemID) != itemEspIds.end();
}

bool IsInLobaEspIds(int itemID, const std::vector<int>& lobaEspIds) {
    return std::find(lobaEspIds.begin(), lobaEspIds.end(), itemID) != lobaEspIds.end();
}


/*
void readConfigAndPopulateSectionMaps(std::map<std::string, std::string>& config,
                                     std::vector<std::pair<std::string, std::map<std::string, bool>&>>& sections) {
    for (const auto& sectionPair : sections) {
        const std::string& sectionName = sectionPair.first;
        std::map<std::string, bool>& sectionMap = sectionPair.second;

        for (const auto& entry : config) {
            if (entry.first.find(sectionName + ".") == 0) {
                std::string itemName = entry.first.substr(sectionName.length() + 1);
                bool shouldDisplay = entry.second == "1" || entry.second == "true";
                sectionMap[itemName] = shouldDisplay;
            }
        }
    }
}
*/

/*
void reloadConfigOnUpdate() {
    std::ifstream configFile("config.ini");
    if (configFile) {
        configFile.seekg(0, std::ios::end);
        size_t fileSize = configFile.tellg();
        configFile.seekg(0, std::ios::beg);

        static size_t previousSize = 0;
        if (fileSize != previousSize) {
            previousSize = fileSize;
            config.clear();
            readConfig();
            std::cout << "Config reloaded due to update." << std::endl;
        }
    }
}
*/

std::map<std::string, std::string> config;

namespace {
    float stofConfig(const std::string& key) {
        return std::stof(config[key]);
    }

    int stoiConfig(const std::string& key) {
        return std::stoi(config[key]);
    }

    double stodConfig(const std::string& key) {
        return std::stod(config[key]);
    }

    std::chrono::milliseconds millisecondsConfig(const std::string& key) {
        return std::chrono::milliseconds(std::stoi(config[key]));
    }

    std::vector<int> parseIntegerList(const std::string& key) {
        std::vector<int> values;
        std::istringstream stream(config[key]);
        std::string value;
        while (std::getline(stream, value, ',')) {
            values.push_back(std::stoi(value));
        }
        return values;
    }

    std::vector<std::string> parseRGBColor(const std::string& key) {
        std::vector<std::string> components;
        splitString(config[key], ',', components);
        return components;
    }
}

//AimBot
const float AIMSMOOTH = stofConfig("AIMBOT.AIMSMOOTH");
const int AIMBOT_ENABLED = stoiConfig("AIMBOT.AIMBOT_ENABLED");
const std::chrono::milliseconds AIMBOT_SLEEP = millisecondsConfig("AIMBOT.SLEEP");
const float AIMBOT_MAXDISTANCE = stofConfig("AIMBOT.MAXDISTANCE");

//Aim Key
const int AIMKEY = stoiConfig("AIMKEY.AIMKEY");
const int AIMKEY2 = stoiConfig("AIMKEY.AIMKEY2");

//Aim FOV
const float AIMFOV_ADS = stofConfig("AIMFOV.AIMFOV_ADS");
const float AIMFOV_HIPFIRE = stofConfig("AIMFOV.AIMFOV_HIPFIRE");
const float AIMFOV_DEADZONE = stofConfig("AIMFOV.AIMFOV_DEADZONE");

//No Recoil
const int NORECOIL_ENABLED = stoiConfig("NORECOIL.NORECOIL_ENABLED");
const int NORECOIL2_ENABLED = stoiConfig("NORECOIL.NORECOIL2_ENABLED");
const float NORECOIL_STRENGTH = stofConfig("NORECOIL.NORECOIL_STRENGTH");
const double NORECOIL_PitchStrength = stodConfig("NORECOIL.PitchStrength");
const double NORECOIL_YawStrength = stodConfig("NORECOIL.YawStrength");

//Trigger Bot
const int TRIGGER_ENABLED = stoiConfig("TRIGGERBOT.TRIGGER_ENABLED");
//const int TRIGGER2_ENABLED = stoiConfig("TRIGGERBOT.TRIGGER2_ENABLED");
const int TRIGGER_KEY = stoiConfig("TRIGGERBOT.TRIGGER_KEY");
//const float TRIGGER_RANGE = stoiConfig("TRIGGERBOT.TRIGGER_RANGE");

//Glow
const int GLOW_ENABLED = stoiConfig("GLOW.GLOW_ENABLED");
const int GLOW_ENEMY = stoiConfig("GLOW.GLOW_ENEMY");
//const int GLOW_SQUARD = stoiConfig("GLOW.GLOW_SQUARD");
const int GLOW_HEALTH = stoiConfig("GLOW.GLOW_HEALTH");
//const float GLOW_RANGE = stoiConfig("GLOW.GLOW_RANGE");

//Item ESP
const int ITEM_ESP_ENABLED = stoiConfig("ESP.ITEM_ESP_ENABLED");
const int ITEM_ESP = stoiConfig("ESP.ITEM_ESP");
const int LOBA_ESP = stoiConfig("ESP.LOBA_ESP");
//const int ITEM_ESP_TOGGLE = stoiConfig("ESP.ITEM_ESP_TOGGLE");

//Chams
const int WEAPON_CHAMS = stoiConfig("CHAMS.WEAPON_CHAMS");
const int ARMS_CHAMS = stoiConfig("CHAMS.ARMS_CHAMS");
const int8_t CHAM_BORDER = static_cast<int8_t>(stoiConfig("CHAMS.CHAM_BORDER"));

//Misc
const int MAP_RADAR = stoiConfig("MISC.MAP_RADAR");
const int TAPSTRAFE = stoiConfig("MISC.TAPSTRAFE");
const int AUTO_GRAPPLE = stoiConfig("MISC.AUTO_GRAPPLE");
const int SPEC_COUNT = stoiConfig("MISC.SPECTATOR_COUNT");
//const int FAKEDUCK = stoiConfig("MISC.FAKEDUCK");
//const int FAKEDUCK_KEY = stoiConfig("MISC.FAKEDUCK_KEY");

// Bone List
const std::vector<int> BONE_LIST = parseIntegerList("AIMBOT.BONE_LIST");

//Item ESP IDs
const std::vector<int> ITEM_ESP_IDS = parseIntegerList("ESP.ITEM_ESP_IDS");
const std::vector<int> LOBA_ESP_IDS = parseIntegerList("ESP.LOBA_ESP_IDS");

// cham RGB color
const std::vector<std::string> CHAMS_RGB_COMPONENTS = parseRGBColor("CHAMS.CHAMS_RGB");
const float CHAMS_RED_VALUE = (CHAMS_RGB_COMPONENTS.size() == 3) ? stofConfig(CHAMS_RGB_COMPONENTS[0]) : 61.0f;
const float CHAMS_GREEN_VALUE = (CHAMS_RGB_COMPONENTS.size() == 3) ? stofConfig(CHAMS_RGB_COMPONENTS[1]) : 2.0f;
const float CHAMS_BLUE_VALUE = (CHAMS_RGB_COMPONENTS.size() == 3) ? stofConfig(CHAMS_RGB_COMPONENTS[2]) : 2.0f;

// GLOW RGB
//const std::vector<std::string> GLOW_RGB_COMPONENTS = parseRGBColor("GLOW.ENEMY_RGB");
//const float GLOW_RED_VALUE = (GLOW_RGB_COMPONENTS.size() == 3) ? stofConfig(GLOW_RGB_COMPONENTS[0]) : 0.0f;
//const float GLOW_GREEN_VALUE = (GLOW_RGB_COMPONENTS.size() == 3) ? stofConfig(GLOW_RGB_COMPONENTS[1]) : 100.0f;
//const float GLOW_BLUE_VALUE = (GLOW_RGB_COMPONENTS.size() == 3) ? stofConfig(GLOW_RGB_COMPONENTS[2]) : 0.0f;


// Glow Color for visible enemies
const std::vector<std::string> ENEMY_VISIBLE_RGB_COMPONENTS = parseRGBColor("GLOW.ENEMY_VISIBLE");
const float VISIBLE_RED_VALUE = (ENEMY_VISIBLE_RGB_COMPONENTS.size() == 3) ? stofConfig(ENEMY_VISIBLE_RGB_COMPONENTS[0]) : 0.0f;
const float VISIBLE_GREEN_VALUE = (ENEMY_VISIBLE_RGB_COMPONENTS.size() == 3) ? stofConfig(ENEMY_VISIBLE_RGB_COMPONENTS[1]) : 60.0f;
const float VISIBLE_BLUE_VALUE = (ENEMY_VISIBLE_RGB_COMPONENTS.size() == 3) ? stofConfig(ENEMY_VISIBLE_RGB_COMPONENTS[2]) : 0.0f;

// Glow Color for inVisible enemies
const std::vector<std::string> ENEMY_INVISIBLE_RGB_COMPONENTS = parseRGBColor("GLOW.ENEMY_inVISIBLE");
const float INVISIBLE_RED_VALUE = (ENEMY_INVISIBLE_RGB_COMPONENTS.size() == 3) ? stofConfig(ENEMY_INVISIBLE_RGB_COMPONENTS[0]) : 60.0f;
const float INVISIBLE_GREEN_VALUE = (ENEMY_INVISIBLE_RGB_COMPONENTS.size() == 3) ? stofConfig(ENEMY_INVISIBLE_RGB_COMPONENTS[1]) : 0.0f;
const float INVISIBLE_BLUE_VALUE = (ENEMY_INVISIBLE_RGB_COMPONENTS.size() == 3) ? stofConfig(ENEMY_INVISIBLE_RGB_COMPONENTS[2]) : 0.0f;

// Glow Color for Squard enemies
//const std::vector<std::string> SQUARD_RGB_COMPONENTS = parseRGBColor("GLOW.SQUARD_RGB");
//const float SQUARD_RED_VALUE = (SQUARD_RGB_COMPONENTS.size() == 3) ? stofConfig(SQUARD_RGB_COMPONENTS[0]) : 50.0f;
//const float SQUARD_GREEN_VALUE = (SQUARD_RGB_COMPONENTS.size() == 3) ? stofConfig(SQUARD_RGB_COMPONENTS[1]) : 50.0f;
//const float SQUARD_BLUE_VALUE = (SQUARD_RGB_COMPONENTS.size() == 3) ? stofConfig(SQUARD_RGB_COMPONENTS[2]) : 0.0f;
