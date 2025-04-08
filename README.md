# Visualisierung in der Kommandozeile

g++ -std=c++11 epigenetic_net.cpp visualization.cpp main.cpp -o epigenetic_model

# Visualisierung mit Farbgebung
cd "/Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska"
gcc -o cpg_visualization cpg_visualization.c
./cpg_visualization

# Heatmap generieren

Die Heatmap kann mit der pythonerweiterung generiert werden.

## Relevante Pakete installieren
pip install pandas matplotlib seaborn numpy

## Skript laufen lassen
python plot_cpg_states.py
