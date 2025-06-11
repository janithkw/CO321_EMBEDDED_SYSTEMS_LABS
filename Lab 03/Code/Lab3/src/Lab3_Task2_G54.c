// Try to increase the delay in Exercise 2 to 500ms. Talk to an instructor and explain how this can be done, or why this cannot be done.


/*
XTAL = 16MHz -> Tclock = 1/16MHz = 62.5 ns
Prescaler = 1:1024 → Tcounter_clock = 1024 × 1/16 MHz = 64 μs (Largert Possible Prescaler in Timer0 Normal Mode)


Counter increments needed = 500000μs / 64μs = 7812.5

Maximum possible counter increment = 256
Initial counter value = 1 + 255 - 7812.5 = -7556.5 (not possible, as initial counter value must be between 0 and 255)
*/