# Results

- [Exp](#Test-results-for-Eulers-number-evaluation)
- [Diff](#Test-Results-for-Derivative-Calculations)
- [Cannon Problem](#Cannon-Problem)
- [Orbital Problem](#Orbital-Problem)
- [BDM PDE](#BSM-PDE)

# Test results for Euler's number evaluation

- Range: [-300.000000, 1000.000000]
- Step: 0.001000

**Maximum Errors by Method**

| Method              | Float (ε)        | Double (ε)      | Long double (ε)  |
|---------------------|------------------|-----------------|------------------|
| **Taylor**          | 32.5538          | 222.972         | 532.001          |
| **Pade**            | 32.7759          | 222.512         | 532.522          |
| **Chebyshev**       | 32.1264          | 222.265         | 3058.99          |
| **ChebyshevUnused** | 32.1264          | 224.236         | 15484.5          |

- Range: [0.000000, 3.141593]
- Step: 0.001000

| Method              | Float (ε)        | Double (ε)      | Long double (ε)  |
|---------------------|------------------|-----------------|------------------|
| **Fourier**         |  4.64926e+06     | 2.49605e+15     | 5.11191e+18      |


# Test Results for Derivative Calculations

## Derivative: X

| Method        | x  | y  | Result               | Absolute Error          | True Value             |
|---------------|----|----|----------------------|-------------------------|------------------------|
| Stencil3      | 3  | 1  | -12.3968759605819    | 6.72761807862798e-06    | -12.3968826882         |
| Stencil3Extra | 3  | 1  | -12.39688268819      | 9.97957272375061e-12    |                        |
| Stencil5      | 3  | 1  | -12.3968826882651    | 6.5053740172516e-11     |                        |
| Stencil5Extra | 3  | 1  | -12.3968826881653    | 3.47437634218295e-11    |                        |
| FwdAAD        | 3  | 1  | -12.3968826882308    | 3.07647241015729e-11    |                        |

## Derivative: XY

| Method        | x  | y  | Result               | Absolute Error          | True Value             |
|---------------|----|----|----------------------|-------------------------|------------------------|
| Stencil3      | 3  | 1  | -31.6066898931044    | 2.41471955568784e-05    | -31.6067140403         |
| Stencil3Extra | 3  | 1  | -31.606714044156     | 3.85597331842291e-09    |                        |
| Stencil5      | 3  | 1  | -31.6067140423056    | 2.00559568952485e-09    |                        |
| Stencil5Extra | 3  | 1  | -31.6067140472399    | 6.9399206381604e-09     |                        |
| FwdAAD        | 3  | 1  | -31.6067140403415    | 4.1456615917923e-11     |                        |

## Derivative: Y

| Method        | x    | y    | Result               | Absolute Error           | True Value             |
|---------------|------|------|----------------------|--------------------------|------------------------|
| Stencil3      | 45.2 | 4653 | 44.931651765201      | 0.000152973199021744     | 44.9318047384          |
| Stencil3Extra | 45.2 | 4653 | 44.9318041819498     | 5.5645017482675e-07      |                        |
| Stencil5      | 45.2 | 4653 | 44.9318047357809     | 2.61907473486644e-09     |                        |
| Stencil5Extra | 45.2 | 4653 | 44.931803997339      | 7.41061036535484e-07     |                        |
| FwdAAD        | 45.2 | 4653 | 44.9318047384128     | 1.2768452961609e-11      |                        |

## Derivative: YY

| Method        | x    | y    | Result               | Absolute Error          | True Value             |
|---------------|------|------|----------------------|-------------------------|------------------------|
| Stencil3      | 45.2 | 4653 | -222.265875104988    | 0.00327127401226335     | -222.269146379         |
| Stencil3Extra | 45.2 | 4653 | -222.270105232347    | 0.000958853347214017    |                        |
| Stencil5      | 45.2 | 4653 | -222.265530007426    | 0.00361637157394057     |                        |
| Stencil5Extra | 45.2 | 4653 | -222.271630287584    | 0.0024839085836561      |                        |
| FwdAAD        | 45.2 | 4653 | -222.269146378964    | 3.60103058483219e-11    |                        |

## Derivative: XY

| Method        | x    | y    | Result               | Absolute Error          | True Value             |
|---------------|------|------|----------------------|-------------------------|------------------------|
| Stencil3      | 45.2 | 4653 | -22063.2285087174    | 816.714102182556        | -22879.9426109         |
| Stencil3Extra | 45.2 | 4653 | -22877.724512782     | 2.21809811797721        |                        |
| Stencil5      | 45.2 | 4653 | -22845.0981524205    | 34.8444584795143        |                        |
| Stencil5Extra | 45.2 | 4653 | -22888.5999662408    | 8.65735534080159        |                        |
| FwdAAD        | 45.2 | 4653 | -22879.9426067433    | 4.15671092923731e-06    |                        |


# Cannon Problem

**Best angle: 52 | Best distance: 158234**

Data visualization:

![Cannon_best_angle_plot](https://github.com/setday/HSE_NaOM_S2024/assets/78466953/69038f79-3f37-4768-a9c5-791aa7b16d9a)


# Orbital Problem

![satellite](https://github.com/setday/HSE_NaOM_S2024/assets/78466953/95ce7f02-38d6-44e4-a364-05bd344b90c7)

# BSM PDE

**Premium Calculation Methods**

| Method              | Premium  |
|---------------------|----------|
| Analytical solution | 9.86092  |
| Explicit method     | 10.442   |
| Implicit method     | 20.4855  |
