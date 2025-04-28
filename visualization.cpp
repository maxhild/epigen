#include "visualization.h"
#include <map>

std::string NetVisualizer::stateToString(ModificationState state) {
    switch(state) {
        case ModificationState::UNMODIFIED: return "U";
        case ModificationState::METHYLATED: return "M";
        case ModificationState::PHOSPHORYLATED: return "P";
        case ModificationState::ACETYLATED: return "A";
        case ModificationState::UBIQUITYLATED: return "Ub";
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
    std::cout << "Unmodified: " << distribution[ModificationState::UNMODIFIED] << "\n";
    std::cout << "Methylated: " << distribution[ModificationState::METHYLATED] << "\n";
    std::cout << "Phosphorylated: " << distribution[ModificationState::PHOSPHORYLATED] << "\n";
    std::cout << "Acetylated: " << distribution[ModificationState::ACETYLATED] << "\n";
    std::cout << "Ubiquitylation: " << distribution[ModificationState::UBIQUITYLATED] << "\n";
}