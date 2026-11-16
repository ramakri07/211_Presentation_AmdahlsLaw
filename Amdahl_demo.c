#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * Amdahl_demo.c
 * Demo of the 90/10 rule and Amdahl's Law.
 *
 * Compile: gcc -O0 Amdahl_demo.c -o demo
 * Run:     ./demo
 *
 * -O0 turns off compiler optimization, so the compiler
 * doesn't change the loops being timed.
 */


/* Returns 1 if n is prime, 0 if not.
   SLOW: tries every divisor from 2 up to n-1. */
int is_prime_slow(int n) {
    if (n < 2) return 0;
    for (int d = 2; d < n; d++) {
        if (n % d == 0) return 0;
    }
    return 1;
}

/* Returns 1 if n is prime, 0 if not.
   FAST: stops once d*d > n (any divisor above sqrt(n) pairs with one below it). */
int is_prime_fast(int n) {
    if (n < 2) return 0;
    for (int d = 2; d * d <= n; d++) {
        if (n % d == 0) return 0;
    }
    return 1;
}

/* Seconds elapsed since 'start'. */
double seconds_since(clock_t start) {
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

int main(void) {
    clock_t start;

    /* 1. COLD CODE: setup that runs straight through once. */
    start = clock();
    long sum = 0;
    for (int i = 0; i < 5000000; i++) {
        sum += rand() % 100;
    }
    double cold = seconds_since(start);

    /* 2. HOT CODE, slow version: count the primes below 100,000. */
    start = clock();
    int primes_slow = 0;
    for (int n = 0; n < 100000; n++) {
        primes_slow += is_prime_slow(n);
    }
    double hot_slow = seconds_since(start);

    /* 3. HOT CODE, fast version: same job, faster check. */
    start = clock();
    int primes_fast = 0;
    for (int n = 0; n < 100000; n++) {
        primes_fast += is_prime_fast(n);
    }
    double hot_fast = seconds_since(start);

    /* 4. Amdahl's Law. */
    double p = hot_slow / (cold + hot_slow);   /* share of time in hot code */
    double s = hot_slow / hot_fast;            /* how much faster hot code got */
    double predicted = 1 / ((1 - p) + p / s);
    double measured  = (cold + hot_slow) / (cold + hot_fast);

    printf("Setup sum: %ld | Primes found: %d (slow), %d (fast)\n\n", sum, primes_slow, primes_fast);
    printf("Cold code (setup):       %.3f s\n", cold);
    printf("Hot code, slow version:  %.3f s\n", hot_slow);
    printf("Hot code, fast version:  %.3f s\n\n", hot_fast);
    printf("p (share of time in hot code):  %.1f%%\n", 100 * p);
    printf("s (hot code speedup):           %.1fx\n", s);
    printf("Amdahl predicted speedup:       %.2fx\n", predicted);
    printf("Measured speedup:               %.2fx\n", measured);
    printf("Ceiling, 1/(1-p):               %.2fx\n", 1 / (1 - p));
    return 0;
}
