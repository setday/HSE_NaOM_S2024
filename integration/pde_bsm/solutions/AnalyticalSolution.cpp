#include <math.h>
#include <utility>

#include "../AucRHS.hpp"
#include <gsl/gsl_sf.h>
#include <gsl/gsl_sf_erf.h>

namespace ADAAI::Integration::PDE_BSM::Analytical
{
  /// @brief r_tau
  /// @param tau
  /// @return
  double solveAnalytical( double S_tau_max, double tau_max )
  {
    double V       = AUX_FUNC::get_sigma2_integral( 1 );
    double d_main  = std::log( S_tau_max / AucRHS::K ) + AUX_FUNC::risk_free_interest_rate_function( tau_max );
    double d_plus  = ( d_main + V / 2.0 ) / std::sqrt( V );
    double d_minus = ( d_main - V / 2.0 ) / std::sqrt( V );

    return S_tau_max * ( 1 - gsl_sf_erf_Q( d_plus ) ) - AucRHS::K * std::exp( -AUX_FUNC::risk_free_interest_rate_function( tau_max ) ) * ( 1 - gsl_sf_erf_Q( d_minus ) );
  }
} // namespace ADAAI::Integration::PDE_BSM::Analytical
