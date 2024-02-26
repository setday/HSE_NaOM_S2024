#pragma once

#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_linalg.h>

#include "../../utils/Consts.hpp"
#include "TaylorExponential.hpp"

namespace ADAAI::Exp::Core::Chebyshev
{

  namespace unused
  {
    /// \brief Function to be used with gsl_cheb_alloc
    /// \tparam T - Floating point type
    /// \param x - Value to compute
    /// \return e^x
    template<typename T>
    T f( T x, void* )
    {
      return std::exp( x );
    }

    /// \brief Computes exp(x) using Chebyshev series
    /// \example \code Exp_Chebyshev( 0.1 ); \endcode
    /// \tparam T - Floating point type
    /// \param x - Value to compute
    /// \return e^x
    template<typename T>
      requires std::is_floating_point_v<T>
    constexpr T
    Exp_Chebyshev( T x )
    {
      gsl_cheb_series* cs = gsl_cheb_alloc( Taylor::N<T> + 1 );

      gsl_function F = { .function = &f };

      gsl_cheb_init( cs, &F, -1, 1 );

      T res = gsl_cheb_eval( cs, x );

      gsl_cheb_free( cs ); // for proper coding

      return res;
    }

  } // namespace unused

  constexpr size_t get_a( size_t n, size_t k )
  {
    if ( ( k & 1 ) == ( n & 1 ) ) // k and n are both even or both odd
      return 0;
    if ( ( n & 1 ) == 1 && k == 0 )
      return n;
    return n << 1;
  }

  constexpr int get_T0( size_t n )
  {
    if ( ( n & 1 ) == 1 )
      return 0;
    if ( ( n & 3 ) == 0 )
      return 1;
    return -1;
  }

  /// @warning Only float and double values are available
  template<typename T>
  constexpr T Exp_Chebyshev( T x )
  {
    const std::size_t SIZE = Taylor::N<T> + 1;

    double a_data[SIZE * SIZE] = { 0.0 };

    for ( size_t k = 0; k < SIZE; ++k )
    {
      if ( k == SIZE - 1 )
      {
        for ( size_t n = 0; n < SIZE; ++n )
          a_data[k * SIZE + n] = get_T0( n );
      }
      else
      {
        a_data[k * SIZE + k] = -1.0;
        for ( size_t n = k + 1; n < SIZE; ++n )
          a_data[k * SIZE + n] = get_a( n, k );
      }
    }

    double b_data[SIZE] = { 0.0 };
    b_data[SIZE - 1]    = 1.0;

    // TODO: here we need to somehow implement gsl_matrix_float_view and gsl_matrix_long_double_view
    gsl_matrix_view A = gsl_matrix_view_array( a_data, SIZE, SIZE );
    gsl_vector_view b = gsl_vector_view_array( b_data, SIZE );

    int              s;
    gsl_permutation* p   = gsl_permutation_alloc( SIZE );
    gsl_vector*      sol = gsl_vector_alloc( SIZE );

    gsl_linalg_LU_decomp( &A.matrix, p, &s );
    gsl_linalg_LU_solve( &A.matrix, p, &b.vector, sol );

    gsl_cheb_series* cs = gsl_cheb_alloc( SIZE - 1 );
    cs->c               = sol->data;

    cs->c[0] *= 2;

    cs->order = SIZE;
    cs->a     = -1;
    cs->b     = 1;

    return gsl_cheb_eval( cs, ( double ) x );
  }
} // namespace ADAAI::Exp::Core::Chebyshev
