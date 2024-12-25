#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#include <time.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
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


    int sum = 0;
    int s[n];           // Prefix-sum array
    int m[n];           // Prefix-minimum array
    int s_m[n];         // Maximum sum array := s_m[j] = s[j] - m[j-1]
    int indx[n];

    s[0] = x[0];        // Prefix maximum at the first position
    int r = x[0];
    #pragma omp parallel for reduction (inscan, +: r)
    for (int i = 1; i < n; i++) {
        r += x[i];
        #pragma omp scan inclusive(r)
        s[i] = r;
    }

    m[0] = s[0];        // Prefix minimum at the first position. Actually dependency occurs if I define it like ```m[0] = s[0]```.
    int t = s[0];
    #pragma omp parallel for reduction(inscan, min: t)
    for (int i = 1; i < n; i++) {
        t = MIN(t, s[i]);

        #pragma omp scan inclusive(t)
        m[i] = t;
    }

    // Compute s_m[] where s_m[i] = s[i] − m[i−1]
    s_m[0] = x[0];
    #pragma omp parallel for
    for (int i = 1; i < n; i++) {
        s_m[i] = s[i] - m[i-1];
    }

    indx[0] = 0;
    int indx_val = 0;
    #pragma omp parallel for reduction (inscan, max: indx_val)
    for (int i = 1; i < n; i++) {
        if (s[i] < m[i-1]) {
            indx_val = i;
        }

        #pragma omp scan inclusive(indx_val)
        indx[i] = indx_val;
    }

    int MCS = INT_MIN;
    #pragma omp parallel for reduction(max: MCS)
    for (int i = 0; i < n; i++) {
        if (s_m[i] > MCS) {
            MCS = s_m[i];
        }
    }

    int end_index = 0;
    #pragma omp parallel for reduction(+: end_index)
    for (int i = 0; i < n; i++) {
        if (MCS == s_m[i]) {
            end_index = i;
        }
    }


    int start_index = indx[end_index] + 1;

    /*printf("Array of X = [");
    for(int i = 0; i < n; i++) {
        if (i != (n-1)) {
            printf("%d, ", x[i]);
        } else {
            printf("%d", x[i]);
        }
    }
    printf("]");
*/
    printf("\nwhere the MCS value is max(sm[j]) = %d, the indices are [%d, %d].\n", MCS, start_index, end_index);

    /*
    printf("\nIndex\tPrefix-Sum (s)\tPrefix-Min (m)\ts_m\tindx\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t%d\n", i, s[i], m[i], s_m[i], indx[i]);
    }

    printf("s[i]:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", s[i]);
   }
    printf("\n");

    printf("m[i]:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", m[i]);   
    }
    printf("\n");

    printf("s_m[i]:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", s_m[i]);   
    }
    printf("\n");

    printf("indx[i]:\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t", indx[i]);   
    }
    printf("\n");
    printf("%d\tStart: %d - End: %d\n", MCS, start_index, end_index);
*/
    return 0;
}
