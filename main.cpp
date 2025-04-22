#include "epigenetic_net.h"
#include "visualization.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    ConditionalNet model;
    
    // Epigenetische Sites hinzufügen
    // Beispiel: Verschiedene Anfangszustände setzen
    model.addSite("CpG_1", ModificationState::METHYLATED);
    model.addSite("CpG_2", ModificationState::UNMETHYLATED);
    model.addSite("CpG_3", ModificationState::HYDROXYMETHYLATED);
    // Für alle anderen Sites ggf. Schleife:
    for (int i = 4; i <= 10; i++) {
        model.addSite("CpG_" + std::to_string(i), ModificationState::UNMETHYLATED);
    }
    

    // Beispielhafte Übergangswahrscheinlichkeiten gemäß Literatur:
    // De-novo-Methylierung nach Fu et al.(Tochterstrang)
    // model.addTransition("U", "M", 0.07);
    // Maintenance-Fehlerrate nach Fu et al.(Methylated → Unmethylated)
    // model.addTransition("M", "U", 0.024);
    
    model.addTransition("M", "H", 0.5);
    model.addTransition("H", "M", 0.5);
    model.addTransition("H", "U", 0.5);
    model.addTransition("U", "H", 0.5);
    model.addTransition("C", "F", 0.5);
    model.addTransition("F", "C", 0.5);
    model.addTransition("U", "C", 0.5);
    model.addTransition("C", "U", 0.5);
    model.addTransition("M", "C", 0.5);
    // Zusätzliche Übergänge für H, F, C
    model.addTransition("H", "U", 0.5);
    model.addTransition("H", "M", 0.5);
    model.addTransition("M", "H", 0.5);
    model.addTransition("U", "H", 0.5);

    // Simulation initialisieren
    std::cout << "Starten der Simulation...\n\n";

    bool useProhaska = true; // Flag für deterministisch/stochastisch
    bool useMarkov = true; // Flag für Markov-Chain
    std::string csvFile = "cpg_states_stoch_det.csv";
    // Header nur beim ersten Mal schreiben
    model.exportStateToCSV(csvFile, 0, true);

    for (int step = 0; step < 100000; step++) { // Erhöhe die Schrittzahl hier
        std::cout << "Step " << step << "\n";
        NetVisualizer::displayNetwork(model);
        NetVisualizer::displayStateDistribution(model);
        std::cout << "\n";
        
        if (useProhaska) {
            model.applyDeterministicProhaskaRules();
            if (useMarkov){
                model.simulateStep();
            }

        } else {
            // Simulieren des nächsten Schritts
            model.simulateStep();
        }

        // CSV-Export (kein Header mehr)
        model.exportStateToCSV(csvFile, step + 1, false);
        
        // Pause für Sichtbarkeit der Simulation
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}