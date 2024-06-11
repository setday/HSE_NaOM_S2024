#include <fstream>
#include <iostream>
#include <vector>

#include "../intergartor/Interator.hpp"
#include "../intergartor/steppers/RFK45_TimeStepper.hpp"
#include "AucRHS.hpp"

#include "solutions/AnalyticalSolution.cpp"
#include "solutions/ExplicitSolution.cpp"

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
        return solveAnalytical( S_tau_max, tau_max );
      case SolutionApproach::EXPLICIT:
        return solveExplicit( S_tau_max, tau_max );
      case SolutionApproach::IMPLICIT:
        return 0.0;
    }

    throw std::invalid_argument( "Unknown solution approach" );
  }
} // namespace ADAAI::Integration::PDE_BSM
