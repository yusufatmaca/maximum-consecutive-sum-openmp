#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MIN

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define FILE_NAME "numbers.txt"

void read_numbers_from_file(const char *filename, int **array, int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "%d", size);

    *array = malloc((*size) * sizeof(int));
    if (!*array) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *size; i++) {
        fscanf(file, "%d", &(*array)[i]);
    }

    fclose(file);
}

int main() {
    int *x = NULL;
    int n = 0;
    read_numbers_from_file(FILE_NAME, &x, &n);

    int s[n], m[n], sm[n], indx[n]; // Arrays for input, prefix-sum, prefix-min, sm, and indices

    // BASE CASE
    s[0] = x[0];        // Prefix sum for the first element
    m[0] = s[0];        // First prefix-minimum is the first prefix-sum
    sm[0] = s[0];       // sm[0] = s[0] since m[-1] doesn't exist
    indx[0] = 0;        // The prefix minimum at position 0 occurs at index 0

    for (int i = 1; i < n; i++) {
        s[i] = s[i - 1] + x[i];        // Prefix-sum array
        m[i] = MIN(m[i - 1], s[i]);    // Prefix-minimum array
        sm[i] = s[i] - m[i - 1];       // sm[j] = s[j] - m[j-1]

        // Update indx[]: Track where the prefix minimum occurs
        if (s[i] < m[i - 1]) {
            indx[i] = i;               // New minimum found at position i
        } else {
            indx[i] = indx[i - 1];     // Carry forward the previous index
        }
    }

    int MCS = INT_MIN;   // Maximum sm[j]
    int end_index = 0;   // End index of the subarray
    for (int j = 0; j < n; j++) {
        if (sm[j] > MCS) {
            MCS = sm[j];
            end_index = j;
        }
    }

    int start_index = indx[end_index] + 1;

    printf("\nwhere the MCS value is max(sm[j]) = %d, the indices are [%d, %d].\n", MCS, start_index, end_index);

    
    return 0;
}
