// configvalues.cpp
#include "configvalues.hpp"
#include "readConfig.hpp" // Make sure to include the necessary header file

float ConfigValues::AIMSMOOTH = 18.5f;
int ConfigValues::AIMBOT_ENABLED = 1;
int ConfigValues::AIMKEY = 107;
int ConfigValues::AIMKEY2 = 108;
float ConfigValues::AIMFOV_ADS = 3.0f;
float ConfigValues::AIMFOV_HIPFIRE = 8.5f;
float ConfigValues::AIMFOV_DEADZONE = 0.09f;
std::chrono::milliseconds ConfigValues::AIMBOT_SLEEP(10);
float ConfigValues::AIMBOT_MAXDISTANCE = 160.0f;

// NORECOIL Configuration
int ConfigValues::NORECOIL_ENABLED = 0;
float ConfigValues::NORECOIL_STRENGTH = 80.0f;

// Trigger bot
int ConfigValues::TRIGGER_ENABLED = 1;
int ConfigValues::TRIGGER_KEY = 111;
int ConfigValues::TB_IGNORE_KNOCKED = 1;

// GLOW
int ConfigValues::GLOW_ENABLED = 1;
int ConfigValues::GLOW_ENEMY = 1;
int ConfigValues::GLOW_HEALTH = 0;
float ConfigValues::GLOW_DISTANCE = 200.0f/0.01905f;

// ESP and ITEM ESP
int ConfigValues::ITEM_ESP_ENABLED = 1;
int ConfigValues::ITEM_ESP = 1;
int ConfigValues::LOBA_ESP = 1;

// Chams
int ConfigValues::CHAMS_ENABLED = 1;
int ConfigValues::WEAPON_CHAMS = 1;
int ConfigValues::ARMS_CHAMS = 0;
int8_t ConfigValues::CHAM_BORDER = 80;

// SkinChanger
int ConfigValues::SKIN_CHANGER = 1;
int ConfigValues::WeaponSkinID = 15;
int ConfigValues::PlayerSkinID = 15;

// Misc
//int ConfigValues::MAP_RADAR = 0;
int ConfigValues::AUTO_GRAPPLE = 1;
int ConfigValues::SPECTATOR_COUNT = 1;

// Bone List
std::vector<int> ConfigValues::BONE_LIST{0, 2, 3, 5, 8};

// Item ESP IDs
std::vector<int> ConfigValues::ITEM_ESP_IDS{80, 90, 28, 105, 134, 150};

// Loba ESP IDs
std::vector<int> ConfigValues::LOBA_ESP_IDS{182, 183, 185, 186, 199, 208, 209};

// Chams RGB values
float ConfigValues::CHAMS_RED_VALUE = 61.0f;
float ConfigValues::CHAMS_GREEN_VALUE = 2.0f;
float ConfigValues::CHAMS_BLUE_VALUE = 2.0f;

// Glow colors
float ConfigValues::VISIBLE_RED_VALUE = 0.0f;
float ConfigValues::VISIBLE_GREEN_VALUE = 60.0f;
float ConfigValues::VISIBLE_BLUE_VALUE = 0.0f;

float ConfigValues::INVISIBLE_RED_VALUE = 60.0f;
float ConfigValues::INVISIBLE_GREEN_VALUE = 0.0f;
float ConfigValues::INVISIBLE_BLUE_VALUE = 0.0f;

void InitConfigValues(std::map<std::string, std::string>& config) {
    // AIMBOT Configuration
    ConfigValues::AIMSMOOTH = std::stof(config["AIMBOT.AIMSMOOTH"]);
    ConfigValues::AIMBOT_ENABLED = std::stoi(config["AIMBOT.AIMBOT_ENABLED"]);
    ConfigValues::AIMBOT_SLEEP = std::chrono::milliseconds(std::stoi(config["AIMBOT.SLEEP"]));
    ConfigValues::AIMBOT_MAXDISTANCE = std::stof(config["AIMBOT.MAXDISTANCE"]);

    // AIM key
    ConfigValues::AIMKEY = std::stoi(config["AIMKEY.AIMKEY"]);
    ConfigValues::AIMKEY2 = std::stoi(config["AIMKEY.AIMKEY2"]);

    // Aim FOV
    ConfigValues::AIMFOV_ADS = std::stof(config["AIMFOV.AIMFOV_ADS"]);
    ConfigValues::AIMFOV_HIPFIRE = std::stof(config["AIMFOV.AIMFOV_HIPFIRE"]);
    ConfigValues::AIMFOV_DEADZONE = std::stof(config["AIMFOV.AIMFOV_DEADZONE"]);

    // NO Recoil
    ConfigValues::NORECOIL_ENABLED = std::stoi(config["NORECOIL.NORECOIL_ENABLED"]);
    ConfigValues::NORECOIL_STRENGTH = std::stof(config["NORECOIL.NORECOIL_STRENGTH"]);

    // Trigger bot
    ConfigValues::TRIGGER_ENABLED = std::stoi(config["TRIGGERBOT.TRIGGER_ENABLED"]);
    ConfigValues::TRIGGER_KEY = std::stoi(config["TRIGGERBOT.TRIGGER_KEY"]);
    ConfigValues::TRIGGER_ENABLED = std::stoi(config["TRIGGERBOT.TB_IGNORE_KNOCKED"]);

    // GLOW
    ConfigValues::GLOW_ENABLED = std::stoi(config["GLOW.GLOW_ENABLED"]);
    ConfigValues::GLOW_ENEMY = std::stoi(config["GLOW.GLOW_ENEMY"]);
    ConfigValues::GLOW_HEALTH = std::stoi(config["GLOW.GLOW_HEALTH"]);
    ConfigValues::GLOW_DISTANCE = (std::stof(config["GLOW.GLOW_DISTANCE"])) / 0.01905f;      //convert from meter to in game unit * 0.01905

    // ESP and ITEM ESP
    ConfigValues::ITEM_ESP_ENABLED = std::stoi(config["ESP.ITEM_ESP_ENABLED"]);
    ConfigValues::ITEM_ESP = std::stoi(config["ESP.ITEM_ESP"]);
    ConfigValues::LOBA_ESP = std::stoi(config["ESP.LOBA_ESP"]);

    // Chams
    ConfigValues::CHAMS_ENABLED = std::stoi(config["CHAMS.CHAMS_ENABLED"]);
    ConfigValues::WEAPON_CHAMS = std::stoi(config["CHAMS.WEAPON_CHAMS"]);
    ConfigValues::ARMS_CHAMS = std::stoi(config["CHAMS.ARMS_CHAMS"]);
    ConfigValues::CHAM_BORDER = std::stoi(config["CHAMS.CHAM_BORDER"]);

    // SkinChanger
    ConfigValues::SKIN_CHANGER = std::stoi(config["SKIN_CHANGER.SKIN_CHANGER_ENABLED"]);
    ConfigValues::WeaponSkinID = std::stoi(config["SKIN_CHANGER.WeaponSkinID"]);
    ConfigValues::PlayerSkinID = std::stoi(config["SKIN_CHANGER.PlayerSkinID"]);

    // Misc
    //ConfigValues::MAP_RADAR = std::stoi(config["MISC.MAP_RADAR"]);
    ConfigValues::AUTO_GRAPPLE = std::stoi(config["MISC.AUTO_GRAPPLE"]);
    ConfigValues::SPECTATOR_COUNT = std::stoi(config["MISC.SPECTATOR_COUNT"]);

    // Bone List ---------------------------------------------
    std::vector<int> boneList;
    std::istringstream boneStream(config["AIMBOT.BONE_LIST"]);
    std::string bone;
    while (std::getline(boneStream, bone, ',')) {
        boneList.push_back(std::stoi(bone));
    }

    // Convert boneList to an array if needed
    ConfigValues::BONE_LIST = boneList;

    // Item ESP IDs ---------------------------------------------
    std::vector<int> itemEspIds;
    std::istringstream itemEspStream(config["ESP.ITEM_ESP_IDS"]);
    std::string itemId;
    while (std::getline(itemEspStream, itemId, ',')) {
        itemEspIds.push_back(std::stoi(itemId));
    }

    ConfigValues::ITEM_ESP_IDS = itemEspIds;

    // Loba ESP IDs ---------------------------------------------
    std::vector<int> lobaEspIds;
    std::istringstream lobaEspStream(config["ESP.LOBA_ESP_IDS"]);
    std::string lobaId;
    while (std::getline(lobaEspStream, lobaId, ',')) {
        lobaEspIds.push_back(std::stoi(lobaId));
    }

    ConfigValues::LOBA_ESP_IDS = lobaEspIds;

    // cham RGB color ---------------------------------------------
    std::string chamRgbValue = config["CHAMS.CHAMS_RGB"];
    std::vector<std::string> chamRgbComponents;
    splitString(chamRgbValue, ',', chamRgbComponents);

    // ConfigValues::CHAMS_RED_VALUE = 61.0f;   // Default values
    // ConfigValues::CHAMS_GREEN_VALUE = 2.0f;
    // ConfigValues::CHAMS_BLUE_VALUE = 2.0f;

    if (chamRgbComponents.size() == 3) {
        ConfigValues::CHAMS_RED_VALUE = std::stof(chamRgbComponents[0]);
        ConfigValues::CHAMS_GREEN_VALUE = std::stof(chamRgbComponents[1]);
        ConfigValues::CHAMS_BLUE_VALUE = std::stof(chamRgbComponents[2]);
    } else {
        std::cout << "Invalid CHAM_RGB value in config." << std::endl;
        // Default values are already set
    }

    // Glow Color for visible enemies
    std::string visibleEnemyRgbValue = config["GLOW.ENEMY_VISIBLE"];
    std::vector<std::string> visibleRgbComponents;
    splitString(visibleEnemyRgbValue, ',', visibleRgbComponents);

    // ConfigValues::VISIBLE_RED_VALUE;    // Default values
    // ConfigValues::VISIBLE_GREEN_VALUE = 60.0f;
    // ConfigValues::VISIBLE_BLUE_VALUE;

    if (visibleRgbComponents.size() == 3) {
        ConfigValues::VISIBLE_RED_VALUE = std::stof(visibleRgbComponents[0]);
        ConfigValues::VISIBLE_GREEN_VALUE = std::stof(visibleRgbComponents[1]);
        ConfigValues::VISIBLE_BLUE_VALUE = std::stof(visibleRgbComponents[2]);
    } else {
        std::cout << "Invalid Visible color value in config." << std::endl;
        // Default values are already set
    }

    // Glow Color for inVisible enemies
    std::string invisibleEnemyRgbValue = config["GLOW.ENEMY_inVISIBLE"];
    std::vector<std::string> invisibleRgbComponents;
    splitString(invisibleEnemyRgbValue, ',', invisibleRgbComponents);

    // ConfigValues::INVISIBLE_RED_VALUE = 60.0f;    // Default values
    // ConfigValues::INVISIBLE_GREEN_VALUE;
    // ConfigValues::INVISIBLE_BLUE_VALUE;

    if (invisibleRgbComponents.size() == 3) {
        ConfigValues::INVISIBLE_RED_VALUE = std::stof(invisibleRgbComponents[0]);
        ConfigValues::INVISIBLE_GREEN_VALUE = std::stof(invisibleRgbComponents[1]);
        ConfigValues::INVISIBLE_BLUE_VALUE = std::stof(invisibleRgbComponents[2]);
    } else {
        std::cout << "Invalid inVisible color value in config." << std::endl;
        // Default values are already set
    }
}