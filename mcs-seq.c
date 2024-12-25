#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // For INT_MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b)) // Macro for minimum

int main() {
    int x[] = {72, 8, -47, -100, 72};
    int n = sizeof(x) / sizeof(x[0]);

    int s[n], m[n], sm[n], indx[n]; // Arrays for input, prefix-sum, prefix-min, sm, and indices

    // BASE CASE
    s[0] = x[0];        // Prefix sum for the first element
    m[0] = s[0];        // First prefix-minimum is the first prefix-sum
    sm[0] = s[0];       // sm[0] = s[0] since m[-1] doesn't exist
    indx[0] = 0;        // The prefix minimum at position 0 occurs at index 0

    // Step 3: Compute s[], m[], sm[], and indx[] in a single for loop
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

    // Step 4: Find MCS value and its indices
    int MCS = INT_MIN;   // Maximum sm[j]
    int end_index = 0;   // End index of the subarray
    for (int j = 0; j < n; j++) {
        if (sm[j] > MCS) {
            MCS = sm[j];
            end_index = j;
        }
    }

    // Start index is determined using indx[end_index]
    int start_index = indx[end_index] + 1;

    // Step 5: Print the results
    printf("\nwhere the MCS value is max(sm[j]) = %d, the indices are [%d, %d].\n", MCS, start_index, end_index);

    // Print s[], m[], sm[], and indx[] for clarity
    printf("\nIndex\tPrefix-Sum (s)\tPrefix-Min (m)\tsm[j]\tindx\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t%d\n", i, s[i], m[i], sm[i], indx[i]);
    }
    
    return 0;
}
