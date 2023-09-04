#include <stdio.h>
#include <string.h>
#define N 4
#define Size 22
#define Count 14
#ifndef max
    #define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
    #define min(a, b) ((a) > (b) ? (b) : (a))
#endif

char result[20000000][35];
unsigned long long n_results = 0;

char buffer[Size + 1];
void print_hands(int n, int k, char *start, char *end) {
    if (k == 0) {
        /*
        while (n) {
            *end++ = '0';
            --n;
        }
        *end++ = 0;
        memcpy(result[n_results], start, end-start);
        */
        n_results++;
        return;
    }
    if (n == 0) {
        return;
    }
    for (int i = min(N, k); i >= 0; --i) {
        //*end = '0' + i;
        print_hands(n-1, k-i, start, end+1);
    }
}


int main() {
    print_hands(Size, Count, buffer, buffer);
    //for (int i = 0; i < n_results; ++i) {
    //    puts(result[i]);
    //}
    printf("n = %llu\n", n_results);
    return 0;
}
