#ifndef HIGHLIGHTS_HPP
#define HIGHLIGHTS_HPP

#include <cstdint>
#include "../core/handling.hpp"
#include "../offsets/offsets.cpp"
#include "../config/configvalues.hpp"

constexpr uint32_t OFFSET_HIGHLIGHTSETTINGS = 0xb5f9620;        // Highlight function call

constexpr uint32_t OFFSET_HIGHLIGHTSERVERACTIVESTATES = 0x298;

constexpr uint32_t OFFSET_HIGHLIGHTCURRENTCONTEXTID = 0x294;    // Glow Enable, will take context ID
constexpr uint32_t OFFSET_HIGHLIGHTVISIBILITYTYPE = 0x278;      // Glow Through Walls, Will take 0 disable, 1 force ON, 2 Visibility to always

constexpr uint32_t HIGHLIGHT_TYPE_SIZE = 0x28;
constexpr uint32_t OFFSET_HIGHLIGHTDISTANCE = 0x26c;            // Distance

typedef struct
{
    float r, b, g;
} Color;

class Highlights {
private:
    //QWORD entityAddress;

public:
    //void SetEntityAddress(QWORD address);

    uint32_t GetHighlightCurrentContext(rx_handle process, QWORD entityAddress);
    
    void SetHighlightCurrentContext(rx_handle process, QWORD entityAddress, uint32_t CurrentContextId);
    void SetHighlightVisibilityType(rx_handle process, QWORD entityAddress, uint32_t VisibilityType);
    void SetHighlightActiveState(rx_handle process, QWORD entityAddress, uint32_t ContextId, uint32_t FunctionParameterId);

    void SetHighlightFunctionsWithActiveState(rx_handle process, QWORD entityAddress, uint32_t ContextId, uint8_t InsideFunction, uint8_t OutlineFunction, float OutlineRadius, bool EntityVisible, uint32_t State, bool AfterPostProcess);
    void SetHighlightFunctions(rx_handle process, QWORD entityAddress, uint32_t settingIndex, uint8_t InsideFunction, uint8_t OutlineFunction, float OutlineRadius, bool EntityVisible, uint32_t State, bool AfterPostProcess);
    
    //void SetHighlightParameter(rx_handle process, QWORD entityAddress, uint32_t settingIndex, uint32_t FunctionParameterId, uint32_t* Parameter);
    void SetHighlightParameter(rx_handle process, QWORD entityAddress, uint32_t FunctionParameterId, float* Parameter);

    void SetHighlightDistance(rx_handle process, QWORD entityAddress, float Distance);
};

#endif // HIGHLIGHTS_HPP
