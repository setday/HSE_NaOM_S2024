#pragma once

#include "../../utils/Consts.hpp"

#include <gsl/gsl_fft_halfcomplex.h>

#include "../../utils/Consts.hpp"

namespace ADAAI::Exp::Core::Fourier
{

  // STEP 1: finding coefficients (a_k) analytically (integration by parts)
  const int N = 31; // number of terms for the Fourier series.

  /// \brief Computes the k-th coefficient for the Fourier series.
  /// \param k - Index of the coefficient.
  /// \return The computed coefficient a_k.
  double get_a_k_analytically( int k )
  {
    // Formula for a_k in the Fourier series
    return CONST::TWO_OVER_PI * ( CONST::EXP_OF_PI * ( k * std::sin( M_PI * k ) + std::cos( M_PI * k ) ) - 1 ) / ( k * k + 1 );
  }

  /// \brief Computes the value of the Fourier series for the exponential function.
  /// \param x - The input value.
  /// \param N - The number of terms in the series (default is 32).
  /// \return The computed value of the Fourier series.
  double fourier_series_for_exp( double x )
  {
    double value = get_a_k_analytically( 0 ) / 2.0; // Initializing with a_0, the constant term

    // Summing up terms in the Fourier series
    for ( int k = 1; k <= N; ++k )
    {
      value += ( get_a_k_analytically( k ) * std::cos( k * x ) ); // Adding each term in the series
    }

    return value; // Returning the final computed value of the Fourier series
  }

  // STEP 2: finding coefficients (a_k) using Chebyshev–Gauss quadrature

  double x_i_values[N + 2]; // nodes

  /// \brief Calculates the nodes (x_i values)
  /// The nodes are calculated using the formula:
  /// x_i = \cos\frac{\pi(2i-1)}{2(N+1)} for each i in the range [1, N + 1].
  /// The nodes are then stored in the array x_i_values.
  void calculate_the_nodes()
  {
    // Loop through i values from 1 to N
    for ( int i = 1; i <= N + 1; ++i )
    {
      // Calculate the node x_i using the specified formula
      x_i_values[i] = std::cos( M_PI * ( 2.0 * i - 1 ) / ( 2.0 * ( N + 1 ) ) );
    }
  }

  double exp_of_acos_of_x_i_values[N + 2]; // exp(arccos(x_i))

  /// \brief Calculates the exponential of the arccosine of each node x_i.
  ///
  /// For each node x_i in the range [1, N + 1], it computes:
  /// \exp(\arccos(x_i)) and stores the result in exp_of_acos_of_x_i_values.
  void calculate_exp_of_acos_of_x_i()
  {
    calculate_the_nodes();

    // Loop through i values from 1 to N
    for ( int i = 1; i <= N + 1; ++i )
    {
      // Calculate exp(acos(x_i)) and store the result
      exp_of_acos_of_x_i_values[i] = std::exp( std::acos( x_i_values[i] ) );
    }
  }

  double chebyshev_polynomials[N + 1][N + 1] = { 0.0 };

  // chebyshev_polynomials[i][k] - k-th coefficient of the i-th chebyshev polynomial

  inline void construct_chebyshev_polynomials()
  {
    chebyshev_polynomials[0][0] = 1;
    chebyshev_polynomials[1][1] = 1;

    for ( int order = 2; order <= N; ++order )
    {
      for ( int i = order; i > 0; i-- )
      {
        chebyshev_polynomials[order][i] = 2 * chebyshev_polynomials[order - 1][i - 1] - chebyshev_polynomials[order - 2][i];
      }
      chebyshev_polynomials[order][0] = -chebyshev_polynomials[order - 2][0];
    }
  }

  /// \brief Computes the k-th Chebyshev polynomial at a given point
  /// \example \code computeChebyshev(3, 0.5); \endcode
  /// \tparam k - Degree of the Chebyshev polynomial
  /// \tparam x - Point at which to evaluate the Chebyshev polynomial
  /// \return The value of the k-th Chebyshev polynomial at the given point x
  double compute_Chebyshev( int k, double x )
  {
    double x_to_the_power_of_i = x;
    double res                 = chebyshev_polynomials[k][0];
    for ( int i = 1; i <= k; i++ )
    {
      res += chebyshev_polynomials[k][i] * x_to_the_power_of_i;
      x_to_the_power_of_i *= x;
    }
    return res;
  }

  /// \brief Computes the k-th coefficient for the Fourier series using Chebyshev-Gauss quadrature
  /// \param k - Index of the coefficient.
  /// \return The computed coefficient a_k.
  double
  get_a_k_using_Chebyshev_Gauss_quadrature( int k )
  {
    double a_k = 0;
    for ( int i = 1; i <= N + 1; ++i )
    {
      a_k += compute_Chebyshev( k, x_i_values[i] ) * exp_of_acos_of_x_i_values[i];
    }
    a_k *= 2.0 / ( N + 1 );
    return a_k;
  }

  // STEP 3: using FFT to compute the sum

  double x_i[N + 2];    // points at which we will evaluate exp(x)
                        // x_i = PI * i / (N + 1) where i = 0...N
  double a_k[N + 2];    // our coefficients that we previously found
  double result[N + 2]; // result[i] = exp(x[i])

  /// \brief evaluates x[i]
  void initialize_points_x_i()
  {
    for ( int i = 0; i <= N; ++i )
    {
      x_i[i] = M_PI * i / ( N + 1 );
    }
  }

  /// \brief evaluates a[i]
  void initialize_coefficients_a_k()
  {
    calculate_exp_of_acos_of_x_i();
    construct_chebyshev_polynomials();

    for ( int k = 0; k <= N; ++k )
    {
      a_k[k] = get_a_k_using_Chebyshev_Gauss_quadrature( k );
    }
  }

  /// \brief evaluates exp(x_i) using FFT
  void FFT()
  {
    initialize_coefficients_a_k();

    // Change first coefficient to be a_0 / 2 (as an additive constant to sum on 1..N)
    a_k[0] /= 2.0;

    size_t a_count  = N + 1;       // number of elements in a_k
    size_t el_count = a_count * 2; // number of coefficient in the complex array (double as many as a_k)
    // we uses sum sum(a[k] * e^(-pi * I * j * k / (N+1))) over k = 0..N that is different from gsl implementation (sum(a[k] * exp(-2 * pi * I * j * k / (N+1))))
    // so we should double the number of elements in the complex array and fill the second half with zeros

    double data[el_count];

    // Initialize the real array with the coefficients a_k
    for ( size_t i = 0; i < a_count; ++i )
    {
      data[i] = a_k[i];
    }

    // The rest is a bunch of zeroes
    for ( size_t i = a_count; i < el_count; ++i )
    {
      data[i] = 0;
    }

    // Perform the forward FFT
    gsl_fft_real_radix2_transform( data, 1, el_count );

    // Extract the result from the real array
    for ( int i = 0; i < a_count; ++i )
    {
      result[i] = data[i];
    }
  }

  /// \brief Compares exp(x) with the Fourier series approximation.
  /// The Fourier series approximation is given by:
  ///
  /// f(x) = a_0 + \sum_{k=1}^{N} (a_k \cos(kx))\f$, where
  /// a_0 = \int_{0}^{\pi} f(x)dx\f$ and f(x) = \exp(x).
  ///
  /// For each x in the range [0.1, 0.9], it calculates the absolute difference
  /// between exp(x) and the Fourier series approximation and displays the result.
  void display_Fourier()
  {
    FFT();
    initialize_points_x_i();

    for ( int i = 0; i <= N; ++i )
    {
      double true_value = std::exp( x_i[i] );
      std::cout << i << " / " << N << ") diff=" << abs( result[i] - true_value ) << '\n';
    }
  }

  bool is_initialized = false;

  /// \brief Computes exp(x) using the Fourier series approximation.
  /// \example \code Exp_Taylor( 0.1 ); \endcode
  /// \tparam T - Floating point type
  /// \param x - Value to compute
  /// \return e^x
  template<typename T>
    requires std::is_floating_point_v<T>
  constexpr T Exp_Fourier( T x )
  {
    if ( !is_initialized )
    {
      FFT();
      initialize_points_x_i();
      is_initialized = true;
    }

    size_t i = 0;
    double prev_delta = std::abs( x - x_i[i] );
    for ( ; i <= N; ++i )
    {
      double delta = std::abs( x - x_i[i] );

      if ( delta > prev_delta )
      {
        break;
      }

      prev_delta = delta;
    }

    return result[i];
  }
} // namespace ADAAI::Exp::Core::Fourier