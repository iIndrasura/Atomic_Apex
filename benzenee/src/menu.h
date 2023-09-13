#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <map>
#include <string>
#include "configvalues.h"

/* ------------------ ANSI escape ------------------ */
// Define ANSI escape codes for text styles and colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define ITALIC  "\033[3m"
#define UNDERLINE "\033[4m"
#define BLINK   "\033[5m"
#define INVERSE "\033[7m"

// Define ANSI escape codes for text colors
#define FG_BLACK   "\033[30m"
#define FG_RED     "\033[31m"
#define FG_GREEN   "\033[32m"
#define FG_YELLOW  "\033[33m"
#define FG_BLUE    "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN    "\033[36m"
#define FG_WHITE   "\033[37m"

// Define ANSI escape codes for BackGround colors
#define BG_BLACK      "\033[40m"
#define BG_RED        "\033[41m"
#define BG_GREEN      "\033[42m"
#define BG_YELLOW     "\033[43m"
#define BG_BLUE       "\033[44m"
#define BG_MAGENTA    "\033[45m"
#define BG_CYAN       "\033[46m"
#define BG_WHITE      "\033[47m"

namespace Menu {

void displayHeader();
void loadBar();

//void displayConfigValues(const std::map<std::string, std::string>& config);
void displayConfigValues();

}