#ifndef ITEM_ESP_HPP
#define ITEM_ESP_HPP

#include <iostream>
#include <cstdio>

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

    //uint32_t ContextID;
    //uint32_t FunctionParameterIndexId;

    typedef struct
    {
        int a, b, c, d;
    } SettingIndex;
    
public:
    ItemESP();
    ~ItemESP();

    // Function to determine whether the ItemESP feature should be enabled
    bool shouldItemESPEnable(rx_handle process);

    // Function to apply the ItemESP logic
    void ApplyItemESP(rx_handle process, QWORD ClientEntityList);
};

#endif // ITEM_ESP_HPP