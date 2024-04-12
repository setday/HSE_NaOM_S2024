#pragma once

#include <cmath>

/***
 * => Entering this file you see strange manuscript:
 *
 * Per me si va ne la città dolente,
 * per me si va ne l'etterno dolore,
 * per me si va tra la perduta gente.
 *
 * Giustizia mosse il mio alto fattore:
 * fecemi la divina podestate,
 * la somma sapienza e 'l primo amore.
 *
 * Dinanzi a me non fuor cose create
 * se non etterne, e io etterno duro.
 * Lasciate ogne speranza, voi ch'intrate.
 ***/
 
typedef std::pair<double, double> Point;

namespace ADAAI::Diff::Tests
{
  struct BasicFunction // aka absolute zero function
  {
    static double operator()( Point p )
    {
      return 0.0;
    }

    static BasicFunction XDerivative;
    static BasicFunction YDerivative;
  };

  // ============
  // f(x, y) = c
  // ============

  /// This is service function to create constant functions
  /// @warning Do not use it directly. Use ConstFunction instead.
  struct ZeroFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return 0.0;
    }

    constexpr static auto XDerivative = BasicFunction();
    constexpr static auto YDerivative = BasicFunction();
  };

  struct OneFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return 1.0;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  template<double ConstCallback()>
  struct ConstFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return ConstCallback();
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  // ============
  // f(x, y) = x
  // f(x, y) = y
  // ============

  struct XTermFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return p.first;
    }

    constexpr static auto XDerivative = OneFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct YTermFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return p.second;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = OneFunction();
  };

  // ============
  // f(x, y) = x + y
  // ============

  template<typename LeftSubFunction, typename RightSubFunction>
  struct SumFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return LeftSubFunction()( p.first, p.second ) + RightSubFunction()( p.first, p.second );
    }

    constexpr static auto XDerivative = SumFunction<typeof( LeftSubFunction::XDerivative ), typeof( RightSubFunction::XDerivative )>();
    constexpr static auto YDerivative = SumFunction<typeof( LeftSubFunction::YDerivative ), typeof( RightSubFunction::YDerivative )>();
  };

  template<>
  struct SumFunction<const ZeroFunction, const ZeroFunction> : BasicFunction {
    static double operator()( Point p )
    {
      return 0.0;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  template<typename RightSubFunction>
    requires (!std::is_same_v<RightSubFunction, const ZeroFunction>)
  struct SumFunction<const ZeroFunction, RightSubFunction> : RightSubFunction {};

  template<typename LeftSubFunction>
    requires (!std::is_same_v<LeftSubFunction, const ZeroFunction>)
  struct SumFunction<LeftSubFunction, const ZeroFunction> : LeftSubFunction {};

  // ============
  // f(x, y) = x * y
  // ============

  template<typename LeftSubFunction, typename RightSubFunction>
  struct ProductFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return LeftSubFunction()( p.first, p.second ) * RightSubFunction()( p.first, p.second );
    }

    constexpr static auto XDerivative = SumFunction<ProductFunction<typeof( LeftSubFunction::XDerivative ), RightSubFunction>, ProductFunction<LeftSubFunction, typeof( RightSubFunction::XDerivative )>>();
    constexpr static auto YDerivative = SumFunction<ProductFunction<typeof( LeftSubFunction::YDerivative ), RightSubFunction>, ProductFunction<LeftSubFunction, typeof( RightSubFunction::YDerivative )>>();
  };

  template<>
  struct ProductFunction<const ZeroFunction, const ZeroFunction> : BasicFunction {
    static double operator()( Point p )
    {
      return 0.0;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  template<typename RightSubFunction>
    requires (!std::is_same_v<RightSubFunction, const ZeroFunction>)
  struct ProductFunction<const ZeroFunction, RightSubFunction> : ProductFunction<const ZeroFunction, const ZeroFunction> {};

  template<typename LeftSubFunction>
    requires (!std::is_same_v<LeftSubFunction, const ZeroFunction>)
  struct ProductFunction<LeftSubFunction, const ZeroFunction> : ProductFunction<const ZeroFunction, const ZeroFunction> {};

  // ============
  // f(x) = -x
  // ============

  template<typename SubFunction>
  struct NegateFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return -SubFunction()( p.first, p.second );
    }

    constexpr static auto XDerivative = NegateFunction<typeof( SubFunction::XDerivative )>();
    constexpr static auto YDerivative = NegateFunction<typeof( SubFunction::YDerivative )>();
  };

  // ============
  // f(x, y) = x^n
  // f(x, y) = y^n
  // ============

  template<int N, typename SubFunction>
  struct PowerFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return std::pow( SubFunction()( p.first, p.second ), N );
    }

    constexpr static auto XDerivative = ProductFunction<ConstFunction<[]() { return (double) N; }>, ProductFunction<PowerFunction<N - 1, SubFunction>, typeof( SubFunction::XDerivative )>>();
    constexpr static auto YDerivative = ZeroFunction();
  };

  template<typename SubFunction>
  struct PowerFunction<1, SubFunction> : BasicFunction {
    static double operator()( Point p )
    {
      return SubFunction()( p.first, p.second );
    }

    constexpr static auto XDerivative = SubFunction::XDerivative;
    constexpr static auto YDerivative = SubFunction::YDerivative;
  };

  template<typename SubFunction>
  struct PowerFunction<0, SubFunction> : OneFunction {};

  // ============
  // f(x, y) = sin(x)
  // f(x, y) = cos(x)
  // ============

  template<typename SubFunction>
  struct CosFunction;

  template<typename SubFunction>
  struct SinFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return std::sin( SubFunction()( p.first, p.second ) );
    }

    constexpr static auto XDerivative = ProductFunction<CosFunction<SubFunction>, typeof( SubFunction::XDerivative )>();
    constexpr static auto YDerivative = ZeroFunction();
  };

  template<typename SubFunction>
  struct CosFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return std::cos( SubFunction()( p.first, p.second ) );
    }

    constexpr static auto XDerivative = ProductFunction<ConstFunction<[]() { return -1.0; }>, ProductFunction<SinFunction<SubFunction>, typeof( SubFunction::XDerivative )>>();
    constexpr static auto YDerivative = ZeroFunction();
  };

  // ============
  // f(x, y) = exp(x)
  // ============

  template<typename SubFunction>
  struct ExpFunction : BasicFunction
  {
    static double operator()( Point p )
    {
      return std::exp( SubFunction()( p.first, p.second ) );
    }

    constexpr static auto XDerivative = ProductFunction<ExpFunction<SubFunction>, typeof( SubFunction::XDerivative )>();
    constexpr static auto YDerivative = ZeroFunction();
  };

  // ============
  // Example functions
  // ============

  struct ExampleFunction_1_dXY : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      double exyy = std::exp( p.first ) + y * y;

      return -std::sin( exyy ) * std::exp( p.first ) * 2 * y;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct ExampleFunction_1_dXX : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      double exyy = std::exp( p.first ) + y * y;

      return -std::sin( exyy ) * std::exp( p.first ) * std::exp( p.first ) + std::cos( exyy ) * std::exp( p.first );
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct ExampleFunction_1_dX : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      double exyy = std::exp( p.first ) + y * y;

      return std::cos( exyy ) * std::exp( p.first );
    }

    constexpr static auto XDerivative = ExampleFunction_1_dXX();
    constexpr static auto YDerivative = ExampleFunction_1_dXY();
  };

  struct ExampleFunction_1_dYY : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      double exyy = std::exp( p.first ) + y * y;

      return -std::sin( exyy ) * 4 * y * y + std::cos( exyy ) * 2;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct ExampleFunction_1_dY : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      double exyy = std::exp( p.first ) + y * y;

      return std::cos( exyy ) * 2 * y;
    }

    constexpr static auto XDerivative = ExampleFunction_1_dXY();
    constexpr static auto YDerivative = ExampleFunction_1_dYY();
  };

  struct ExampleFunction_1 : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      double exyy = std::exp( p.first ) + y * y;

      return std::sin( exyy );
    }

    constexpr static auto XDerivative = ExampleFunction_1_dX();
    constexpr static auto YDerivative = ExampleFunction_1_dY();
  };

  // ============

  struct ExampleFunction_2_dXY : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      return 2 * x / (y * y) - std::cos( x * y) * x * y - std::sin( x * y);
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct ExampleFunction_2_dXX : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      return -2 / y - std::cos( x * y ) * y * y;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct ExampleFunction_2_dX : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      return -2 * x / y - std::sin( x * y ) * y;
    }

    constexpr static auto XDerivative = ExampleFunction_2_dXX();
    constexpr static auto YDerivative = ExampleFunction_2_dXY();
  };

  struct ExampleFunction_2_dYY : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      return -2 * x * x / ( y * y * y ) - std::cos( x * y ) * x * x;
    }

    constexpr static auto XDerivative = ZeroFunction();
    constexpr static auto YDerivative = ZeroFunction();
  };

  struct ExampleFunction_2_dY : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      return x * x / ( y * y ) - std::sin( x * y ) * x;
    }

    constexpr static auto XDerivative = ExampleFunction_2_dXY();
    constexpr static auto YDerivative = ExampleFunction_2_dYY();
  };

  struct ExampleFunction_2 : BasicFunction
  {
    static double operator()( Point p )
    {
      double x = p.first;
      double y = p.second;

      return -x * x / y + std::cos( x * y );
    }

    constexpr static auto XDerivative = ExampleFunction_2_dX();
    constexpr static auto YDerivative = ExampleFunction_2_dY();
  };
}  // namespace ADAAI::Diff::Tests

/***
 * Turning the knowledge into ash and blowing away...
 * Appealing to the laws of the Creation...
 * Changing the definition of time and space...
 * I am the one who can understand the future...
 * I am the one who can create the past...
 * I am the one who can bring the end...
 * The time has come - a time of absolute evil.
 ***/
