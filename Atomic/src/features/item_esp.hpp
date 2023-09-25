#ifndef ITEM_ESP_HPP
#define ITEM_ESP_HPP

#include <iostream>
#include <cstdio>
#include <unordered_set>

#include "../core/handling.hpp"
#include "../config/configvalues.hpp"
#include "../config/readConfig.hpp"

#include "../offsets/offsets.cpp"

#include "../player/player.hpp"
#include "../player/level.hpp"
#include "../player/highlights.hpp"

class ItemESP {
private:
    Level levelClass;
    Player playerClass;
    Highlights highlightClass;
    
    int itemDelayCounter; // Counter for the delay before ItemESP logic execution
    int itemID;

    // uint32_t ContextID;
    // uint32_t FunctionParameterIndexId;

    /* 
    typedef struct
    {
        int a, b, c, d;
    } SettingIndex;
    */

    float red, green, blue;

    std::unordered_set<int> itemEspIds = ConfigValues::ITEM_ESP_IDS;
    std::unordered_set<int> lobaEspIds = ConfigValues::LOBA_ESP_IDS;

    std::unordered_set<int> redItems = {
        25    // Treasure Pack 
    };

    std::unordered_set<int> goldItems = {
        190,  // Helmet Level 4
        195,  // Body Armor Level 4
        200,  // Evo Shield 4
        205,  // Knockdown Shield Level 4
        209,  // Knockdown Shield Level 4
        218,  // 1x Digital Threat
        223,  // 4x-10x Digital Sniper Threat
        234,  // Light Magazine Level 4
        238,  // Heavy Magazine Level 4
        242,  // Energy Magazine Level 4
        246,  // Sniper Magazine Level 4
        250   // Shotgun Bolt Level 4
        // 1,   // Turbocharger
        // 5,   // Hammerpoint Rounds
        // 4,   // Disruptor Rounds
        // 15   // Boost Loader
    };

    std::unordered_set<int> purpleItems = {
        182,  // Phoenix Kit
        189,  // Helmet Level 3
        194,  // Body Armor Level 3
        199,  // Evo Shield 3
        204,  // Knockdown Shield Level 3
        208,  // Backpack Level 3
        219,  // 3x HCOG (Ranger)
        220,  // 2x-4x Variable AOG
        221,  // 6x Sniper
        222,  // 4x-8x Variable Sniper
        226,  // Barrel Stabilizer Level 3
        230,  // Laser Sight Level 3
        233,  // Light Magazine Level 3
        237,  // Heavy Magazine Level 3
        241,  // Energy Magazine Level 3
        245,  // Sniper Magazine Level 3
        249,  // Shotgun Bolt Level 3
        253,  // Standard Stock Level 3
        256   // Sniper Stock Level 3
        // You can add additional items here if needed
    };

    std::unordered_set<int> blueItems = {
        181,    // Ultimate Accelerant
        183,    // Med Kit
        185,    // Shield Battery
        188,    // Helmet Level 2
        193,    // Body Armor Level 2
        198,    // Evo Shield 2
        203,    // Knockdown Shield Level 2
        207,    // Backpack Level 2
        215,    // 2x HCOG (Bruiser)
        217,    // 1x-2x Variable Holo
        225,    // Barrel Stabilizer Level 2
        229,    // Laser Sight Level 2
        232,    // Light Magazine Level 2
        236,    // Heavy Magazine Level 2
        240,    // Energy Magazine Level 2
        244,    // Sniper Magazine Level 2
        248,    // Shotgun Bolt Level 2
        252,    // Standard Stock Level 2
        255,    // Sniper Stock Level 2
        22,     // Heat Shield
        //23,     // Respawn Beacon
        26      // Evac Tower
    };

    std::unordered_set<int> grayItems = {
        139,    // Light Rounds
        140,    // Energy Ammo
        141,    // Shotgun Shells
        142,    // Heavy Rounds
        143,    // Sniper Rounds
        184,    // Syringe
        186,    // Shield Cell
        187,    // Helmet Level 1
        192,    // Body Armor Level 1
        197,    // Evo Shield 1
        202,    // Knockdown Shield Level 1
        206,    // Backpack Level 1
        211,    // Thermite Grenade
        212,    // Frag Grenade
        213,    // Arc Star
        214,    // 1x HCOG (Classic)
        216,    // 1x Holo
        224,    // Barrel Stabilizer Level 1
        228,    // Laser Sight Level 1
        231,    // Light Magazine Level 1
        235,    // Heavy Magazine Level 1
        239,    // Energy Magazine Level 1
        243,    // Sniper Magazine Level 1
        247,    // Shotgun Bolt Level 1
        251,    // Standard Stock Level 1
        254     // Sniper Stock Level 1
    };

public:
    ItemESP();
    ~ItemESP();

    // Function to determine whether the ItemESP feature should be enabled
    bool shouldItemESPEnable(rx_handle process);

    // Function to apply the ItemESP logic
    void ApplyItemESP(rx_handle process, QWORD ClientEntityList);
};

#endif // ITEM_ESP_HPP