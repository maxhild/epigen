// /Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska/visualization.cpp
#include "visualization.h"
#include <map>

std::string NetVisualizer::stateToString(ModificationState state) {
    switch(state) {
        case ModificationState::UNMETHYLATED: return "U";
        case ModificationState::METHYLATED: return "M";
        case ModificationState::HYDROXYMETHYLATED: return "H";
        case ModificationState::FORMYLATED: return "F";
        case ModificationState::CARBOXYLATED: return "C";
        default: return "?";
    }
}

void NetVisualizer::displayNetwork(const ConditionalNet& net) {
    const auto& sites = const_cast<ConditionalNet&>(net).getSites();
    
    std::cout << "Epigenetic Network State:\n";
    for (const auto& pair : sites) {
        std::cout << "Site " << pair.first << ": " 
                  << stateToString(pair.second.getState()) << "\n";
    }
}

void NetVisualizer::displayStateDistribution(const ConditionalNet& net) {
    const auto& sites = const_cast<ConditionalNet&>(net).getSites();
    std::map<ModificationState, int> distribution;
    
    for (const auto& pair : sites) {
        distribution[pair.second.getState()]++;
    }
    
    std::cout << "State Distribution:\n";
    std::cout << "Unmethylated: " << distribution[ModificationState::UNMETHYLATED] << "\n";
    std::cout << "Methylated: " << distribution[ModificationState::METHYLATED] << "\n";
    std::cout << "Hydroxymethylated: " << distribution[ModificationState::HYDROXYMETHYLATED] << "\n";
    std::cout << "Formylated: " << distribution[ModificationState::FORMYLATED] << "\n";
    std::cout << "Carboxylated: " << distribution[ModificationState::CARBOXYLATED] << "\n";
}