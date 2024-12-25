#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MIN

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define FILE_NAME "numbers.txt"
#define INITIAL_CAPACITY 5000000

void read_numbers_from_file(const char *filename, int **array, int *size) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int capacity = INITIAL_CAPACITY; // Start with a capacity of 3,032,000
    *array = malloc(capacity * sizeof(int));
    if (!*array) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Read numbers from the file
    while (fscanf(file, "%d", &(*array)[count]) == 1) {
        count++;

        // Expand the array only if capacity is exceeded
        if (count >= capacity) {
            capacity = (int)(capacity * 1.5); // Increase capacity by 50%
            *array = realloc(*array, capacity * sizeof(int));
            if (!*array) {
                perror("Memory reallocation failed");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);

    if (count == 0) {
        fprintf(stderr, "Error: No valid numbers found in the file.\n");
        free(*array);
        *array = NULL;
        *size = 0;
        exit(EXIT_FAILURE);
    }

    *size = count; // Update the size with the number of elements read
}

int main() {
    int *x = NULL;
    int n = 0;

    read_numbers_from_file(FILE_NAME, &x, &n);

    int *s = malloc(n * sizeof(int));
    int *m = malloc(n * sizeof(int));
    int *sm = malloc(n * sizeof(int));
    int *indx = malloc(n * sizeof(int));
    if (!s || !m || !sm || !indx) {
        perror("Memory allocation failed for large arrays");
        free(x);
        free(s);
        free(m);
        free(sm);
        free(indx);
        exit(EXIT_FAILURE);
    }

    // BASE CASE
    s[0] = x[0];        // Prefix sum for the first element
    m[0] = s[0];        // First prefix-minimum is the first prefix-sum
    sm[0] = s[0];       // sm[0] = s[0] since m[-1] doesn't exist
    indx[0] = 0;        // The prefix minimum at position 0 occurs at index 0

    // Compute s[], m[], sm[], and indx[]
    for (int i = 1; i < n; i++) {
        s[i] = s[i - 1] + x[i];        // Prefix-sum array
        m[i] = MIN(m[i - 1], s[i]);    // Prefix-minimum array
        sm[i] = s[i] - m[i - 1];       // sm[j] = s[j] - m[j-1]

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

    free(x);
    free(s);
    free(m);
    free(sm);
    free(indx);

    return 0;
}
