#pragma once

#include <climits>

#include "../utils/Math.hpp"
#include "Consts.hpp"
#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_linalg.h>

namespace ADAAI
{

  enum class Method : int
  {
    Taylor    = 1,
    Pade      = 2,
    Chebyshev = 3,
  };

  /// \brief Namespace for core functions
  /// \details Contains functions for computing core functions
  namespace core
  {

    template<typename T>
    constexpr inline std::size_t MakeTaylorOrder()
    {
      T term = CONST::SQRT2<T>;
      for ( std::size_t i = 1; i < 1000; ++i )
      {
        term *= CONST::LN2<T> * 0.5 / i;
        if ( term < CONST::DELTA<T> )
          return i + 2; // if you'd return i or even i - 1 precision would drop
      }
      assert( false );
    }

    template<typename T>
    constexpr std::size_t N = MakeTaylorOrder<T>(); // this is required number of Tailor terms for T type

    constexpr int get_a( int n, int k )
    {
      if ( n % 2 == 0 )
      {
        if ( k % 2 == 0 )
          return 0;
        if ( k == 1 )
          return n;
        return 2 * n;
      }
      // n is odd now
      if ( k % 2 == 1 )
        return 0;
      if ( k == 0 )
        return n;
      return 2 * n;
    }

    constexpr int get_T( int n )
    {
      if ( n % 2 == 1 )
        return 0;
      if ( n % 4 == 0 )
        return 1;
      return -1;
    };

    double f( double x, void* p )
    {
      return std::exp( x );
    }

    template<typename T>
      requires std::is_floating_point_v<T>
    constexpr T Chebyshev_Exp( T x )
    {
      const std::size_t SIZE = N<T> + 1;

      T a_data[SIZE * SIZE] = { 0.0 };

      for ( int k = 0; k < SIZE; ++k )
      {
        if ( k == SIZE - 1 )
        {
          for ( int n = 0; n < SIZE; ++n )
            a_data[( SIZE - 1 ) * SIZE + n] = get_T( n );
        }
        else
        {
          a_data[k * SIZE + k] = -1.0;
          for ( int n = k + 1; n < SIZE; ++n )
            a_data[k * SIZE + n] = get_a( n, k );
        }
      }

      T b_data[SIZE]   = { 0.0 };
      b_data[SIZE - 1] = 1.0;

      T a_data_copy[SIZE * SIZE] = { 0.0 };
      std::copy( std::begin( a_data ), std::end( a_data ), std::begin( a_data_copy ) );

      // TODO: here we need to somehow implement gsl_matrix_float_view and gsl_matrix_long_double_view
      gsl_matrix_view A = gsl_matrix_view_array( a_data, SIZE, SIZE );
      gsl_vector_view b = gsl_vector_view_array( b_data, SIZE );

      gsl_vector* sol = gsl_vector_alloc( SIZE );       // solution
      gsl_linalg_HH_solve( &A.matrix, &b.vector, sol ); // s is status

      //      std::cout << "Solution\n";
      //      for ( int i = 0; i < SIZE; ++i )
      //      {
      //        std::cout << sol->data[i] << " ";
      //      }
      //      std::cout << "\n";
      //
      //      std::cout << "Sums by row\n";
      //      for ( int k = 0; k < SIZE; ++k )
      //      {
      //        T cnt = 0;
      //        for ( int n = 0; n < SIZE; ++n )
      //        {
      //          cnt += a_data_copy[k * SIZE + n] * sol->data[n];
      //        }
      //        std::cout << cnt << " " << b_data[k] << "\n";
      //      }
      //      std::cout << "\n";

      gsl_cheb_series* cs = gsl_cheb_alloc( SIZE - 1 );
      gsl_function     F;
      F.function = &f;
      gsl_cheb_init( cs, &F, -1, 1 );

            std::cout << "real chebyshev and ours\n";
            for (int i = 0; i < SIZE; ++i) {
              std::cout << cs->c[i] << " " << sol->data[i] << "\n";
            }

      //      cs->c = sol->data;
      //      cs->c[0] *= 2;
      //      cs->order = SIZE;
      //      cs->a     = -1;
      //      cs->b     = 1;

      //      std::cout << "x: " << x << '\n'
      //                << "standard exp: " << std::exp( x ) << '\n'
      //                << "Chebyshev exp: " << gsl_cheb_eval( cs, x ) << "\n\n";

      return gsl_cheb_eval( cs, x );
    }

    /// \brief Computes exp(x) using Taylor series
    /// \example \code Exp_( 0.1 ); \endcode
    /// \tparam T - Floating point type
    /// \tparam M - Method to calculate exp
    /// \param x - Value to compute
    /// \return e^x
    template<typename T, Method M = Method::Taylor>
      requires std::is_floating_point_v<T>
    constexpr T Exp_( T x )
    {
      switch ( M )
      {
        case Method::Chebyshev:
          return Chebyshev_Exp( x );
        case Method::Taylor:
        {
          T result = 1, term = 1;
          for ( std::size_t n = 1; n < N<T>; ++n )
          {
            term = term * x / n;
            result += term;
          }
          return result;
        }
        case Method::Pade:
        {
          T numerator   = 0;
          T denominator = 0;

          for ( const auto& term : CONST::P_TERMS<T> )
          {
            numerator = x * numerator + term;
          }

          for ( const auto& term : CONST::Q_TERMS<T> )
          {
            denominator = x * denominator + term;
          }

          return numerator / denominator;
        }
        default:
          assert( false );
      }
    }

  } // namespace core

  /// \brief Computes exp(x)
  /// \example \code Exp( 0.1 ); \endcode
  /// \tparam T - Floating point type
  /// \tparam M - Method to calculate exp
  /// \param x - Value to compute
  /// \return e^x
  template<typename T, Method M = Method::Taylor>
    requires std::is_floating_point_v<T>
  constexpr T Exp( T x )
  {
    if ( std::isnan( x ) )
    {
      return std::numeric_limits<T>::quiet_NaN();
    }

    T y = CONST::LOG2E<T> * x, int_part = 0;
    T frac_part = std::modf( y, &int_part );

    if ( int_part < INT_MIN )
    {
      return 0.0;
    }
    if ( int_part > 11500 ) // this will be overflow for any floating type
    {
      return std::numeric_limits<T>::infinity();
    }

    int n = int( int_part ); // generic case now: e^x = 2^n * e^x2

    //    if ( frac_part > 0.5 ) // lowers the precision
    //    {
    //      n++;
    //      frac_part -= 1;
    //    }

    T x2 = CONST::LN2<T> * frac_part; // if abs(frac_part) <= 0.5, so will be abs(x2)
    T E2 = core::Exp_<T, M>( x2 );
    T En = std::pow<T>( 2.0, n ); // should've been equivalent to std::ldexp(1.0, n);
    return E2 * En;
  }
} // namespace ADAAI
