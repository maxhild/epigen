#include "epigenetic_net.h"
#include "visualization.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    ConditionalNet model;
    
    // Add epigenetic sites (example positions)
    for (int i = 1; i <= 10; i++) {
        model.addSite("CpG_" + std::to_string(i));
    }
    
    // Add transition probabilities based on the paper model
    // These values would be based on Prohaska's paper
    model.addTransition("CpG_1", "CpG_2", 0.3);
    model.addTransition("CpG_1", "CpG_3", 0.2);
    model.addTransition("CpG_2", "CpG_4", 0.4);
    
    // Run simulation and visualize
    std::cout << "Starting simulation of epigenetic modifications...\n\n";
    
    for (int step = 0; step < 10; step++) {
        std::cout << "Step " << step << "\n";
        NetVisualizer::displayNetwork(model);
        NetVisualizer::displayStateDistribution(model);
        std::cout << "\n";
        
        // Simulate next step
        model.simulateStep();
        
        // Pause for visibility
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return 0;
}