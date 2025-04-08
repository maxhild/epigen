# /Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska/plot_cpg_states.py

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

def plot_cpg_states(csv_file):
    # Read the entire file as text
    with open(csv_file, 'r') as f:
        content = f.read()
    
    # Split the content by the double newline that separates the two tables
    tables = content.split('\n\n')
    
    # Read the first table (site-specific states)
    site_states = pd.read_csv(pd.StringIO(tables[0]))
    
    # Read the second table (state counts)
    state_counts = pd.read_csv(pd.StringIO(tables[1]))
    
    # Create a figure with two subplots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 8))
    
    # Plot 1: Heatmap of states at each site
    pivot_data = site_states.pivot_table(index="Step", columns="Site", values="State", aggfunc=lambda x: x)
    
    # Define colormap for states
    states = ['U', 'M', 'H', 'F', 'C']
    colors = ['grey', 'blue', 'green', 'red', 'yellow']
    cmap = dict(zip(states, range(len(states))))
    
    # Convert states to numeric values for the heatmap
    heatmap_data = pivot_data.applymap(lambda x: cmap.get(x, -1))
    
    # Create heatmap
    sns.heatmap(heatmap_data, cmap=plt.cm.colors.ListedColormap(colors), 
                cbar=False, ax=ax1, vmin=0, vmax=len(states)-1)
    
    # Add legend manually
    for i, state in enumerate(states):
        ax1.bar(0, 0, color=colors[i], label=state, alpha=0.7)
    
    ax1.legend(title="State")
    ax1.set_title("CpG Site States Over Time")
    ax1.set_ylabel("Step")
    ax1.set_xlabel("Site")
    
    # Plot 2: Line chart of state counts over time
    for state in states:
        if state in state_counts.columns:
            ax2.plot(state_counts['Step'], state_counts[state], 'o-', label=state)
    
    ax2.set_title("Number of Sites in Each State Over Time")
    ax2.set_xlabel("Step")
    ax2.set_ylabel("Count")
    ax2.legend(title="State")
    ax2.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('cpg_states_plot.png', dpi=300)
    plt.show()

if __name__ == "__main__":
    plot_cpg_states("cpg_states.csv")