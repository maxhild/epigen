#pragma once
#include <vector>
#include <string>
#include <map>
#include <memory>

enum class ModificationState {
    UNMODIFIED,
    METHYLATED,
    PHOSPHORYLATED,
    ACETYLATED,
    UBIQUITYLATED
};

class EpigeneticSite {
private:
    ModificationState state;
    std::string position;
    
public:
    EpigeneticSite(); 
    EpigeneticSite(const std::string& pos, ModificationState initialState = ModificationState::UNMODIFIED);
    void setState(ModificationState newState);
    ModificationState getState() const;
    std::string getPosition() const;
};

class ConditionalNet {
private:
    std::map<std::string, EpigeneticSite> sites;
    std::map<std::string, std::map<std::string, double>> transitionProbabilities;
    
public:
    void addSite(const std::string& position, ModificationState initialState = ModificationState::UNMODIFIED);
    void addTransition(const std::string& from, const std::string& to, double probability);
    void simulateStep();
    std::map<std::string, EpigeneticSite>& getSites();
    void exportStateToCSV(const std::string& filename, int step, bool header = false);
    void applyProhaskaRules();
};