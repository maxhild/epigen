// /Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska/visualization.h
#pragma once
#include "epigenetic_net.h"
#include <iostream>

class NetVisualizer {
public:
    static void displayNetwork(const ConditionalNet& net);
    static void displayStateDistribution(const ConditionalNet& net);
    static std::string stateToString(ModificationState state);
};