#include "item_esp.hpp"

ItemESP::ItemESP() : itemDelayCounter(0), itemID(0) {
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
            QWORD entity = playerClass.GetClientEntity(process, ClientEntityList, k);
            itemID = rx_read_int(process, entity + OFFSETS::m_itemId);

            // Apply ITEM_ESP settings only if ITEM_ESP is enabled and itemID is in ITEM_ESP_IDS
            if (ConfigValues::ITEM_ESP == 1 && IsInItemEspIds(itemID, ConfigValues::ITEM_ESP_IDS)) 
            {
                rx_write_i32(process, entity + 0x3F8, 1);
                rx_write_i32(process, entity + 0x400, 2);
                rx_write_i32(process, entity + 0x2F4, 1512990053);

                rx_write_float(process, entity + 0x200, 61.f);
                rx_write_float(process, entity + 0x204, 2.f);
                rx_write_float(process, entity + 0x208, 2.f);
            }

            // Apply LOBA_ESP settings only if LOBA_ESP is enabled and itemID is in LOBA_ESP_IDS
            if (ConfigValues::LOBA_ESP == 1 && IsInLobaEspIds(itemID, ConfigValues::LOBA_ESP_IDS)) 
            {
                //rx_write_i32(r5apex, entity + 0x292, 16256);
				//rx_write_i32(r5apex, entity + 0x30c, 1193322764);
                rx_write_i32(process, entity + 0x2F0, 1363184265);
            }
        }
    }
}

