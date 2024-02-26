#pragma once

#include "../../utils/Consts.hpp"

#include <gsl/gsl_fft_halfcomplex.h>

#include "../../utils/Consts.hpp"

namespace ADAAI::Exp::Core::Fourier
{

  const int N = 32 - 1; // number of terms for the Fourier series, should be equal to 2^n - 1

  namespace unused
  {
    /// \brief Computes the k-th coefficient for the Fourier series.
    /// \param k - Index of the coefficient.
    /// \return The computed coefficient a.
    constexpr double get_a( int k )
    {
      return CONST::TWO_OVER_PI * ( CONST::EXP_OF_PI * ( k * std::sin( M_PI * k ) + std::cos( M_PI * k ) ) - 1 ) / ( k * k + 1 );
    }

    /// @warning This method is deprecated, please, use FFT instead
    /// \brief Computes the value of the Fourier series for the exponential function.
    /// \param x - The input value.
    /// \param N - The number of terms in the series (default is 32).
    /// \return The computed value of the Fourier series.
    template<typename T>
      requires std::is_floating_point_v<T>
    constexpr T Exp_Fourier( T x )
    {
      double value = get_a( 0 ) / 2.0;

      for ( int k = 1; k <= N; ++k )
      {
        value += ( get_a( k ) * std::cos( k * x ) );
      }

      return value;
    }

  } // namespace unused

  double points[N + 2];                     // points to compute Chebyshev in
  double scale[N + 2];                      // exp(arccos(points[i]))
  double nodes[N + 2];                      // points at which we will evaluate exp(points) (unused in further computations)
  double chebyshev[N + 1][N + 1] = { 0.0 }; // chebyshev[i][k] is k-th coefficient of the i-th chebyshev polynomial
                                            // nodes = PI * i / (N + 1) where i = 0...N
  double a[N + 2]      = { 0 };             // coefficients for FFT
  double result[N + 2] = { 0 };             // result[i] = exp(nodes[i]) calculated with Chebyshev-Gauss quadrature

  /// \brief Computes the nodes (nodes values)
  inline void initialize_points()
  {
    for ( int i = 1; i <= N + 1; ++i )
    {
      points[i] = std::cos( M_PI * ( 2.0 * i - 1 ) / ( 2.0 * ( N + 1 ) ) );
    }
  }

  /// \brief Computes \exp(\arccos(nodes)) for each point.
  inline void initialize_scale()
  {
    for ( int i = 1; i <= N + 1; ++i )
    {
      scale[i] = std::exp( std::acos( points[i] ) );
    }
  }

  /// \brief Computes nodes[i].
  void initialize_nodes()
  {
    for ( int i = 0; i <= N; ++i )
    {
      nodes[i] = M_PI * i / ( N + 1 );
    }
  }

  inline void initialize_chebyshev_polynomials()
  {
    chebyshev[0][0] = chebyshev[1][1] = 1;

    for ( int order = 2; order <= N; ++order )
    {
      for ( int i = order; i > 0; i-- )
      {
        chebyshev[order][i] = 2 * chebyshev[order - 1][i - 1] - chebyshev[order - 2][i];
      }
      chebyshev[order][0] = -chebyshev[order - 2][0];
    }
  }

  /// \brief Computes the k-th Chebyshev polynomial at a given point
  /// \example \code computeChebyshev(3, 0.5); \endcode
  /// \param k - Degree of the Chebyshev polynomial
  /// \param x - Point at which to evaluate the Chebyshev polynomial
  /// \return The value of the k-th Chebyshev polynomial at the given point points
  double compute_Chebyshev( int k, double x )
  {
    double x_pow = 1;
    double res   = 0;
    for ( int i = 0; i <= k; i++ )
    {
      res += chebyshev[k][i] * x_pow;
      x_pow *= x;
    }
    return res;
  }

  /// \brief Initializes coefficients a[k] for k in [0, N] the Fourier series using Chebyshev-Gauss quadrature
  void initialize_a()
  {
    for ( int k = 0; k <= N; ++k )
    {
      for ( int i = 1; i <= N + 1; ++i )
      {
        a[k] += compute_Chebyshev( k, points[i] ) * scale[i];
      }
      a[k] *= 2.0 / ( N + 1 );
    }
  }

  /// \brief initializes FFT method
  void FFT()
  {
    initialize_points();
    initialize_nodes();
    initialize_scale();
    initialize_chebyshev_polynomials();
    initialize_a();

    // Change first coefficient to be a_0 / 2 (as an additive constant to sum on 1...N)
    a[0] /= 2.0;

    size_t a_count  = N + 1;       // number of elements in a
    size_t el_count = a_count * 2; // number of coefficient in the complex array (double as many as a)
    // using sum( a[k] * e^(-pi * I * j * k / (N + 1) ) ) over k = 0...N that is different from gsl implementation sum( a[k] * exp(-2 * pi * I * j * k / ( N + 1 ) ) )
    // so we should double the number of elements in the complex array and fill the second half with zeros

    // Initialize the real array with the coefficients a
    for ( size_t i = 0; i < a_count; ++i )
    {
      result[i] = a[i];
    }

    // Perform the forward FFT
    gsl_fft_real_radix2_transform( result, 1, el_count );
  }

  static bool is_initialized = false;

  /// @warning now only suitable for float and double
  /// \brief Computes exp(x) using the Fourier series approximation.
  /// \example \code Exp_Fourier( 0.1 ); \endcode
  /// \tparam T - Floating point type
  /// \param x - Value to compute
  /// \return e^x approximation
  template<typename T>
    requires std::is_floating_point_v<T>
  constexpr T Exp_Fourier( T x )
  {
    if ( !is_initialized )
    {
      FFT();
      is_initialized = true;
    }

    size_t i          = 0;
    double prev_delta = std::abs( x - result[i] );
    for ( ; i <= N; ++i )
    {
      double delta = std::abs( x - result[i] );

      if ( delta > prev_delta )
      {
        break;
      }

      prev_delta = delta;
    }

    return result[i];
  }
} // namespace ADAAI::Exp::Core::Fourier