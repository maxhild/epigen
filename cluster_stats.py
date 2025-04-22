import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

def cluster_size_distribution(csv_file, state='M'):
    # Einlesen und Bereinigung
    df = pd.read_csv(csv_file, dtype=str)
    df = df[df['Step'].str.isnumeric()]
    df['Step'] = df['Step'].astype(int)
    df = df.drop_duplicates(subset=['Step','Site'], keep='last')
    pivot = df.pivot(index='Step', columns='Site', values='State').sort_index()
    
    # Berechne Cluster-Längen für jeden Step
    clusters = []
    for step, row in pivot.iterrows():
        vals = row.values
        current = None
        length = 0
        for v in vals:
            if v == state:
                length += 1
            else:
                if length > 0:
                    clusters.append(length)
                length = 0
        if length > 0:
            clusters.append(length)
    
    # Plot Distribution
    plt.figure(figsize=(6,4))
    plt.hist(clusters, bins=range(1, max(clusters)+2), align='left', density=True)
    plt.xlabel('Clustergröße')
    plt.ylabel('Frequenz')
    plt.title(f'Clustergrößenverteilung für Zustand {state}')
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.show()
    plt.savefig("cluster_size_distribution_stoch_det.png", dpi=300)
    
    # Rückgabe der Verteilung
    return pd.Series(clusters).value_counts().sort_index()

def spatial_autocorrelation(csv_file, state='M', max_dist=10):
    df = pd.read_csv(csv_file, dtype=str)
    df = df[df['Step'].str.isnumeric()]
    df['Step'] = df['Step'].astype(int)
    df = df.drop_duplicates(subset=['Step','Site'], keep='last')
    pivot = df.pivot(index='Step', columns='Site', values='State').sort_index()

    # Wandle in binäre Matrix um: 1 wenn state, sonst 0
    mat = (pivot == state).astype(int).values
    n_steps, n_sites = mat.shape

    # Autokorrelation für Distanzen 1..max_dist
    ac = []
    for d in range(1, max_dist + 1):
        corrs = []
        for i in range(n_sites - d):
            x = mat[:, i]
            y = mat[:, i + d]
            # Prüfen, ob beide Vektoren Varianz besitzen
            if x.any() and (x != x[0]).any() and y.any() and (y != y[0]).any():
                corr = np.corrcoef(x, y)[0, 1]
                corrs.append(corr)
        ac.append(np.nanmean(corrs) if corrs else 0.0)  # 0 statt nan

    # Plot
    plt.figure(figsize=(6,4))
    plt.plot(range(1, max_dist+1), ac, 'o-')
    plt.xlabel('Entfernung d')
    plt.ylabel('Autokorrelation')
    plt.title(f'Räumliche Autokorrelation für Zustand {state}')
    plt.grid(alpha=0.3)
    plt.tight_layout()
    plt.show()
    plt.savefig("spatial_autocorrelation_stoch.png", dpi=300)

    return pd.Series(ac, index=range(1, max_dist+1))


cluster_dist = cluster_size_distribution("cpg_states_stoch_det.csv", state='U')
autocorr = spatial_autocorrelation("cpg_states_stoch_det.csv", state='U')
