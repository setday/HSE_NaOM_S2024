#pragma once

#include "AirDensity.hpp"
#include "DrugCoefficient.hpp"

namespace ADAAI::Integration::Environment
{
  /// \brief Computes the value of Q (given y and v^2)
  float AeroDynamicForce( float y, float v2 )
  {
    static float d = 0.216;              // calibre (diameter) in meters
    static float S = M_PI * d * d / 4.0; // cross-sectional area

    auto AE = AirEnviorment { y };

    float pressure = AE.getPressure();
    float density = AE.getDensity();

    float  M = std::sqrt( v2 * density / pressure );

    return DrugCoefficient { M }() * density * v2 * S / 2.0;
  }
} // namespace ADAAI::Integration::Environment
