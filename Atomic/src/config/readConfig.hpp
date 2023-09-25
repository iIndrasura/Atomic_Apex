#ifndef READCONFIG_HPP
#define READCONFIG_HPP

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <chrono>

#include <unordered_set>

void readConfig(std::map<std::string, std::string>& config);

//GLOW Color split
void splitString(const std::string& input, char delimiter, std::vector<std::string>& output);

// Item ESP
// bool IsInItemEspIds(int itemID, const std::vector<int>& itemEspIds);
// bool IsInLobaEspIds(int itemID, const std::vector<int>& lobaEspIds);

bool IsInLobaEspIds(int itemID, const std::unordered_set<int>& lobaEspIds);
bool IsInItemEspIds(int itemID, const std::unordered_set<int>& itemEspIds);

#endif // READCONFIG_HPP