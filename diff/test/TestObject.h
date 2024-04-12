#pragma once

#include "../../utils/Tester.hpp"
#include "../Diff.hpp"
#include "Functions.h"

using namespace ADAAI::Utils;

namespace ADAAI::Diff::Tests
{
  template<Method M, D d, typename F = BasicFunction>
  struct DifferentiatorT {
    static double operator()( std::pair<double, double> const& p )
    {
      return Differentiator<M, d>( F::operator(), p.first, p.second );
    }
  };


  template<typename Function>
    requires std::is_base_of_v<ADAAI::Diff::Tests::BasicFunction, Function>
  struct PentaCheckObject : public CheckObjectBase<double>
  {
    Method methods[5] = { Method::Stencil3, Method::Stencil3Extra, Method::Stencil5, Method::Stencil5Extra, Method::FwdAAD };
    D      ds[5]      = { D::X, D::Y, D::XX, D::YY, D::XY };

    double dx_error[5]  = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    double dy_error[5]  = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    double dxx_error[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    double dyy_error[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };
    double dxy_error[5] = { 0.0, 0.0, 0.0, 0.0, 0.0 };

    double* errors[5] = { dx_error, dy_error, dxx_error, dyy_error, dxy_error };

    bool check_function( double x ) override
    {
      for ( int m = 0; m < 4; ++m ) ///TODO: FwdAAD not available for now
      {
        const Method method = methods[m];
        const 
        // dX
        auto res     = adaptive_compare<double, Point,
                                        DifferentiatorT<method, D[0], Function>,
                                        typeof( Function::XDerivative )>( Point( x, 0.0 ) );
        errors[0][m] = std::max( errors[0][m], res );

        // dY
        res          = adaptive_compare<double, Point,
                                        Differentiator<method, D[1], Function>,
                                        typeof( Function::YDerivative )>( Point( 0.0, x ) );
        errors[1][m] = std::max( errors[1][m], res );

        // dXX
        res          = adaptive_compare<double, Point,
                                        Differentiator<method, D[2], Function>,
                                        typeof( Function::XDerivative::XDerivative )>( Point( x, 0.0 ) );
        errors[2][m] = std::max( errors[2][m], res );

        // dYY
        res          = adaptive_compare<double, Point,
                                        Differentiator<method, D[3], Function>,
                                        typeof( Function::YDerivative::YDerivative )>( Point( 0.0, x ) );
        errors[3][m] = std::max( errors[3][m], res );

        // dXY
        res          = adaptive_compare<double, Point,
                                        Differentiator<method, D[4], Function>,
                                        typeof( Function::XDerivative::YDerivative )>( Point( x, x ) );
        errors[4][m] = std::max( errors[4][m], res );

        double check_error = errors[0][m];
        check_error             = std::max( errors[1][m], check_error );
        check_error             = std::max( errors[2][m], check_error );
        check_error             = std::max( errors[3][m], check_error );
        check_error             = std::max( errors[4][m], check_error );

        return check_error < ADAAI::CONST::BOUND<double>;
      }
    }

    void print_data( std::ostream& os ) const override
    {
      for ( int i = 0; i < 5; ++i )
      {
        os << "=> Max error for " << DTypes[i] << ":\n";
        for ( int j = 0; j < 5; ++j )
        {
          os << "  " << Methods[int( methods[j] )] << ": " << errors[i][j] << " * eps\n";
        }
      }
    }
  };
} // namespace ADAAI::Diff::Tests
