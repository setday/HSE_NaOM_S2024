#include <iostream>

#include "../intergartor/Interator.hpp"
#include "AucRHS.hpp"

#include "solutions/AnalyticalSolution.cpp"
#include "solutions/NumericalSolution.cpp"

namespace ADAAI::Integration::PDE_BSM
{
  enum class SolutionApproach
  {
    ANALYTICAL,
    EXPLICIT,
    IMPLICIT
  };

  double launchAuc( SolutionApproach approach = SolutionApproach::ANALYTICAL )
  {
    int    S_tau_max = 0.9 * AucRHS::K;
    double tau_max   = 1.0;

    switch ( approach )
    {
      case SolutionApproach::ANALYTICAL:
        return Analytical::solveAnalytical( S_tau_max, tau_max );
      case SolutionApproach::EXPLICIT:
        return Numerical::solveNumerical( S_tau_max, tau_max, Numerical::SolutionApproach::EXPLICIT );
      case SolutionApproach::IMPLICIT:
        return Numerical::solveNumerical( S_tau_max, tau_max, Numerical::SolutionApproach::IMPLICIT );
    }

    throw std::invalid_argument( "Unknown solution approach" );
  }
} // namespace ADAAI::Integration::PDE_BSM
