# Numerical and optimization methods seminar project

This repository contains the code for the seminar project of the course "Numerical and optimization methods" at the **HSE**. The project is about the implementation of a numerical method to approximate the exponential function and the solution of a differential equation.

![GitHub](https://img.shields.io/badge/license-MIT-green)
![GitHub top language](https://img.shields.io/badge/C/C%2B%2B-99.5%25-blue)
![GitHub top language](https://img.shields.io/badge/CMake-0.5%25-blue)

## The project is divided into two parts:
1. The implementation of the numerical method for exponential finding.
2. The implementation of the numerical method for differential function solving.

### Exp approximation:
``` c
=== Test case 1 ===
=> Test data: Range check in [-300.000000, 1000.000000] with step 0.001000

-> Method used: Taylor

=> Max errors:
==> float:       32.5538 * exp
==> double:      222.972 * exp
==> long double: 532.001 * exp
=> Execution time: 1.231s
===--===---===---===--===


=== Test case 2 ===
=> Test data: Range check in [-300.000000, 1000.000000] with step 0.001000

-> Method used: Pade

=> Max errors:
==> float:       32.7759 * exp
==> double:      222.512 * exp
==> long double: 532.522 * exp
=> Execution time: 1.057s
===--===---===---===--===


=== Test case 3 ===
=> Test data: Range check in [-300.000000, 1000.000000] with step 0.001000

-> Method used: ChebyshevUnused

=> Max errors:
==> float:       32.1264 * exp
==> double:      224.236 * exp
==> long double: 15415.4 * exp
=> Execution time: 26.768s
===--===---===---===--===

=== Test case 4 ===
=> Test data: Range check in [-300.000000, 1000.000000] with step 0.001000

-> Method used: Chebyshev

=> Max errors:
==> float:       32.1264 * eps
==> double:      222.265 * eps
==> long double: 3058.24 * eps
=> Execution time: 34.036s
===--===---===---===--===
```

### Differential equation solving:
``` c
=> No test cases implemented yet <=
```
