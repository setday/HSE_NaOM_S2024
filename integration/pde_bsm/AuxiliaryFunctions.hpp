#include <fstream>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <math.h>

namespace ADAAI::Integration::PDE_BSM::AUX_FUNC
{
  /// @brief The sigma function
  /// @param tau
  /// @return
  double sigma_function( double tau )
  {
    if ( tau < 0.25 )
    {
      return 0.21;
    }
    if ( tau < 0.5 )
    {
      return 0.229;
    }
    if ( tau < 0.75 )
    {
      return 0.29;
    }
    return 0.25;
  }

  constexpr double sigma_max = 0.29; // Sigma maximum

  /// @brief r_tau
  /// @param tau 
  /// @return 
  double risk_free_interest_rate_function( double tau )
  {
    if ( tau <= 0.25 )
    {
      return 0.16;
    }
    if ( tau <= 0.5 )
    {
      return 0.15;
    }
    if ( tau <= 0.75 )
    {
      return 0.14;
    }
    if ( tau <= 1.0 )
    {
      return 0.13;
    }
    return 0.12;
  }

  double get_r_integral( double tau )
  {
    if ( tau <= 0.25 )
    {
      return 0.16 * tau;
    }
    if ( tau <= 0.5 )
    {
      return 0.04 + 0.15 * ( tau - 0.25 );
    }
    if ( tau <= 0.75 )
    {
      return 0.0775 + 0.14 * ( tau - 0.5 );
    }
    return 0.1125 + 0.13 * ( tau - 0.75 );
  }

  double get_sigma2_integral( double tau )
  {
    if ( tau <= 0.25 )
    {
      return 0.21 * 0.21 * tau;
    }
    if ( tau <= 0.5 )
    {
      return 0.011025 + 0.229 * 0.229 * ( tau - 0.25 );
    }
    if ( tau <= 0.75 )
    {
      return 0.02413525 + 0.29 * 0.29 * ( tau - 0.5 );
    }
    return 0.04516025 + 0.25 * 0.25 * ( tau - 0.75 );
  }
} // namespace ADAAI::Integration::PDE_BSM::AUX_FUNC
