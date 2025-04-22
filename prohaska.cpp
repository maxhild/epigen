
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

// Modelliert ein Nukleosom mit Position, Modifikationen und Aktivitätsflag
struct Nucleosome {
    int position;
    bool isActive;
    bool acetylated;        // H3K27ac
    bool methylated;        // H3K9me3
    Nucleosome(int pos)
        : position(pos), isActive(false), acetylated(false), methylated(false) {}
};

// Anwenden der Prohaska-Regeln deterministisch auf das Chromatin
void applyProhaskaRules(std::vector<Nucleosome>& chrom) {
    // paralleles Update speichern
    std::vector<Nucleosome> next = chrom;
    int N = chrom.size();
    for (int i = 0; i < N; ++i) {
        bool leftAc  = (i>0  && chrom[i-1].acetylated);
        bool rightMe = (i+1 < N && chrom[i+1].methylated);
        // Regel 1: Wenn linker Nachbar acetylated, schreibe acetylation
        if (leftAc) {
            next[i].acetylated = true;
            next[i].methylated = false;
            next[i].isActive  = true;
        }
        // Regel 2: Sonst wenn rechter Nachbar methylated, schreibe methylation
        else if (rightMe) {
            next[i].methylated = true;
            next[i].acetylated = false;
            next[i].isActive   = false;
        }
        // Sonst Zustand beibehalten
    }
    chrom.swap(next);
}

void exportToCsv(const std::vector<Nucleosome>& chrom, int step, std::ofstream& out) {
    for (auto const& n : chrom) {
        out << step << ',' << n.position << ','
            << n.acetylated << ','
            << n.methylated << ','
            << n.isActive << '\n';
    }
}

int main(){
    const int N = 50;
    const int STEPS = 100;
    std::vector<Nucleosome> chrom;
    chrom.reserve(N);
    for(int i=0;i<N;++i) chrom.emplace_back(i);
    
    // Initialzustand: Nukleosom 0 acetylated (Eingabesignal)
    chrom[0].acetylated = true;
    chrom[0].isActive  = true;

    // CSV-Datei öffnen
    std::ofstream out("prohaska_sim.csv");
    out << "Step,Position,Acetylated,Methylated,Active\n";

    // Simulation
    for (int step=0; step<STEPS; ++step) {
        applyProhaskaRules(chrom);
        // Signalweiterleitung: aktive Nukleosomen aktivieren rechtes nächstes
        for(int i=0;i+1<N;++i) {
            if(chrom[i].isActive) chrom[i+1].isActive = true;
        }
        exportToCsv(chrom, step, out);
    }

    out.close();
    std::cout << "Prohaska-Simulation abgeschlossen: prohaska_sim.csv" << std::endl;
    return 0;
}
