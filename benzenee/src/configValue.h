#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>

std::map<std::string, std::string> config;

namespace {
    float stofConfig(const std::string& key) {
        const std::string& valueStr = config[key];
        return std::stof(valueStr);

        //return std::stof(config[key]);
    }
        
    /*
    float stofConfig(const std::string& key, float defaultValue) {
        try {
            return std::stof(config[key]);
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid value for key: " << key << ". Using default value." << std::endl;
            return defaultValue;
        }
    }
    */

    int stoiConfig(const std::string& key) {
        return std::stoi(config[key]);
    }

    double stodConfig(const std::string& key) {
        return std::stod(config[key]);
    }

    std::chrono::milliseconds millisecondsConfig(const std::string& key) {
        return std::chrono::milliseconds(std::stoi(config[key]));
    }

    std::vector<float> parseRGBColor(const std::string& configKey) {
        std::vector<float> rgbComponents;

        if (config.find(configKey) != config.end()) {
            std::string rgbString = config[configKey];

            // Parse the RGB components from the config value
            std::istringstream iss(rgbString);
            float component;
            while (iss >> component) {
                rgbComponents.push_back(component);
            }
        }

        return rgbComponents;
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
const std::vector<float> CHAMS_RGB_COMPONENTS = parseRGBColor("CHAMS.CHAMS_RGB");
const float CHAMS_RED_VALUE = (CHAMS_RGB_COMPONENTS.size() == 3) ? CHAMS_RGB_COMPONENTS[0] : 61.0f;
const float CHAMS_GREEN_VALUE = (CHAMS_RGB_COMPONENTS.size() == 3) ? CHAMS_RGB_COMPONENTS[1] : 2.0f;
const float CHAMS_BLUE_VALUE = (CHAMS_RGB_COMPONENTS.size() == 3) ? CHAMS_RGB_COMPONENTS[2] : 2.0f;

// Glow RGB color for enemy
//const std::vector<float> GLOW_RGB_COMPONENTS = parseRGBColor("GLOW.ENEMY_RGB");
//const float GLOW_RED_VALUE = (GLOW_RGB_COMPONENTS.size() == 3) ? GLOW_RGB_COMPONENTS[0] : 0.0f;
//const float GLOW_GREEN_VALUE = (GLOW_RGB_COMPONENTS.size() == 3) ? GLOW_RGB_COMPONENTS[1] : 100.0f;
//const float GLOW_BLUE_VALUE = (GLOW_RGB_COMPONENTS.size() == 3) ? GLOW_RGB_COMPONENTS[2] : 0.0f;

// Glow color for visible enemies
const std::vector<float> ENEMY_VISIBLE_RGB_COMPONENTS = parseRGBColor("GLOW.ENEMY_VISIBLE");
const float VISIBLE_RED_VALUE = (ENEMY_VISIBLE_RGB_COMPONENTS.size() == 3) ? ENEMY_VISIBLE_RGB_COMPONENTS[0] : 0.0f;
const float VISIBLE_GREEN_VALUE = (ENEMY_VISIBLE_RGB_COMPONENTS.size() == 3) ? ENEMY_VISIBLE_RGB_COMPONENTS[1] : 60.0f;
const float VISIBLE_BLUE_VALUE = (ENEMY_VISIBLE_RGB_COMPONENTS.size() == 3) ? ENEMY_VISIBLE_RGB_COMPONENTS[2] : 0.0f;

// Glow color for invisible enemies
const std::vector<float> ENEMY_INVISIBLE_RGB_COMPONENTS = parseRGBColor("GLOW.ENEMY_inVISIBLE");
const float INVISIBLE_RED_VALUE = (ENEMY_INVISIBLE_RGB_COMPONENTS.size() == 3) ? ENEMY_INVISIBLE_RGB_COMPONENTS[0] : 60.0f;
const float INVISIBLE_GREEN_VALUE = (ENEMY_INVISIBLE_RGB_COMPONENTS.size() == 3) ? ENEMY_INVISIBLE_RGB_COMPONENTS[1] : 0.0f;
const float INVISIBLE_BLUE_VALUE = (ENEMY_INVISIBLE_RGB_COMPONENTS.size() == 3) ? ENEMY_INVISIBLE_RGB_COMPONENTS[2] : 0.0f;

// Glow color for squad enemies
//const std::vector<float> SQUAD_RGB_COMPONENTS = parseRGBColor("GLOW.SQUARD_RGB");
//const float SQUARD_RED_VALUE = (SQUAD_RGB_COMPONENTS.size() == 3) ? SQUAD_RGB_COMPONENTS[0] : 50.0f;
//const float SQUARD_GREEN_VALUE = (SQUAD_RGB_COMPONENTS.size() == 3) ? SQUAD_RGB_COMPONENTS[1] : 50.0f;
//const float SQUARD_BLUE_VALUE = (SQUAD_RGB_COMPONENTS.size() == 3) ? SQUAD_RGB_COMPONENTS[2] : 0.0f;
