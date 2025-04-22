# Visualisierung in der Kommandozeile

g++ -std=c++11 epigenetic_net.cpp visualization.cpp main.cpp -o epigenetic_model

# Visualisierung mit Farbgebung
cd "/Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska"
gcc -o cpg_visualization cpg_visualization.c
./cpg_visualization

./epigenetic-model

# Heatmap generieren

Die Heatmap kann mit der pythonerweiterung generiert werden.
python plot_cpg_states.py


## Relevante Pakete installieren
pip install pandas matplotlib seaborn numpy
