#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#include <time.h>

#define NUM_COUNT 100   // Number of random numbers to generate
#define MIN_VAL -250    // Minimum value for the random range
#define MAX_VAL 250     // Maximum value for the random range
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int main() {

    int x[] = {40,-141,198,248,124,145,35,196,-40,-68,198,185,16,199,20,-32,209,161,-144,28,-211,-102,187,-123,110,226,94,96,42,120,162,-118,-209,235,-166,202,53,-176,-172,-192,72,-73,123,146,191,-90,-35,-187,-234,42,145,120,132,-209,37,-80,14,-86,-103,69,228,-143,-245,212,205,91,-154,-123,-102,166,92,-149,-57,91,114,-57,-87,-5,-70,49,-89,-246,-110,-209,-47,-216,-16,79,167,97,-66,-97,3,142,-129,-78,10,-149,-29,68,243,-165,22,143,124,-65,-94,72,-19,-21,160,-132,104,-99,-54,189,73,213,-26,-11,-153,-199,-2,-187,139,180,-86,-212,-122,88,-143,-37,-104,-74,20,170,213,167,73,-223,11,57,20,165,-14,-151,-47,-152,167,181,233,144,-164,189,-40,-212,36,-201,232,114,121,94,-16,-71,-126,137,-4,-138,-184,158,167,-146,-153,-21,207,-24,14,113,33,92,210,201,46,206,-172,155,250,239,117,-118,-91,-11,143,18,101,-246,211,-224,51,-19};

    int n = sizeof(x) / sizeof(int);
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
    #pragma omp parallel for reduction (inscan, +: indx_val)
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
