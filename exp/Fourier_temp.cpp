#pragma once

#include <climits>
#include <cmath>
#include <iostream>
#include <stdio.h>

#include "../utils/Math.hpp"
#include "Consts.hpp"
#include <gsl/gsl_blas.h>
#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_fft_complex.h>
#include <gsl/gsl_fft_halfcomplex.h>
#include <gsl/gsl_fft_real.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_poly.h>

// STEP 1: finding coefficients (a_k) analytically (integration by parts)
const int N = 31; // number of terms for the Fourier series.

/// \brief Computes the k-th coefficient for the Fourier series.
/// \param k - Index of the coefficient.
/// \return The computed coefficient a_k.
double get_a_k_analytically( int k )
{
  // Formula for a_k in the Fourier series
  return ( 2 / M_PI ) * ( std::exp( M_PI ) * ( k * std::sin( M_PI * k ) + std::cos( M_PI * k ) ) - 1 ) / ( k * k + 1 );
}

/// \brief Computes the value of the Fourier series for the exponential function.
/// \param x - The input value.
/// \param N - The number of terms in the series (default is 20).
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

/// \brief Compares exp(x) with the Fourier series approximation.
/// The Fourier series approximation is given by:
///
/// f(x) = a_0 + \sum_{k=1}^{N} (a_k \cos(kx))\f$, where
/// a_0 = \int_{0}^{\pi} f(x)dx\f$ and f(x) = \exp(x).
///
/// For each x in the range [0.1, 0.9], it calculates the absolute difference
/// between exp(x) and the Fourier series approximation and displays the result.
void display_diffs_between_exp_and_analytical_Fourier()
{
  // Loop through x values from 0.1 to 0.9
  for ( int i = 1; i <= 50; ++i )
  {
    double x = static_cast<double>( i ) / 50;

    // True value is exp(x)
    double true_value = std::exp( x );

    // Evaluate the Fourier series for exp(x)
    double evaluated_value = fourier_series_for_exp( x );

    // Display the absolute difference between true and evaluated values
    std::cout << "diff=" << std::abs( true_value - evaluated_value ) << '\n';
  }
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
  // Loop through i values from 1 to N
  for ( int i = 1; i <= N + 1; ++i )
  {
    // Calculate exp(acos(x_i)) and store the result
    exp_of_acos_of_x_i_values[i] = std::exp( std::acos( x_i_values[i] ) );
  }
}

/// WARNING: IT'S A RECURSIVE COMPUTATION OF THE CHEBYSHEV POLYNOMIALS

/// \brief Computes the k-th Chebyshev polynomial at a given point using gsl_chebyshev.h
/// \example \code computeChebyshev(3, 0.5); \endcode
/// \tparam k - Degree of the Chebyshev polynomial
/// \tparam x - Point at which to evaluate the Chebyshev polynomial
/// \return The value of the k-th Chebyshev polynomial at the given point x
double compute_Chebyshev( int k, double x )
{
  if ( k == 1 )
  {
    return x;
  }
  else if ( k == 0 )
  {
    return 1;
  }
  else
  {
    return 2 * x * compute_Chebyshev( k - 1, x ) - compute_Chebyshev( k - 2, x );
  }
}

/// \brief Computes the k-th coefficient for the Fourier series using Chebyshev-Gauss quadrature
/// \param k - Index of the coefficient.
/// \return The computed coefficient a_k.
double get_a_k_using_Chebyshev_Gauss_quadrature( int k )
{
  double a_k = 0;
  for ( int i = 1; i <= N + 1; ++i )
  {
    a_k += compute_Chebyshev( k, x_i_values[i] ) * exp_of_acos_of_x_i_values[i];
  }
  a_k *= 2.0 / ( N + 1 );
  return a_k;
}

/// \brief compares coefficients obtaied using analytical solution and Chebyshev-Gauss quadrature
void compare_coefficients_for_analytical_and_gauss_approaches()
{
  calculate_the_nodes();
  calculate_exp_of_acos_of_x_i();
  for ( int i = 0; i <= 30; i++ )
  {
    double a_true       = get_a_k_analytically( i );
    double a_calculated = get_a_k_using_Chebyshev_Gauss_quadrature( i );
    std::cout << "diff=" << abs( a_calculated - a_true ) << '\n';
  }
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
  calculate_the_nodes();
  calculate_exp_of_acos_of_x_i();
  for ( int k = 0; k <= N; ++k )
  {
    a_k[k] = get_a_k_using_Chebyshev_Gauss_quadrature( k );
  }
}

/// \brief evaluates exp(x_i) using FFT
void FFT()
{
  initialize_points_x_i();
  initialize_coefficients_a_k();

  // Change first coefficient to be a_0 / 2 (as an additive constant to sum on 1..N)
  a_k[0] /= 2.0;

  gsl_fft_complex_wavetable* wavetable;
  gsl_fft_complex_workspace* workspace;

  size_t a_count  = N + 1;       // number of elements in a_k
  size_t el_count = a_count * 2; // number of coefficient in the complex array (double as many as a_k)
  // we uses sum sum(a[k] * e^(-pi * I * j * k / (N+1))) over k = 0..N that is different from gsl implementation (sum(a[k] * exp(-2 * pi * I * j * k / (N+1))))
  // so we should double the number of elements in the complex array and fill the second half with zeros

  double data[2 * el_count];

  // Initialize the complex array with the coefficients a_k
  for ( size_t i = 0; i < a_count; ++i )
  {
    data[2 * i]     = a_k[i]; // real part
    data[2 * i + 1] = 0.0;    // imaginary part
  }
  // we skip elements from N+1 to 2N because we don't need the second half of sum
  for ( size_t i = a_count; i < el_count; ++i )
  {
    data[2 * i]     = 0.0; // real part
    data[2 * i + 1] = 0.0; // imaginary part
  }

  // Perform the forward FFT
  gsl_fft_complex_radix2_forward( data, 1, el_count );

  // Extract the result from the complex array
  for ( int i = 0; i < a_count; ++i )
  {
    result[i] = data[2 * i];
  }
}

/// \brief compares the results of the FFT with the exp(x_i) values
void compare_results_of_FFT_with_exp_x_i()
{
  FFT();
  for ( int i = 0; i <= N; ++i )
  {
    double true_value = std::exp( x_i[i] );
    std::cout << i << " / " << N << ") diff=" << abs( result[i] - true_value ) << '\n';
  }
}
