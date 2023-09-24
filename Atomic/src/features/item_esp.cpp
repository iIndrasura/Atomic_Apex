#include "item_esp.hpp"

ItemESP::ItemESP() : itemDelayCounter(0), itemID(0){
    // Constructor implementation (if needed)
}

ItemESP::~ItemESP() {
    // Destructor implementation
}

bool ItemESP::shouldItemESPEnable(rx_handle process) {
    // Check if the ItemESP feature should be enabled based on game state and configuration
    if (ConfigValues::ITEM_ESP_ENABLED == 1 && (levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process))) {
        return true;
    }
    return false;
}

/*
bool ItemESP::shouldItemESPEnable(rx_handle process) {
    if (ConfigValues::ITEM_ESP_ENABLED != 1)
        return false;

    if (!(levelClass.isTrainingArea(process) || levelClass.isPlayable(process) || levelClass.isSpecialMode(process)))
        return false;

    return true;
}
*/

void ItemESP::ApplyItemESP(rx_handle process, QWORD ClientEntityList) 
{
    // Increment the delay counter with each loop iteration
    itemDelayCounter++;

    if(itemDelayCounter > 5001)
	{
		itemDelayCounter = 0;
	}

    // Check if the delay counter has reached 5000
    if (itemDelayCounter == 5000 && shouldItemESPEnable(process)) 
    {
        //printf("GLOW -- %i",itemDelayCounter);
        
        for (int k = 0; k < 10000; k++)
        {
            QWORD itemEntity = playerClass.GetClientEntity(process, ClientEntityList, k);
            itemID = rx_read_int(process, itemEntity + OFFSETS::m_itemId);

            // Apply ITEM_ESP settings only if ITEM_ESP is enabled and itemID is in ITEM_ESP_IDS
            if (ConfigValues::ITEM_ESP == 1 && IsInItemEspIds(itemID, ConfigValues::ITEM_ESP_IDS)) 
            {
                // rx_write_i32(process, itemEntity + 0x3F8, 1);
                // rx_write_i32(process, itemEntity + 0x400, 2);
                // rx_write_i32(process, itemEntity + 0x2F4, 1512990053);

                // rx_write_float(process, itemEntity + 0x200, 61.f);
                // rx_write_float(process, itemEntity + 0x204, 2.f);
                // rx_write_float(process, itemEntity + 0x208, 2.f);

                uint32_t ContextID = highlightClass.GetHighlightCurrentContext(process, itemEntity);         // 1
                uint32_t FunctionParameterIndexId = 64;
                highlightClass.SetHighlightCurrentContext(process, itemEntity, ContextID);
                highlightClass.SetHighlightVisibilityType(process, itemEntity, 2);
                
                highlightClass.SetHighlightActiveState(process, itemEntity, ContextID, FunctionParameterIndexId);
                highlightClass.SetHighlightFunctions(process, itemEntity, FunctionParameterIndexId, 137, 138, 1.0f, true, 0, false);
                
                // set color    { 1.0f, 0.0f, 0.0f }
                Color color = { 
                    ConfigValues::ITEM_RED_VALUE, 
                    ConfigValues::ITEM_GREEN_VALUE, 
                    ConfigValues::ITEM_BLUE_VALUE 
                };
                highlightClass.SetHighlightParameter(process, itemEntity, FunctionParameterIndexId, &color.r);

                //highlightClass.SetHighlightDistance(process, itemEntity, 3149.6f);  // 60 meters divided by 0.01905 distance factor
            }

            // Apply LOBA_ESP settings only if LOBA_ESP is enabled and itemID is in LOBA_ESP_IDS
            if (ConfigValues::LOBA_ESP == 1 && IsInLobaEspIds(itemID, ConfigValues::LOBA_ESP_IDS)) 
            {
                //rx_write_i32(process, itemEntity + 0x2F0, 1363184265);      // highlightFunctionBits item glow offset
                
                //uint32_t FunctionParameterIndexId = 64;
                uint32_t ContextID = highlightClass.GetHighlightCurrentContext(process, itemEntity);
                //highlightClass.SetHighlightCurrentContext(process, itemEntity, ContextID);
                //highlightClass.SetHighlightVisibilityType(process, itemEntity, 2);

                //highlightClass.SetHighlightActiveState(process, itemEntity, ContextID, FunctionParameterIndexId);
                highlightClass.SetHighlightFunctionsWithActiveState(process, itemEntity, ContextID, 137, 138, 3.0f, true, 0, false);

                //highlightClass.SetHighlightDistance(process, itemEntity, 3149.6f);  // 60 meters divided by 0.01905 distance factor
            }
        }
    }
}


// Those are 2 default values for glow. 1363184265 is the default value for red items with glow when you are playing Loba. 1411417991 is the default value for white items with no glow.
// For reference:
// White default: 1411417991
// Blue default: 1428195207
// Purple default: 1444972423 // 1396738697 glow
// Gold default: 1461749639 // 1379961481 glow
// Red default: 1478526855 // 1363184265 glow
// Weapons default: 1358917120

// I tried changing them to non-default values and it seems that they represent the thickness of the glow around the item.
// I scan through 10.000 entities and I check if an itemEntity is an item by looking at glow values.

