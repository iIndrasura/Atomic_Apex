#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>

struct RGBValues {
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
};

void readConfig(std::map<std::string, std::string>& config);
//void readConfig(std::map<std::string, std::string>& config, std::vector<std::pair<std::string, std::map<std::string, bool>&>>& sections)

//GLOW Color split
void splitString(const std::string& input, char delimiter, std::vector<std::string>& output);

// Item ESP
bool IsInItemEspIds(int itemID, const std::vector<int>& itemEspIds);
bool IsInLobaEspIds(int itemID, const std::vector<int>& lobaEspIds);

//void parseConfigFile(std::map<int, int>& itemEspMap, std::map<int, int>& lobaEspMap, std::map<int, int>& filteredItemEspMap, std::map<int, int>& filteredLobaEspMap) {

//void reloadConfigOnUpdate()
/**/

/*
extern const float AIMSMOOTH;
extern const int AIMBOT_ENABLED;
extern const std::chrono::milliseconds AIMBOT_SLEEP;
extern const float AIMBOT_MAXDISTANCE;

extern const int AIMKEY;
extern const int AIMKEY2;

extern const float AIMFOV_ADS;
extern const float AIMFOV_HIPFIRE;
extern const float AIMFOV_DEADZONE;

extern const int NORECOIL_ENABLED;
extern const int NORECOIL2_ENABLED;
extern const float NORECOIL_STRENGTH;
extern const double NORECOIL_PitchStrength;
extern const double NORECOIL_YawStrength;

extern const int TRIGGER_ENABLED;
//extern const int TRIGGER2_ENABLED
extern const int TRIGGER_KEY;
//extern const float TRIGGER_RANGE

extern const int GLOW_ENABLED;
extern const int GLOW_ENEMY;
//extern const int GLOW_SQUARD
extern const int GLOW_HEALTH;
//extern const int GLOW_RANGE

extern const int ITEM_ESP_ENABLED;
extern const int ITEM_ESP;
extern const int LOBA_ESP;
//extern const int ITEM_ESP_TOGGLE

extern const int WEAPON_CHAMS;
extern const int ARMS_CHAMS;
extern const int8_t CHAM_BORDER;

extern const int MAP_RADAR;
extern const int TAPSTRAFE;
extern const int AUTO_GRAPPLE;
extern const int SPEC_COUNT;
//extern const int FAKEDUCK
//extern const int FAKEDUCK_KEY

extern const std::vector<int> BONE_LIST;

extern const std::vector<int> ITEM_ESP_IDS;
extern const std::vector<int> LOBA_ESP_IDS;

extern const float CHAMS_RED_VALUE;
extern const float CHAMS_GREEN_VALUE;
extern const float CHAMS_BLUE_VALUE;

//extern const float GLOW_RED_VALUE;
//extern const float GLOW_GREEN_VALUE;
//extern const float GLOW_BLUE_VALUE;

extern const float VISIBLE_RED_VALUE;
extern const float VISIBLE_GREEN_VALUE;
extern const float VISIBLE_BLUE_VALUE;

extern const float INVISIBLE_RED_VALUE;
extern const float INVISIBLE_GREEN_VALUE;
extern const float INVISIBLE_BLUE_VALUE;

//extern const float SQUARD_RED_VALUE;
//extern const float SQUARD_GREEN_VALUE;
//extern const float SQUARD_BLUE_VALUE;
*/