import pandas as pd
import matplotlib.pyplot as plt

def calculate_equilibrium_step(csv_file):
    try:
        # CSV mit Spalten: Step, Site, State
        df = pd.read_csv(csv_file, dtype=str)
        df = df[df['Step'].str.isnumeric()]
        df['Step'] = df['Step'].astype(int)

        # Duplikate entfernen: pro (Step, Site) nur der letzte Eintrag
        df = df.drop_duplicates(subset=['Step', 'Site'], keep='last')

        # Pivot: Zeile = Step, Spalten = Site, Werte = State
        pivot = df.pivot(index='Step', columns='Site', values='State').sort_index()

        prev_row = None
        for step, row in pivot.iterrows():
            if prev_row is not None and row.equals(prev_row):
                return step
            prev_row = row
        return None
    except Exception as e:
        print(f"Error while calculating equilibrium step: {e}")
        return None

def plot_state_counts_and_equilibrium(csv_file):
    try:
        df = pd.read_csv(csv_file, dtype=str)
        df = df[df['Step'].str.isnumeric()]
        df['Step'] = df['Step'].astype(int)

        # Duplikate entfernen: pro (Step, Site) nur der letzte Eintrag
        df = df.drop_duplicates(subset=['Step', 'Site'], keep='last')

        # Zähle pro Step die Häufigkeit jedes Zustands
        state_counts = df.groupby(['Step', 'State']).size().unstack(fill_value=0)
        eq_step = calculate_equilibrium_step(csv_file)

        plt.figure(figsize=(8, 5))
        for state in state_counts.columns:
            plt.plot(state_counts.index, state_counts[state], marker='o', label=state)
        if eq_step is not None:
            plt.axvline(eq_step, color='red', linestyle='--', label=f'Equilibrium at step {eq_step}')
        plt.xlabel('Step')
        plt.ylabel('Anzahl CpG pro Zustand')
        plt.title('Ruhezustands-Stabilität (Equilibrium) bei CpG-Modell')
        plt.legend()
        plt.grid(alpha=0.3)
        plt.tight_layout()
        plt.show()

        print(f'Equilibrium reached at step: {eq_step}')
    except FileNotFoundError:
        print(f"Error: File '{csv_file}' not found.")
    except Exception as e:
        print(f"Error while plotting state counts and equilibrium: {e}")

if __name__ == "__main__":
    plot_state_counts_and_equilibrium("cpg_states_stoch_det.csv")
