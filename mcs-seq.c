#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MIN, to be used *MCS*
#include <time.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_SIZE 300000000

void generate_random_array(int *array, int size) {

    /*array[0] = -2;
     *   array[1] = 1;
     *   array[2] = 3;
     *   array[3] = -7;
     *   array[4] = 11;
     *   array[5] = -2;
     *   array[6] = -6;
     *   array[7] = 12;
     *   array[8] = -3;
     *   array[9] = -1;*/

    for (int i = 0; i < size; i++) {
        array[i] = rand() % 201 - 100;  // Random numbers between -100 and 100
    }
}

double solve_mcs(int *x, int n) {

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
    clock_t start_time_for_s_m_sm = clock();
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
    clock_t end_time_for_s_m_sm = clock();
    double calculation_time_for_s_m_sm = (double)(end_time_for_s_m_sm - start_time_for_s_m_sm) / CLOCKS_PER_SEC;

    int MCS = INT_MIN;   // Maximum sm[j]
    int end_index = 0;   // End index of the subarray
    clock_t start_time_for_mcs = clock();
    for (int j = 0; j < n; j++) {
        if (sm[j] > MCS) {
            MCS = sm[j];
            end_index = j;
        }
    }
    clock_t end_time_for_mcs = clock();
    double calculation_time_for_mcs = (double)(end_time_for_mcs - start_time_for_mcs) / CLOCKS_PER_SEC;

    int start_index = indx[end_index] + 1;

    printf("\nwhere the MCS value is max(sm[j]) = %d, the indices are [%d, %d].\n", MCS, start_index, end_index);

    free(s);
    free(m);
    free(sm);
    free(indx);

    return calculation_time_for_s_m_sm + calculation_time_for_mcs;
}

int main() {
    int *x = malloc(ARRAY_SIZE * sizeof(int));
    if (!x) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    generate_random_array(x, ARRAY_SIZE);

    printf("Testing with array size: %d\n\n", ARRAY_SIZE);

    double execution_time = solve_mcs(x, ARRAY_SIZE);
    printf("Calculation time: %.4f seconds\n\n", (execution_time));

    free(x);

    return 0;
}
