#include <math.h>
#include <utility>

#include "../AucRHS.hpp"
#include "ImplicitSolution.cpp"

namespace ADAAI::Integration::PDE_BSM::Numerical
{
  enum class SolutionApproach
  {
    EXPLICIT,
    IMPLICIT
  };

  /// @brief r_tau
  /// @param tau
  /// @return
  double solveNumerical( double S_tau_max, double tau_max, SolutionApproach approach )
  {
    double delta_tau = tau_max / 1000;

    double state[AucRHS::N];
    double end_state[AucRHS::N];

    AucFunc::initStartCondition( state );

    auto rhs      = AucRHS();
    auto observer = AucObserver();

    try
    {
      if ( approach == SolutionApproach::EXPLICIT )
      {
        auto stepper    = Integrator::Stepper::RFK45_TimeStepper( &rhs );
        auto integrator = Integrator::ODE_Integrator<AucRHS, Integrator::Stepper::RFK45_TimeStepper<AucRHS>>( &stepper, &observer );

        integrator( state, end_state, 0.0, tau_max, delta_tau );
      }
      else
      {
        auto stepper    = Implicit::ImplicitStepper( &rhs );
        auto integrator = Integrator::ODE_Integrator<AucRHS, Implicit::ImplicitStepper<AucRHS>>( &stepper, &observer );

        integrator( state, end_state, 0.0, tau_max, delta_tau );
      }
    }
    catch ( std::exception& e )
    {
      std::cerr << e.what() << std::endl;
    }

    return AucFunc::get_c( end_state, S_tau_max );
  }
} // namespace ADAAI::Integration::PDE_BSM::Numerical
