#pragma once

#include <climits>
#include <cmath>
#include <iostream>

#include "../utils/Math.hpp"
#include "Consts.hpp"
#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_poly.h>

// STEP 1: finding coefficients (a_k) analytically
const int N = 20; // number of terms for the Fourier series.

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
  double value = ADAAI::CONST::a_0; // Initializing with a_0, the constant term

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
/// x_i = \cos\left(\frac{\pi(2i-1)}{2(N+1)}\right) for each i in the range [1, N + 1].
/// The nodes are then stored in the array x_i_values.
void calculate_the_nodes()
{
  // Loop through i values from 1 to N
  for ( int i = 1; i <= N + 1; ++i )
  {
    // Calculate the node x_i using the specified formula
    x_i_values[i] = std::cos( M_PI * ( 2 * i - 1 ) / ( 2 * ( N + 1 ) ) );
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

/// \brief Computes the k-th Chebyshev polynomial at a given point using gsl_chebyshev.h
/// \example \code computeChebyshev(3, 0.5); \endcode
/// \tparam k - Degree of the Chebyshev polynomial
/// \tparam x - Point at which to evaluate the Chebyshev polynomial
/// \return The value of the k-th Chebyshev polynomial at the given point x
double compute_Chebyshev( int k, double x )
{
  gsl_cheb_series* series = gsl_cheb_alloc( k + 1 );

  gsl_function F;
  F.function = []( double x, void* )
  { return std::exp( x ); };
  F.params = nullptr;

  gsl_cheb_init( series, &F, -1, 1 );

  double res = gsl_cheb_eval( series, x );

  gsl_cheb_free( series );

  return res;
}

double get_a_k_using_Chebyshev_Gauss_quadrature( int k )
{
  double a_k = 0;
  for ( int i = 1; i <= N + 1; ++i )
  {
    a_k += compute_Chebyshev( k, x_i_values[i] ) * exp_of_acos_of_x_i_values[i];
  }
  a_k *= 2 / ( N + 1 );
  return a_k;
}

void compare_coefficients_for_analytical_and_gauss_approaches()
{
  for ( int i = 1; i <= 100; i++ )
  {
    double a_true       = get_a_k_analytically( i );
    double a_calculated = get_a_k_using_Chebyshev_Gauss_quadrature( i );
    std::cout << "diff=" << abs( a_calculated - a_true) << '\n';
  }
}