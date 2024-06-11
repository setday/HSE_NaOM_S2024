#include <math.h>
#include <utility>

#include "../AucRHS.hpp"

namespace ADAAI::Integration::PDE_BSM
{
  /// @brief r_tau
  /// @param tau
  /// @return
  double solveExplicit( double S_tau_max, double tau_max )
  {
    double state[AucRHS::N];
    double end_state[AucRHS::N];

    AucFunc::initStartCondition( state );

    auto rhs      = AucRHS();
    auto observer = AucObserver();
    auto stepper  = Integrator::Stepper::RFK45_TimeStepper( &rhs );

    auto integrator = Integrator::ODE_Integrator<AucRHS, Integrator::Stepper::RFK45_TimeStepper<AucRHS>>( &stepper, &observer );

    try
    {
      integrator( state, end_state, 0.0, tau_max, tau_max / 1000 );
    }
    catch ( std::exception& e )
    {
      std::cerr << e.what() << std::endl;
    }

    return AucFunc::get_c( end_state, S_tau_max );
  }
} // namespace ADAAI::Integration::PDE_BSM
