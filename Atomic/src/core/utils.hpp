#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cstdio>

namespace Utils {

    // Clear screen function
    void clearScreen() {
        printf("\e[H\e[2J\e[3J");
    }

    // Save cursor position
    void saveCursor() {
        printf("\e[s");
    }

    // Restore cursor position
    void restoreCursor() {
        printf("\e[u");
    }

    // Clear everything between saved and restored cursor positions
    void cleanBetween() {
        printf("\e[J");
    }

}

#endif