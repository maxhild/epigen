import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from matplotlib.colors import ListedColormap

def plot_histone_states(csv_file):
    # 1) CSV einlesen (alle Spalten als strings, low_memory=False)
    df = pd.read_csv(csv_file, dtype=str, low_memory=False)

    if 'State' not in df.columns:
        # rename Position -> Site
        df = df.rename(columns={'Position':'Site'})

    else:
        # für klassisches Histone-CSV
        df = df.rename(columns={'Site':'Site'})

    df = df[df['Step'].str.isnumeric()]
    df['Step'] = df['Step'].astype(int)

    states = ['U','M','P','A','Ub']
    state_counts = (
        df.groupby(['Step','State'])
          .size()
          .unstack(fill_value=0)
          .reindex(columns=states, fill_value=0)
          .reset_index()
    )
    pivot = df.pivot_table(index='Step', columns='Site', values='State', aggfunc='first')
    cmap_map = {'U':0,'M':1,'P':2,'A':3,'Ub':4}
    heat = pivot.applymap(lambda x: cmap_map.get(x, np.nan))

    # 5) Plot
    fig, (ax1, ax2) = plt.subplots(1,2,figsize=(16,8))
    sns.heatmap(heat, cmap=ListedColormap(['grey','blue','green','red','yellow']),
                cbar=False, ax=ax1, vmin=0, vmax=4)
    ax1.set(title="Histonzustände über die Zeit", xlabel="Site", ylabel="Step")

    for s in states:
        if s in state_counts:
            ax2.plot(state_counts['Step'], state_counts[s], marker='o', label=s)
    ax2.set(title="Anzahl der Histonzustände", xlabel="Step", ylabel="Count")
    ax2.legend(title="State")
    ax2.grid(alpha=0.3)

    plt.tight_layout()
    plt.savefig("./Paper/Images/Histone_Prohaska_and_Markov_1000.png", dpi=300)
    plt.show()

if __name__=='__main__':
    plot_histone_states("Histone_Prohaska_and_Markov_1000.csv")
