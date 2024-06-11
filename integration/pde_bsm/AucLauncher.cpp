#include <iostream>

#include "../intergartor/Interator.hpp"
#include "AucRHS.hpp"

#include "solutions/AnalyticalSolution.cpp"
#include "solutions/ExplicitSolution.cpp"
#include "solutions/ImplicitSolution.cpp"

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
        return Explicit::solveExplicit( S_tau_max, tau_max );
      case SolutionApproach::IMPLICIT:
        return Implicit::runSolution( S_tau_max, tau_max );
    }

    throw std::invalid_argument( "Unknown solution approach" );
  }
} // namespace ADAAI::Integration::PDE_BSM
