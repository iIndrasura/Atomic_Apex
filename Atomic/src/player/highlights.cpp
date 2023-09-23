#include "highlights.hpp"

/* 
void Highlights::SetEntityAddress(QWORD address)
{
    entityAddress = address;
}
*/

uint32_t Highlights::GetHighlightCurrentContext(rx_handle process, QWORD entityAddress)
{
    QWORD currentContextPtr = entityAddress + OFFSET_HIGHLIGHTCURRENTCONTEXTID;
    return rx_read_i32(process, currentContextPtr);
}

// Set Glow Enable and context ID
void Highlights::SetHighlightCurrentContext(rx_handle process, QWORD entityAddress, uint32_t CurrentContextId)
{
    QWORD currentContextPtr = entityAddress + OFFSET_HIGHLIGHTCURRENTCONTEXTID;
    rx_write_i32(process, currentContextPtr, CurrentContextId);
}

// Set Visibility Type {0 disable, 1 Force Enable, 2 Always Visibility}
void Highlights::SetHighlightVisibilityType(rx_handle process, QWORD entityAddress, uint32_t VisibilityType)
{
    QWORD visibilityTypePtr = entityAddress + OFFSET_HIGHLIGHTVISIBILITYTYPE;
    rx_write_i32(process, visibilityTypePtr, VisibilityType);
}

// Set Active State
void Highlights::SetHighlightActiveState(rx_handle process, QWORD entityAddress, uint32_t ContextId, uint32_t FunctionParameterId)
{
    QWORD ActiveStatePtr = entityAddress + OFFSET_HIGHLIGHTSERVERACTIVESTATES + ContextId;
    rx_write_i32(process, ActiveStatePtr, FunctionParameterId);
}

// To set highlightFunctionBits style, inside, outside, outline radius 
void Highlights::SetHighlightFunctions(rx_handle process, QWORD entityAddress, uint32_t settingIndex, uint8_t InsideFunction, uint8_t OutlineFunction, float OutlineRadius, bool EntityVisible, uint32_t State, bool AfterPostProcess)
{
    struct HighlightFunctionBits
    {
        uint8_t FunctionBits[4];
    };

    QWORD highlightSettings = rx_read_i64(process, OFFSETS::REGION + OFFSET_HIGHLIGHTSETTINGS);

    QWORD serverActiveStatesPtr = entityAddress + OFFSET_HIGHLIGHTSERVERACTIVESTATES;
    uint8_t HighlightServerActiveStates = rx_read_i32(process, serverActiveStatesPtr);
    if (HighlightServerActiveStates != 0xFF)
    {
        // QWORD functionBitsPtr = highlightSettings + 40 * HighlightServerActiveStates + 4;
        QWORD functionBitsPtr = highlightSettings + HIGHLIGHT_TYPE_SIZE * settingIndex + 4;         // settingIndex by default can put 40

        HighlightFunctionBits functionBits = { InsideFunction, OutlineFunction, static_cast<uint8_t>(((OutlineRadius * 255.0f) * 0.125f) + 0.5f), static_cast<uint8_t>((EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)) };
        rx_write_array(process, functionBitsPtr, (char*)&functionBits, sizeof(functionBits));
    }
}

// Set Server Active state
void Highlights::SetHighlightFunctionsWithActiveState(rx_handle process, QWORD entityAddress, uint32_t ContextId, uint8_t InsideFunction, uint8_t OutlineFunction, float OutlineRadius, bool EntityVisible, uint32_t State, bool AfterPostProcess)
{
	struct HighlightFunctionBits
	{
		uint8_t FunctionBits[4];
	};

	QWORD highlightSettings = rx_read_i64(process, OFFSETS::REGION + OFFSET_HIGHLIGHTSETTINGS);

	QWORD serverActiveStatesPtr = entityAddress + ContextId + OFFSET_HIGHLIGHTSERVERACTIVESTATES;
	uint8_t HighlightServerActiveStates = rx_read_i32(process, serverActiveStatesPtr);
	if (HighlightServerActiveStates != 0xFF)
	{
		QWORD functionBitsPtr = highlightSettings + 40 * HighlightServerActiveStates + 4;

		HighlightFunctionBits functionBits = { InsideFunction, OutlineFunction, static_cast<uint8_t>(((OutlineRadius * 255.0f) * 0.125f) + 0.5f), static_cast<uint8_t>((EntityVisible << 6) | State & 0x3F | (AfterPostProcess << 7)) };
		rx_write_array(process, functionBitsPtr, (char*)&functionBits, sizeof(functionBits));
	}
}


// To set colors
void Highlights::SetHighlightParameter(rx_handle process, QWORD entityAddress, uint32_t FunctionParameterId, float* Parameter)
{
    struct HighlightParameter
    {
        float Parameter[3];
    };

    QWORD highlightSettings = rx_read_i64(process, OFFSETS::REGION + OFFSET_HIGHLIGHTSETTINGS);

    QWORD serverActiveStatesPtr = entityAddress + OFFSET_HIGHLIGHTSERVERACTIVESTATES;
    uint8_t HighlightServerActiveStates = rx_read_i32(process, serverActiveStatesPtr);
    if (HighlightServerActiveStates != 0xFF)
    {
        // QWORD parameterPtr = (highlightSettings + 12 * settingIndex) + 8 * (5 * HighlightServerActiveStates) + 8;        // settingIndex by default can put
        QWORD parameterPtr = (highlightSettings + HIGHLIGHT_TYPE_SIZE * FunctionParameterId) + 8 /* * (5 * HighlightServerActiveStates) + 8 */;
        
        HighlightParameter parameter = { Parameter[0], Parameter[1], Parameter[2] };

        // rx_write_process(process, parameterPtr, &parameter, sizeof(parameter));
        rx_write_array(process, parameterPtr, (char*)&parameter, sizeof(parameter));

    }
}

// Distance 
void Highlights::SetHighlightDistance(rx_handle process, QWORD entityAddress, float Distance)
{
    QWORD HighlightdistancePtr = entityAddress + OFFSET_HIGHLIGHTDISTANCE;
    rx_write_float(process, HighlightdistancePtr, Distance);
}