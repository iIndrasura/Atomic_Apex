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

            uint32_t ContextID = 1;
            uint32_t FunctionParameterIndexId;

            // Apply ITEM_ESP settings only if ITEM_ESP is enabled and itemID is in ITEM_ESP_IDS
            if (ConfigValues::ITEM_ESP == 1 && (itemEspIds.find(itemID) != itemEspIds.end()))
            {
                FunctionParameterIndexId = 60;
                red = ConfigValues::ITEM_RED_VALUE;
                green = ConfigValues::ITEM_GREEN_VALUE;
                blue = ConfigValues::ITEM_BLUE_VALUE;

                highlightClass.SetHighlightCurrentContext(process, itemEntity, ContextID);
                highlightClass.SetHighlightVisibilityType(process, itemEntity, 2);

                highlightClass.SetHighlightActiveState(process, itemEntity, ContextID, FunctionParameterIndexId);
                highlightClass.SetHighlightFunctions(process, itemEntity, FunctionParameterIndexId, 137, 125, 1.5f, true, 0, false);
                
                // set color
                Color color = { red, green, blue };
                highlightClass.SetHighlightParameter(process, itemEntity, FunctionParameterIndexId, &color.r);

                highlightClass.SetHighlightDistance(process, itemEntity, 2624.68f);          // 50 meters divided by 0.01905
            }

            // Apply LOBA_ESP settings only if LOBA_ESP is enabled and itemID is in LOBA_ESP_IDS
            if (ConfigValues::LOBA_ESP == 1 /* && IsInLobaEspIds(itemID, ConfigValues::LOBA_ESP_IDS) */) 
            {
                ContextID = highlightClass.GetHighlightCurrentContext(process, itemEntity);
                highlightClass.SetHighlightFunctionsWithActiveState(process, itemEntity, ContextID, 137, 138, 3.0f, true, 0, false);
            }
            else if (ConfigValues::LOBA_ESP2 == 1 && (lobaEspIds.find(itemID) != lobaEspIds.end()))
            {   
                // Epic Items
                if (redItems.find(itemID) != redItems.end()) 
                {
                    FunctionParameterIndexId = 61;
                    red = 0.8f;
                    green = 0.0f;
                    blue = 0.0f;
                }
                // Gold Items
                else if (goldItems.find(itemID) != goldItems.end())
                {
                    FunctionParameterIndexId = 62;
                    red = 1.0f;
                    green = 0.843f;
                    blue = 0.0f;
                }
                // Purple Items
                else if (purpleItems.find(itemID) != purpleItems.end())
                {
                    FunctionParameterIndexId = 63;
                    red = 0.8f;
                    green = 0.0f;
                    blue = 0.8f;
                }
                // Blue Items
                else if (blueItems.find(itemID) != blueItems.end())
                {
                    FunctionParameterIndexId = 64;
                    red = 0.0f;
                    green = 0.50196f;
                    blue = 1.0f;
                }
                // Gray Items
                else if (grayItems.find(itemID) != grayItems.end())
                {
                    FunctionParameterIndexId = 65;
                    red = 0.50196f;
                    green = 0.50196f;
                    blue = 0.50196f;
                }
                
                highlightClass.SetHighlightCurrentContext(process, itemEntity, ContextID);
                highlightClass.SetHighlightVisibilityType(process, itemEntity, 2);

                highlightClass.SetHighlightActiveState(process, itemEntity, ContextID, FunctionParameterIndexId);
                highlightClass.SetHighlightFunctions(process, itemEntity, FunctionParameterIndexId, 137, 125, 1.5f, true, 0, false);
                
                // set color
                Color color = { red, green, blue };
                highlightClass.SetHighlightParameter(process, itemEntity, FunctionParameterIndexId, &color.r);

                highlightClass.SetHighlightDistance(process, itemEntity, 2624.68f);          // 50 meters divided by 0.01905
            }
        }
    }
}

