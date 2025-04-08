#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "./matplotlib-cpp/matplotlib.hpp"

// Nucleosome structure
struct Nucleosome {
    int position;
    std::vector<std::string> modifications;
    bool isActive;
    Nucleosome(int pos) : position(pos), isActive(false) {}
};

// Base class for chromatin modifiers
class ChromatinModifier {
public:
    virtual void modify(Nucleosome& nucleosome) = 0;
    virtual ~ChromatinModifier() = default;
};

// Writers for different modifications
class AcetylationWriter : public ChromatinModifier {
public:
    void modify(Nucleosome& nucleosome) override {
        nucleosome.modifications.push_back("H3K27ac");
        nucleosome.isActive = true;
    }
};

class MethylationWriter : public ChromatinModifier {
public:
    void modify(Nucleosome& nucleosome) override {
        nucleosome.modifications.push_back("H3K9me3");
        nucleosome.isActive = false; // Silencing modification
    }
};

class PhosphorylationWriter : public ChromatinModifier {
public:
    void modify(Nucleosome& nucleosome) override {
        nucleosome.modifications.push_back("H3S10ph");
        nucleosome.isActive = true; // Signal propagation modification
    }
};

// Readers for interpreting modifications
class Reader {
public:
    virtual void read(const Nucleosome& nucleosome) = 0;
    virtual ~Reader() = default;
};

class AcetylationReader : public Reader {
public:
    void read(const Nucleosome& nucleosome) override {
        std::cout << "Acetylation Reader - Position: " << nucleosome.position << "\n";
        for (const auto& mod : nucleosome.modifications) {
            if (mod.find("ac") != std::string::npos) {
                std::cout << "Detected acetylation: " << mod << "\n";
            }
        }
    }
};

// Function to propagate signals across nucleosomes
void propagateSignal(std::vector<Nucleosome>& chromatin) {
    for (size_t i = 0; i < chromatin.size(); ++i) {
        if (chromatin[i].isActive && i + 1 < chromatin.size()) {
            chromatin[i + 1].isActive = true;
            chromatin[i + 1].modifications.push_back("SignalPropagated");
        }
    }
}

// Function to simulate transcription based on chromatin state
void simulateTranscription(const std::vector<Nucleosome>& chromatin) {
    for (const auto& nucleosome : chromatin) {
        if (nucleosome.isActive) {
            std::cout << "Gene at position " << nucleosome.position << " is transcribed.\n";
        } else {
            std::cout << "Gene at position " << nucleosome.position << " is silent.\n";
        }
    }
}

int main() {
    srand(time(0)); 
    std::vector<Nucleosome> chromatin;
    for (int i = 0; i < 10; ++i) {
        chromatin.emplace_back(i);
    }


    AcetylationWriter acetylationWriter;
    MethylationWriter methylationWriter;
    PhosphorylationWriter phosphorylationWriter;

    for (int step = 0; step < 5; ++step) {
        std::cout << "=== Simulation Step " << step + 1 << " ===\n";

        for (auto& nucleosome : chromatin) {
            int randomEvent = rand() % 3;
            if (randomEvent == 0) acetylationWriter.modify(nucleosome);
            if (randomEvent == 1) methylationWriter.modify(nucleosome);
            if (randomEvent == 2) phosphorylationWriter.modify(nucleosome);
        }

        propagateSignal(chromatin);

        simulateTranscription(chromatin);

        std::cout << "\n";
    }

    return 0;
}
