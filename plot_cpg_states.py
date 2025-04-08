
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from io import StringIO 

def plot_cpg_states(csv_file):
    with open(csv_file, 'r') as f:
        content = f.read()
    
    tables = content.split('\n\n')
    
    site_states = pd.read_csv(StringIO(tables[0]))  # Fixed: use StringIO not pd.StringIO
    
    state_counts = pd.read_csv(StringIO(tables[1]))  # Fixed: use StringIO not pd.StringIO
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    pivot_data = site_states.pivot_table(index="Step", columns="Site", values="State", aggfunc=lambda x: x)
    
    states = ['U', 'M', 'H', 'F', 'C']
    colors = ['grey', 'blue', 'green', 'red', 'yellow']
    cmap = dict(zip(states, range(len(states))))
    
    heatmap_data = pivot_data.applymap(lambda x: cmap.get(x, -1))
    
    sns.heatmap(heatmap_data, cmap=plt.cm.colors.ListedColormap(colors), 
                cbar=False, ax=ax1, vmin=0, vmax=len(states)-1)
    
    for i, state in enumerate(states):
        ax1.bar(0, 0, color=colors[i], label=state, alpha=0.7)
    
    ax1.legend(title="Zustand")
    ax1.set_title("Zustand an CpG Orten über die Zeit")
    ax1.set_ylabel("Schritt")
    ax1.set_xlabel("Ort")
    
    for state in states:
        if state in state_counts.columns:
            ax2.plot(state_counts['Step'], state_counts[state], 'o-', label=state)
    
    ax2.set_title("Anzahl der Zustände über die Zeit")
    ax2.set_xlabel("Schritt")
    ax2.set_ylabel("Menge")
    ax2.legend(title="Zustand")
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('cpg_states_plot.png', dpi=300)
    plt.show()

if __name__ == "__main__":
    plot_cpg_states("cpg_states.csv")