// /Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska/epigenetic_net.cpp
#include "epigenetic_net.h"
#include <random>
#include <fstream>

EpigeneticSite::EpigeneticSite()
    : position(""), state(ModificationState::UNMETHYLATED) {}

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

void ConditionalNet::applyDeterministicProhaskaRules() {
    // Kopiere aktuellen Zustand für paralleles Update
    std::map<std::string, EpigeneticSite> next = sites;
    int N = sites.size();
    std::vector<std::string> keys;
    keys.reserve(N);
    for (const auto &p : sites) keys.push_back(p.first);

    for (int idx = 0; idx < N; ++idx) {
        const auto &pos = keys[idx];
        bool leftAc = false, rightMe = false;
        // linker Nachbar
        if (idx > 0) {
            const auto &l = sites.at(keys[idx-1]).getState();
            leftAc = (l == ModificationState::FORMYLATED); // FORM = Acetylation
        }
        // rechter Nachbar
        if (idx+1 < N) {
            const auto &r = sites.at(keys[idx+1]).getState();
            rightMe = (r == ModificationState::METHYLATED);
        }
        // Regel 1: linker Nachbar acet. -> FORM
        if (leftAc) {
            next[pos].setState(ModificationState::FORMYLATED);
        }
        // Regel 2: rechter Nachbar methyliert -> METH
        else if (rightMe) {
            next[pos].setState(ModificationState::METHYLATED);
        }
        // sonst bleibt Zustand wie gehabt
    }
    // Übernehmen
    for (auto &p : next) {
        sites[p.first] = p.second;
    }
}

void ConditionalNet::simulateStep() {
    // 1. Deterministisch nach Prohaska
    applyDeterministicProhaskaRules();

    // 2. Stochastischer Markov-Update (wie bisher)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::map<std::string, ModificationState> nextStates;
    for (auto& pair : sites) {
        const std::string& pos = pair.first;
        ModificationState currentState = pair.second.getState();

        std::string stateKey;
        switch (currentState) {
            case ModificationState::UNMETHYLATED: stateKey = "U"; break;
            case ModificationState::METHYLATED: stateKey = "M"; break;
            case ModificationState::HYDROXYMETHYLATED: stateKey = "H"; break;
            case ModificationState::FORMYLATED: stateKey = "F"; break;
            case ModificationState::CARBOXYLATED: stateKey = "C"; break;
            default: stateKey = "?";
        }

        if (transitionProbabilities.find(stateKey) != transitionProbabilities.end()) {
            double random = dis(gen);
            double cumulative = 0.0;
            ModificationState nextState = currentState;

            for (const auto& transition : transitionProbabilities[stateKey]) {
                cumulative += transition.second;
                if (random <= cumulative) {
                    if (transition.first == "U") nextState = ModificationState::UNMETHYLATED;
                    else if (transition.first == "M") nextState = ModificationState::METHYLATED;
                    else if (transition.first == "H") nextState = ModificationState::HYDROXYMETHYLATED;
                    else if (transition.first == "F") nextState = ModificationState::FORMYLATED;
                    else if (transition.first == "C") nextState = ModificationState::CARBOXYLATED;
                    break;
                }
            }
            nextStates[pos] = nextState;
        } else {
            nextStates[pos] = currentState;
        }
    }
    for (auto& pair : sites) {
        pair.second.setState(nextStates[pair.first]);
    }
}

void ConditionalNet::exportStateToCSV(const std::string& filename, int step, bool header) {
    std::ofstream file(filename, std::ios::app);
    if (!file) return;

    if (header) {
        file << "Step,Site,State\n";
    }
    for (const auto& pair : sites) {
        file << step << "," << pair.first << ",";
        // Enum zu String
        switch (pair.second.getState()) {
            case ModificationState::UNMETHYLATED: file << "U"; break;
            case ModificationState::METHYLATED: file << "M"; break;
            case ModificationState::HYDROXYMETHYLATED: file << "H"; break;
            case ModificationState::FORMYLATED: file << "F"; break;
            case ModificationState::CARBOXYLATED: file << "C"; break;
            default: file << "?";
        }
        file << "\n";
    }
}

std::map<std::string, EpigeneticSite>& ConditionalNet::getSites() {
    return sites;
}