#ifndef CONFIGVALUES_H
#define CONFIGVALUES_H

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>

class ConfigValues {
public:
    // AIMBOT Configuration
    static float AIMSMOOTH;
    static int AIMBOT_ENABLED;
    static int AIMKEY;
    static int AIMKEY2;
    static float AIMFOV_ADS;
    static float AIMFOV_HIPFIRE;
    static float AIMFOV_DEADZONE;
    static std::chrono::milliseconds AIMBOT_SLEEP;
    static float AIMBOT_MAXDISTANCE;

    // NORECOIL Configuration
    static int NORECOIL_ENABLED;
    static float NORECOIL_STRENGTH;
    //static double norecoilPitchStrength;
    //static double norecoilYawStrength;

    // TRIGGERBOT Configuration
    static int TRIGGER_ENABLED;
    static int TRIGGER_KEY;
    //static float Trigger_distance;

    // GLOW Configuration
    static int GLOW_ENABLED;
    static int GLOW_ENEMY;
    //static int GLOW_SQUARD;
    static int GLOW_HEALTH;
    static float GLOW_DISTANCE;

    // ESP and ITEM ESP Configuration
    static int ITEM_ESP_ENABLED;
    static int ITEM_ESP;
    static int LOBA_ESP;
    //static int itemESP_toggle_key;

    // Chams Configuration
    static int WEAPON_CHAMS;
    static int ARMS_CHAMS;
    static int8_t CHAM_BORDER;

    // SkinChanger Configuration
    static int SKIN_CHANGER;
    static int WeaponSkinID;
    static int PlayerSkinID;

    // Misc Configuration
    //static int FAKEDUCK;
    //static int FAKEDUCK_KEY;
    static int MAP_RADAR;
    //static int TAPSTRAFE;
    static int AUTO_GRAPPLE;
    static int SPECTATOR_COUNT;

    // Bone List Configuration
    static std::vector<int> BONE_LIST;

    // Item ESP IDs Configuration
    static std::vector<int> ITEM_ESP_IDS;

    // Loba ESP IDs Configuration
    static std::vector<int> LOBA_ESP_IDS;

    // Chams RGB Color Configuration
    static float CHAMS_RED_VALUE;
    static float CHAMS_GREEN_VALUE;
    static float CHAMS_BLUE_VALUE;

    // GLOW RGB Color Configuration
    //static float glow_redValue;
    //static float glow_greenValue;
    //static float glow_blueValue;

    // Visible Enemy RGB Color Configuration
    static float VISIBLE_RED_VALUE;
    static float VISIBLE_GREEN_VALUE;
    static float VISIBLE_BLUE_VALUE;

    // Invisible Enemy RGB Color Configuration
    static float INVISIBLE_RED_VALUE;
    static float INVISIBLE_GREEN_VALUE;
    static float INVISIBLE_BLUE_VALUE;

    // Squard RGB Color Configuration
    //static float squard_redValue;
    //static float squard_greenValue;
    //static float squard_blueValue;
};

void InitConfigValues(std::map<std::string, std::string>& config);

#endif // CONFIGVALUES_H
