#ifndef AUTO_GRAPPLE_HPP
#define AUTO_GRAPPLE_HPP

#include <cstdio>
#include <iostream>

#include "../core/handling.hpp"
#include "../config/configvalues.hpp"

#include "../player/localplayer.hpp"
#include "../player/level.hpp"
#include "../offsets/offsets.cpp"

class AutoGrapple {

private:
    Level levelClass;
    LocalPlayer localplayerClass;

public:
    AutoGrapple();
    ~AutoGrapple();

    // Function to determine whether the AutoGrapple feature should be enabled
    bool shouldAutoGrappleEnable(rx_handle process);

    // Function to apply the AutoGrapple logic
    void ApplyAutoGrapple(rx_handle process);
};

#endif // AUTO_GRAPPLE_HPP