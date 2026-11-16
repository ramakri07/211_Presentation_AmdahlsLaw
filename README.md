# The 90/10 Rule of Code Optimization and Amdahl's Law

## The ideas

The 90/10 rule: a program spends about 90% of its run time in about 10% of its code. Code inside loops runs many times, while the code that sets up the program only runs once. In essence, the biggest gains come from optimizing that small "hot" part vs the cold part.

Amdahl's Law: if a fraction p of the run time is sped up by a factor s, the whole program speeds up by the following formula:

Speedup = 1 / ((1 - p) + p / s)

If s theoretically grows without a limit towards infinity, the speedup approaches a ceiling of 1 / (1 - p). The part you don't optimize prevents the gain from exceeding that.

## The demo

amdahl_demo.c times three pieces of code:

1. Cold code: a setup loop that adds up 5 million random numbers, once. I did this because the cold code has to take a visible amount of time (at least a few percent), or else the ceiling wont show up.
2. Hot code, slow: counts the primes below 100,000 by trying every divisor from 2 to n - 1.
3. Hot code, fast: the same count, but stops checking at the square root of n.

It then computes p, s, Amdahl's predicted speedup, and the measured speedup.

## Results (one run)

Cold code (setup): 0.015 s

Hot code, slow: 0.835 s

Hot code, fast: 0.005 s

p (share of time in hot code): 96.4%

s (hot code speedup): 164.6x

Amdahl predicted speedup: 43.11x

Measured speedup: 43.11x

Ceiling, 1/(1 - p): 58.04x

The hot code got about 165x faster, but the whole program got only about 43x faster, because the setup code I didn't touch now takes most of the time. Timings vary by machine, but the pattern holds.

## Finding the hot spot: profilers

You find the hot 10% by measuring instead of guessing, using a profiler. In practice people are usually bad at guessing. There are two common kinds:

Sampling (snapshot) profilers pause the program at regular intervals and record which function it is in. The functions that show up in the most snapshots are where the time goes. They slow the program very little, but the results are estimates, not exact counts. An example is perf on Linux.

Instrumenting (injection) profilers insert extra timing or counting code into the program, for example at every function call. 30 years ago, programmers used to manually inject counts into assembly code. The advantage is the counts are exact, but the added code slows the program and can distort the results. Examples: gprof (compile with -pg) and Valgrind's Callgrind.
