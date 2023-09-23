#ifndef CHAMS_HPP
#define CHAMS_HPP

#include <cstdio>
#include <iostream>

#include "../core/handling.hpp"
#include "../core/math.hpp"
#include "../config/configvalues.hpp"

#include "../player/localplayer.hpp"
#include "../player/level.hpp"
#include "../player/highlights.hpp"

#include "../offsets/offsets.cpp"

class Chams {

    private:
        LocalPlayer localplayerClass;
        Level levelClass;
        Highlights highlightClass;

    public:
        Chams();
        ~Chams();

        bool shouldChamsEnable(rx_handle process); // Declaration for the condition check function
        void ApplyChamsLogic(rx_handle process) ; // Declaration for the chams logic function
        void ApplyChamsToViewModel(rx_handle process, uintptr_t viewModelPtr, float red, float green, float blue);
};

#endif // CHAMS_HPP