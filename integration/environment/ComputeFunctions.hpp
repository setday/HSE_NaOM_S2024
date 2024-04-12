#pragma once

#include "AirDensity.hpp"
#include "DrugCoefficient.hpp"

namespace ADAAI::Integration::Environment
{
  /// \brief Computes the value of Q (given y and v^2)
  double AeroDynamicForce( double y, double v2 )
  {
    static double d = 0.216;              // calibre (diameter) in meters
    static double S = M_PI * d * d / 4.0; // cross-sectional area

    auto AE = AirEnvironment { y };

    double pressure = AE.getPressure();
    double density  = AE.getDensity();

    double M = std::sqrt( v2 * density / pressure );

    return DrugCoefficient { M }() * density * v2 * S / 2.0;
  }
} // namespace ADAAI::Integration::Environment
