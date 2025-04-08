// /Users/Maxhi/Medizininformatik/Modellierung biologische Systeme/C Implementation Prohaska/cpg_visualization.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define color codes for terminal output
#define COLOR_RESET "\033[0m"
#define COLOR_UNMETHYLATED "\033[38;5;253m\033[48;5;253m" // pale background
#define COLOR_METHYLATED "\033[38;5;240m\033[48;5;240m"   // dark background
#define COLOR_H "\033[38;5;33m\033[48;5;33m"              // blue for H
#define COLOR_F "\033[38;5;76m\033[48;5;76m"              // green for F
#define COLOR_C "\033[38;5;208m\033[48;5;208m"            // orange for C

// Function to print the matrix view
void print_cpg_matrix(char **states, int num_steps, int num_sites) {
    printf("CpG Site Matrix View:\n");
    
    // Print header with site numbers
    printf("     ");
    for (int j = 0; j < num_sites; j++) {
        printf(" %2d ", j+1);
    }
    printf("\n");
    
    // Print matrix rows
    for (int i = 0; i < num_steps; i++) {
        printf("Step %2d ", i);
        for (int j = 0; j < num_sites; j++) {
            // Select color based on state
            switch(states[i][j]) {
                case 'U': printf("%s   %s", COLOR_UNMETHYLATED, COLOR_RESET); break;
                case 'M': printf("%s   %s", COLOR_METHYLATED, COLOR_RESET); break;
                case 'H': printf("%s   %s", COLOR_H, COLOR_RESET); break;
                case 'F': printf("%s   %s", COLOR_F, COLOR_RESET); break;
                case 'C': printf("%s   %s", COLOR_C, COLOR_RESET); break;
                default:  printf("   "); break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Function to calculate state distribution
void calculate_distribution(char *states, int num_sites, int *counts) {
    // Initialize counts
    memset(counts, 0, 5 * sizeof(int));
    
    // Count occurrences of each state
    for (int i = 0; i < num_sites; i++) {
        switch(states[i]) {
            case 'U': counts[0]++; break;
            case 'M': counts[1]++; break;
            case 'H': counts[2]++; break;
            case 'F': counts[3]++; break;
            case 'C': counts[4]++; break;
        }
    }
}

// Function to print a simple ASCII bar chart
void print_distribution_bars(int step, int *counts, int num_sites) {
    const char *labels[] = {"Unmethylated (U)", "Methylated (M)", "H", "F", "C"};
    const char *colors[] = {COLOR_UNMETHYLATED, COLOR_METHYLATED, COLOR_H, COLOR_F, COLOR_C};
    
    printf("State Distribution at Step %d:\n", step);
    
    for (int i = 0; i < 5; i++) {
        float percentage = (float)counts[i] / num_sites * 100;
        int bar_length = (int)(percentage / 5);  // Scale to reasonable length
        
        printf("%-16s [%3.1f%%] %s", labels[i], percentage, colors[i]);
        for (int j = 0; j < bar_length; j++) {
            printf("█");
        }
        printf("%s\n", COLOR_RESET);
    }
    printf("\n");
}

// Function to export data for external plotting (e.g., with Python)
void export_data_for_plotting(char **states, int num_steps, int num_sites, const char* filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error opening file for writing\n");
        return;
    }
    
    // Write header
    fprintf(fp, "Step,Site,State\n");
    
    // Write data in CSV format
    for (int i = 0; i < num_steps; i++) {
        for (int j = 0; j < num_sites; j++) {
            fprintf(fp, "%d,%d,%c\n", i, j+1, states[i][j]);
        }
    }
    
    // Write aggregate data
    fprintf(fp, "\n\nStep,U,M,H,F,C\n");
    int counts[5];
    for (int i = 0; i < num_steps; i++) {
        calculate_distribution(states[i], num_sites, counts);
        fprintf(fp, "%d,%d,%d,%d,%d,%d\n", i, counts[0], counts[1], counts[2], counts[3], counts[4]);
    }
    
    fclose(fp);
}

// Example usage in main
int main() {
    // Example data: 5 steps, 10 CpG sites
    const int num_steps = 5;
    const int num_sites = 10;
    
    // Allocate and initialize states
    char **states = malloc(num_steps * sizeof(char*));
    for (int i = 0; i < num_steps; i++) {
        states[i] = malloc(num_sites * sizeof(char));
        
        // Generate some example data
        for (int j = 0; j < num_sites; j++) {
            // This is just for demonstration - replace with your actual simulation data
            int r = rand() % 5;
            states[i][j] = r == 0 ? 'U' : (r == 1 ? 'M' : (r == 2 ? 'H' : (r == 3 ? 'F' : 'C')));
        }
    }
    
    // Display matrix view
    print_cpg_matrix(states, num_steps, num_sites);
    
    // Display distribution for each step
    int counts[5];
    for (int i = 0; i < num_steps; i++) {
        calculate_distribution(states[i], num_sites, counts);
        print_distribution_bars(i, counts, num_sites);
    }
    
    // Export data for external plotting
    export_data_for_plotting(states, num_steps, num_sites, "cpg_states.csv");
    
    // Clean up
    for (int i = 0; i < num_steps; i++) {
        free(states[i]);
    }
    free(states);
    
    return 0;
}