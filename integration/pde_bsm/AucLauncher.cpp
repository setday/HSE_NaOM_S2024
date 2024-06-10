#include <fstream>
#include <iostream>
#include <vector>

#include "../intergartor/Interator.hpp"
#include "../intergartor/steppers/RFK45_TimeStepper.hpp"
#include "AnalyticalSolution.hpp"
#include "AucRHS.hpp"

namespace ADAAI::Integration::PDE_BSM
{
  double launchAuc()
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
      integrator( state, end_state, 0.0, 1.0, 0.001 );
    }
    catch ( std::exception& e )
    {
      std::cerr << e.what() << std::endl;
    }

    int S_tau_max = 0.9 * AucRHS::K;
    std::cout << "ACTUAL PAYOFF = " << AUX_FUNC::get_price( S_tau_max, 1 ) << '\n';
    return AucFunc::get_c( end_state, S_tau_max );
  }
} // namespace ADAAI::Integration::PDE_BSM
