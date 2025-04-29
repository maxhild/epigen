# Modellierung

g++ -std=c++11 epigenetic_net.cpp visualization.cpp main.cpp -o epigenetic_model

./epigenetic-model

# Relevante Pakete installieren
pip install pandas matplotlib seaborn numpy

# Heatmap generieren

Die Heatmap und das Liniendiagramm kann mit der pythonerweiterung generiert werden.
python plot_states.py

Für Cluster und Autokorrelation
python cluster_stats.py

Für Equilibrium
python calculate_eq.py
