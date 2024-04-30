#pragma once

#include "AirDensity.hpp"
#include "DrugCoefficient.hpp"

namespace ADAAI::Integration::Environment
{
  /// \brief Computes the value of Q (given y and v^2)
  double AeroDynamicForce( double y, double v2, double S )
  {
    auto AE = AirEnvironment { y };

    double pressure = AE.getPressure();
    double density  = AE.getDensity();

    double M = std::sqrt( v2 * density / pressure );

    return DrugCoefficient { M }() * density * v2 * S / 2.0;
  }

  constexpr double Mu = 398600.4f; // Earth's gravitational parameter (km^3/s^2)
  constexpr double Re = 6378.137f; // Earth's radius (km)
  constexpr double J2 = 1.0827e-3; // J2 perturbation coefficient

  void ComputeUGradient( const double* position, double* u_gradient )
  {
    double
        x = position[0],
        y = position[1],
        z = position[2];

    double
        r2 = x * x + y * y + z * z,
        r  = std::sqrt( r2 ),
        r3 = r2 * r,
        r5 = r3 * r2,
        r7 = r5 * r2;

    double u0_gradient = -Mu / r3; // there is forgotten x/y/z in dr/dr replacement

    double u2_coefficient = J2 * Mu * Re * Re / 2.0;
    double
        u2_general_gradient    = -3.0 / r5,                         // there is forgotten x/y/z in dr/d(x/y/z) replacement
        u2_special_xy_gradient = -15.0 * z * z / r7,                // there is forgotten x/y/z in dr/d(x/y/z) replacement
        u2_special_z_gradient  = u2_special_xy_gradient + 6.0 / r5; // there is forgotten z in d(z^2)/dz replacement
    double
        u2_xy_summary = u0_gradient - u2_coefficient * ( u2_special_xy_gradient - u2_general_gradient ),
        u2_z_summary  = u0_gradient - u2_coefficient * ( u2_special_z_gradient - u2_general_gradient );

    u_gradient[0] = u2_xy_summary * x;
    u_gradient[1] = u2_xy_summary * y;
    u_gradient[2] = u2_z_summary * z;
  }
} // namespace ADAAI::Integration::Environment
