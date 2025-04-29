#include "epigenetic_net.h"
#include <random>
#include <fstream>

EpigeneticSite::EpigeneticSite()
    : position(""), state(ModificationState::UNMODIFIED) {}

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

void ConditionalNet::applyProhaskaRules() {
    static std::mt19937_64 gen{std::random_device{}()};
    std::uniform_real_distribution<> dis(0.0, 1.0);

    constexpr double ERASER_PROB  = 0.0001;  // 1% Erase-Chance

    int N = sites.size();
    std::vector<std::string> keys; keys.reserve(N);
    for (auto &p : sites) keys.push_back(p.first);

    std::vector<bool> acetyl(N), methyl(N), active(N);
    for (int i = 0; i < N; ++i) {
        auto s = sites[keys[i]].getState();
        acetyl[i] = (s == ModificationState::ACETYLATED);
        methyl[i] = (s == ModificationState::METHYLATED);

    }

    std::vector<bool> nextAc = acetyl, nextMe = methyl, nextAct = active; 

    for (int i = 0; i < N; ++i) {
        if (dis(gen) < ERASER_PROB) {
            nextAc[i] = false;
            nextMe[i] = false;
        }
    }

    for (int i = 0; i < N; ++i) {
        bool leftAc  = (i > 0   && acetyl[i-1]);
        bool rightMe = (i+1<N  && methyl[i+1]);

        if (leftAc) {
            nextAc[i] = true;
            nextMe[i] = false;
        }
        else if (rightMe) {
            nextMe[i] = true;
            nextAc[i] = false;
        }
    }

    for (int i = 0; i < N; ++i) {
        if      (nextAc[i] && !nextMe[i]) nextAct[i] = true;
        else if (nextMe[i] && !nextAc[i]) nextAct[i] = false;
        else     nextAct[i] = (sites[keys[i]].getState() == ModificationState::ACETYLATED);
    }

    for (int i = 0; i < N; ++i) {
        auto &site = sites[keys[i]];
        if      (nextAc[i]) site.setState(ModificationState::ACETYLATED);
        else if (nextMe[i]) site.setState(ModificationState::METHYLATED);
        else                site.setState(ModificationState::UNMODIFIED);

    }
}

void ConditionalNet::simulateStep() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::map<std::string, ModificationState> nextStates;
    for (auto& pair : sites) {
        const std::string& pos = pair.first;
        ModificationState currentState = pair.second.getState();

        std::string stateKey;
        switch (currentState) {
            case ModificationState::UNMODIFIED: stateKey = "U"; break;
            case ModificationState::METHYLATED: stateKey = "M"; break;
            case ModificationState::PHOSPHORYLATED: stateKey = "P"; break;
            case ModificationState::ACETYLATED: stateKey = "A"; break;
            case ModificationState::UBIQUITYLATED: stateKey = "Ub"; break;
            default: stateKey = "?";
        }

        if (transitionProbabilities.find(stateKey) != transitionProbabilities.end()) {
            double random = dis(gen);
            double cumulative = 0.0;
            ModificationState nextState = currentState;

            for (const auto& transition : transitionProbabilities[stateKey]) {
                cumulative += transition.second;
                if (random <= cumulative) {
                    if (transition.first == "U") nextState = ModificationState::UNMODIFIED;
                    else if (transition.first == "M") nextState = ModificationState::METHYLATED;
                    else if (transition.first == "P") nextState = ModificationState::PHOSPHORYLATED;
                    else if (transition.first == "A") nextState = ModificationState::ACETYLATED;
                    else if (transition.first == "Ub") nextState = ModificationState::UBIQUITYLATED;   
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
            case ModificationState::UNMODIFIED: file << "U"; break;
            case ModificationState::METHYLATED: file << "M"; break;
            case ModificationState::PHOSPHORYLATED: file << "P"; break;
            case ModificationState::ACETYLATED: file << "A"; break;
            case ModificationState::UBIQUITYLATED: file << "Ub"; break;
            default: file << "?";
        }
        file << "\n";
    }
}

std::map<std::string, EpigeneticSite>& ConditionalNet::getSites() {
    return sites;
}