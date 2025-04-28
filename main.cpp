#include "epigenetic_net.h"
#include "visualization.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    ConditionalNet model;

    // MARKOVMODELL UND PROHASKA MODELL INITIALISIEREN

    bool useProhaska = true; // Flag für deterministisch/stochastisch
    bool useMarkov = true; // Flag für Markovmodell als Add-On

    // SETZEN DER ANFANGSZUSTÄNDE
    // Epigenetische Sites hinzufügen
    // Beispiel: Verschiedene Anfangszustände setzen
    //model.addSite("Histone_1", ModificationState::UNMODIFIED);
    //model.addSite("Histone_2", ModificationState::UNMODIFIED);
    //model.addSite("Histone_3", ModificationState::UNMODIFIED);
    //model.addSite("Histone_4", ModificationState::UNMODIFIED);
    //model.addSite("Histone_5", ModificationState::UNMODIFIED);
    //model.addSite("Histone_6", ModificationState::UNMODIFIED);
    //model.addSite("Histone_7", ModificationState::UNMODIFIED);
    //model.addSite("Histone_8", ModificationState::UNMODIFIED);
    //model.addSite("Histone_9", ModificationState::UNMODIFIED);
    //model.addSite("Histone_10", ModificationState::UNMODIFIED);

    model.addSite("Histone_1", ModificationState::METHYLATED);
    model.addSite("Histone_2", ModificationState::UNMODIFIED);
    model.addSite("Histone_3", ModificationState::METHYLATED);
    model.addSite("Histone_4", ModificationState::UNMODIFIED);
    model.addSite("Histone_5", ModificationState::ACETYLATED);
    model.addSite("Histone_6", ModificationState::UNMODIFIED);
    model.addSite("Histone_7", ModificationState::METHYLATED);
    model.addSite("Histone_8", ModificationState::UNMODIFIED);
    model.addSite("Histone_9", ModificationState::METHYLATED);
    model.addSite("Histone_10", ModificationState::UNMODIFIED);

    
    // SETZEN DER ÜBERGANGSWAHRSCHEINLICHKEITEN FÜR DAS STATISTISCHE MODELL
    // Beispielhafte Übergangswahrscheinlichkeiten gemäß Literatur:
    // De-novo-Methylierung nach Fu et al.(Tochterstrang)
    //model.addTransition("U", "M", 0.07);
    // Maintenance-Fehlerrate nach Fu et al.(Methylated → Unmethylated)
    //model.addTransition("M", "U", 0.024);

    // Zufällige Übergangswahrscheinlichkeiten als Erweiterung
    if (useMarkov) {
        std::vector<std::string> states = {"M", "A", "P", "Ub"};
        for (auto from : states) {
            model.addTransition(from, "U", 0.1);
        }
        
        for (auto to : states) {
            model.addTransition("U", to, 0.1);
        }

        model.addTransition("A", "P", 0.5);
        model.addTransition("A", "Ub", 0.5);
    }

    // Simulation initialisieren
    std::cout << "Starten der Simulation...\n\n";

    std::string csvFile = "Histone_Prohaska_and_Markov_1000.csv";
    // Header nur beim ersten Mal schreiben
    model.exportStateToCSV(csvFile, 0, true);

    for (int step = 0; step < 1000; step++) { 
        std::cout << "Step " << step << "\n";
        NetVisualizer::displayNetwork(model);
        NetVisualizer::displayStateDistribution(model);
        std::cout << "\n";
        
        if (useProhaska) {
            model.applyProhaskaRules();
        }

        if (useMarkov){
            model.simulateStep();
        }


        // CSV-Export (kein Header mehr)
        model.exportStateToCSV(csvFile, step + 1, false);
        
        // Pause für Sichtbarkeit der Simulation
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}