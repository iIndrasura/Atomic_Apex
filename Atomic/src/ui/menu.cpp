#include "menu.hpp"

void Menu::displayHeader() {
    std::cout << BOLD << ITALIC << BG_RED << "\t\tBy: indrasura" << RESET << std::endl;
	std::cout << "\t\t "<< BOLD << BG_MAGENTA << "I Am Atomic" << RESET << std::endl;
}

void Menu::loadBar(){
    int totalSteps = 100; // Total number of steps in the loading bar
    
    std::cout << "[" << BOLD;
    for (int currentStep = 0; currentStep <= totalSteps; ++currentStep) {
        // Calculate the percentage completed
        double progress = static_cast<double>(currentStep) / totalSteps * 100.0;

        // Calculate the number of '=' to print in the loading bar
        int numEquals = static_cast<int>((progress / 100.0) * 40);

        // Print the loading bar
		std::cout << "\r[" << FG_CYAN << BOLD;
        for (int i = 0; i < 40; ++i) {
            if (i < numEquals)
                std::cout << "=";
            else
                std::cout << " ";
        }
        std::cout << "] " << int(progress) << "%" << std::flush;

        // Wait for a short time to simulate progress
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
    }

	std::cout << RESET << std::endl;
    //std::cout << "\t\t "<< BOLD << BG_MAGENTA << "I Am Atomic" << RESET << std::endl;
}

/* 
void Menu::displayConfigValues(const std::map<std::string, std::string>& config)  {
    int lineWidth = 40; // Draw Line width

    // Print "populated config values......"
    std::cout << "populateing config values......" << RESET << std::endl;

    std::cout << FG_BLUE << "AIMBOT\t\t\t" << RESET << (std::stof(config.at("AIMBOT.AIMSMOOTH")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "TRIGGERBOT\t\t" << RESET << (std::stof(config.at("TRIGGERBOT.TRIGGER_ENABLED")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "NORECOIL\t\t" << RESET << (std::stof(config.at("NORECOIL.NORECOIL_ENABLED")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "NORECOIL2\t\t" << RESET << (std::stof(config.at("NORECOIL.NORECOIL2_ENABLED")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "GLOW\t\t\t" << RESET << (std::stof(config.at("GLOW.GLOW_ENABLED")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "GLOW_ENEMY\t\t" << RESET << (std::stof(config.at("GLOW.GLOW_ENEMY")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "GLOW_HEALTH\t\t" << RESET << (std::stof(config.at("GLOW.GLOW_HEALTH")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "ITEM_ESP\t\t" << RESET << (std::stof(config.at("ESP.ITEM_ESP_ENABLED")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "WEAPON_CHAMS\t\t" << RESET << (std::stof(config.at("CHAMS.WEAPON_CHAMS")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "ARMS_CHAMS\t\t" << RESET << (std::stof(config.at("CHAMS.ARMS_CHAMS")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
	std::cout << FG_BLUE << "SPEC_COUNT\t\t" << RESET << (std::stof(config.at("MISC.SPECTATOR_COUNT")) ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;

    std::cout << std::string(lineWidth, '_') << std::endl;
}
 */

void Menu::displayConfigValues() {
    int lineWidth = 40; // Draw Line width

    // Print "populated config values..."
    std::cout << "config initialized......" << RESET << std::endl;

    std::cout << FG_BLUE << "AIMBOT\t\t\t" << RESET << (ConfigValues::AIMSMOOTH ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "TRIGGERBOT\t\t" << RESET << (ConfigValues::TRIGGER_ENABLED ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "NORECOIL\t\t" << RESET << (ConfigValues::NORECOIL_ENABLED ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    //std::cout << FG_BLUE << "NORECOIL2\t\t" << RESET << (ConfigValues::NORECOIL2_ENABLED ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "GLOW\t\t\t" << RESET << (ConfigValues::GLOW_ENABLED ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "GLOW_ENEMY\t\t" << RESET << (ConfigValues::GLOW_ENEMY ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "GLOW_HEALTH\t\t" << RESET << (ConfigValues::GLOW_HEALTH ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "ITEM_ESP\t\t" << RESET << (ConfigValues::ITEM_ESP_ENABLED ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "WEAPON_CHAMS\t\t" << RESET << (ConfigValues::WEAPON_CHAMS ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "ARMS_CHAMS\t\t" << RESET << (ConfigValues::ARMS_CHAMS ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "SKIN_CHANGER\t\t" << RESET << (ConfigValues::SKIN_CHANGER ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;
    std::cout << FG_BLUE << "SPEC_COUNT\t\t" << RESET << (ConfigValues::SPECTATOR_COUNT ? FG_YELLOW "Enable" : FG_YELLOW "Disable") << RESET << std::endl;

    std::cout << std::string(lineWidth, '_') << std::endl;
}
