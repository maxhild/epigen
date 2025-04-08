// /Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska/epigenetic_net.cpp
#include "epigenetic_net.h"
#include <random>

EpigeneticSite::EpigeneticSite(const std::string& pos, ModificationState initialState) 
    : position(pos), state(initialState) {}

void EpigeneticSite::setState(ModificationState newState) {
    state = newState;
}

ModificationState EpigeneticSite::getState() const {
    return state;
}

std::string EpigeneticSite::getPosition() const {
    return position;
}

void ConditionalNet::addSite(const std::string& position, ModificationState initialState) {
    sites.emplace(position, EpigeneticSite(position, initialState));
}

void ConditionalNet::addTransition(const std::string& from, const std::string& to, double probability) {
    transitionProbabilities[from][to] = probability;
}

void ConditionalNet::simulateStep() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    // Implement the conditional transitions based on Prohaska's model
    // This is a simplified version - actual implementation would follow the paper in detail
    for (auto& pair : sites) {
        const std::string& pos = pair.first;
        if (transitionProbabilities.find(pos) != transitionProbabilities.end()) {
            double random = dis(gen);
            double cumulative = 0.0;
            
            for (const auto& transition : transitionProbabilities[pos]) {
                cumulative += transition.second;
                if (random <= cumulative) {
                    // Transition to the next state based on model
                    // This is simplified - actual implementation would be based on the paper
                    if (pair.second.getState() == ModificationState::UNMETHYLATED) {
                        pair.second.setState(ModificationState::METHYLATED);
                    }
                    break;
                }
            }
        }
    }
}

std::map<std::string, EpigeneticSite>& ConditionalNet::getSites() {
    return sites;
}