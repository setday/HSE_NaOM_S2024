#pragma once

#include "../../utils/Tester.hpp"
#include "../Exp.hpp"

using namespace ADAAI::Utils;

namespace ADAAI::Exp::Tests
{
  template<ADAAI::Exp::Method M, typename T>
  struct ExpSingleCheckObject : public CheckObjectBase<T>
  {
    T error = 0.0;

    bool check_function( T x ) override
    {
      auto check = adaptive_compare<T, ADAAI::Exp::Exp<T, M>, std::exp>( x );
      error      = std::max( error, check );

      return check < ADAAI::CONST::BOUND<T>;
    }

    void print_data( std::ostream& os ) const override
    {
      os << "=> Max error: " << error << " * eps\n";
    }
  };

  template<ADAAI::Exp::Method M>
  struct ExpTripleCheckObject : public CheckObjectBase<long double>
  {
    long double f_error  = 0.0;
    long double d_error  = 0.0;
    long double ld_error = 0.0;

    bool check_function( long double x ) override
    {
      auto float_check       = adaptive_compare<float, ADAAI::Exp::Exp<float, M>, std::exp>( x );
      auto double_check      = adaptive_compare<double, ADAAI::Exp::Exp<double, M>, std::exp>( x );
      auto long_double_check = adaptive_compare<long double, ADAAI::Exp::Exp<long double, M>, std::exp>( x );

      f_error  = std::max( ( long double ) float_check, f_error );
      d_error  = std::max( ( long double ) double_check, d_error );
      ld_error = std::max( ( long double ) long_double_check, ld_error );

      long double check_error = ld_error;
      check_error             = std::max( d_error, check_error );
      check_error             = std::max( f_error, check_error );

      return check_error < ADAAI::CONST::BOUND<long double>;
    }

    void print_data( std::ostream& os ) const override
    {
      std::string method;

      switch ( M )
      {
        case ADAAI::Exp::Method::Taylor:
          method = "Taylor";
          break;
        case ADAAI::Exp::Method::Pade:
          method = "Pade";
          break;
        case ADAAI::Exp::Method::Chebyshev:
          method = "Chebyshev";
          break;
        case ADAAI::Exp::Method::ChebyshevUnused:
          method = "ChebyshevUnused";
          break;
        case ADAAI::Exp::Method::Fourier:
          method = "Fourier";
          break;
        case ADAAI::Exp::Method::FourierUnused:
          method = "FourierUnused";
          break;
        default:
          method = "Unknown";
      }

      os << "\n-> Method used: " << method << "\n\n";
      os << "=> Max errors:\n";
      os << "==> Float:       " << f_error << " * eps\n";
      os << "==> Double:      " << d_error << " * eps\n";
      os << "==> Long double: " << ld_error << " * eps\n";
    }
  };
} // namespace ADAAI::Exp::Tests
