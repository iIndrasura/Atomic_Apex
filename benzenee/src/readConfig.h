#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>

void readConfig(std::map<std::string, std::string>& config);

//GLOW Color split
void splitString(const std::string& input, char delimiter, std::vector<std::string>& output);

// Item ESP
bool IsInItemEspIds(int itemID, const std::vector<int>& itemEspIds);
bool IsInLobaEspIds(int itemID, const std::vector<int>& lobaEspIds);