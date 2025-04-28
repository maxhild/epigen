# Visualisierung in der Kommandozeile

g++ -std=c++11 epigenetic_net.cpp visualization.cpp main.cpp -o epigenetic_model

# Visualisierung mit Farbgebung
cd "/Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska"
gcc -o Histone_visualization Histone_visualization.c
./Histone_visualization

./epigenetic-model

# Heatmap generieren

Die Heatmap und das Liniendiagramm kann mit der pythonerweiterung generiert werden.
python plot_Histone_states.py

Für Cluster und Autokorrelation
python cluster_stats.py

Für equilibrium
python calculate_eq.py


## Relevante Pakete installieren
pip install pandas matplotlib seaborn numpy
