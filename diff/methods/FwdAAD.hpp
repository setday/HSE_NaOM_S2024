/// \brief Namespace for AAD (automatic analytic differentiation)
/// \details Contains classes and functions for AAD method
namespace ADAAI::Diff::AAD
{
  /// \brief Forward AAD method class
  class FwdAAD
  {
  private:
    double val {};        // f(x, y) at the given point
    double d1[2] = { 0 }; // First derivatives with respect to x and y
    double d2[3] = { 0 }; // Second derivatives (xx, yy, xy)

    [[maybe_unused]] constexpr FwdAAD( double is_y, double v )
        : val( v ), d1( 1 - is_y, is_y )
    {
    }

    /// \brief friend exp function
    friend FwdAAD exp( FwdAAD v )
    {
      FwdAAD res {};
      res.val = std::exp( v.val );

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = res.val * v.d1[i];
      }

      for ( int i = 0; i < 2; ++i )
      {
        res.d2[i] = res.val * ( v.d2[i] + v.d1[i] * v.d1[i] );
      }

      res.d2[2] = res.val * ( v.d2[2] + v.d1[0] * v.d1[1] );

      return res;
    }

    /// \brief friend sin function
    friend FwdAAD sin( FwdAAD v )
    {
      FwdAAD res {};
      double sin_v = std::sin( v.val );
      double cos_v = std::cos( v.val );
      res.val      = sin_v;

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = cos_v * v.d1[i];
      }

      for ( int i = 0; i < 2; ++i )
      {
        res.d2[i] = cos_v * v.d2[i] - v.d1[i] * v.d1[i] * sin_v;
      }

      res.d2[2] = cos_v * v.d2[2] - v.d1[0] * v.d1[1] * sin_v;

      return res;
    }

    /// \brief friend cos function
    friend FwdAAD cos( FwdAAD v )
    {
      FwdAAD res {};
      double sin_v = std::sin( v.val );
      double cos_v = std::cos( v.val );
      res.val      = cos_v;

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = -sin_v * v.d1[i];
      }

      for ( int i = 0; i < 2; ++i )
      {
        res.d2[i] = -sin_v * v.d2[i] - cos_v * v.d1[i] * v.d1[i];
      }

      res.d2[2] = -v.d2[2] * sin_v - v.d1[0] * v.d1[1] * cos_v;

      return res;
    }

  public:
    FwdAAD operator-()
    {
      FwdAAD res {};
      res.val = -this->val;
      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = -this->d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        res.d2[i] = -this->d2[i];
      }

      return res;
    }

    FwdAAD operator+( FwdAAD const& g )
    {
      FwdAAD res {};
      res.val = this->val + g.val;

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = this->d1[i] + g.d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        res.d2[i] = this->d2[i] + g.d2[i];
      }

      return res;
    }

    FwdAAD operator+=( const FwdAAD& g )
    {
      this->val += g.val;

      for ( int i = 0; i < 2; ++i )
      {
        this->d1[i] += g.d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        this->d2[i] += g.d2[i];
      }

      return *this;
    }

    FwdAAD operator-( FwdAAD const& g )
    {
      FwdAAD res {};
      res.val = this->val - g.val;

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = this->d1[i] - g.d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        res.d2[i] = this->d2[i] - g.d2[i];
      }

      return res;
    }

    FwdAAD operator-=( FwdAAD const& g )
    {
      this->val -= g.val;

      for ( int i = 0; i < 2; ++i )
      {
        this->d1[i] -= g.d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        this->d2[i] -= g.d2[i];
      }

      return *this;
    }

    FwdAAD operator*( FwdAAD const& g )
    {
      FwdAAD res {};
      res.val = this->val * g.val;

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = this->d1[i] * g.val + g.d1[i] * this->val;
      }

      for ( int i = 0; i < 2; ++i )
      {
        res.d2[i] = this->d2[i] * g.val + 2 * this->d1[i] * g.d1[i] + g.d2[i] * this->val;
      }

      res.d2[2] = this->d2[2] * g.val + this->d1[0] * g.d1[1] + this->d1[1] * g.d1[0] + this->val * g.d2[2];

      return res;
    }

    FwdAAD operator*=( FwdAAD const& g )
    {
      return *this * g;
    }

    FwdAAD operator/( FwdAAD const& g )
    {
      FwdAAD res {};
      res.val = this->val / g.val;

      auto g_2 = g.val * g.val;
      auto g_3 = g.val * g_2;

      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = ( this->d1[i] * g.val - this->val * g.d1[i] ) / g_2;
      }

      for ( int i = 0; i < 2; ++i )
      {
        res.d2[i] = this->d2[i] / g.val + ( -g.d2[i] * this->val - 2 * this->d1[i] * g.d1[i] ) / g_2 + 2 * this->val * g.d1[i] * g.d1[i] / g_3;
      }

      res.d2[2] = ( this->d2[2] * g.val - 2 * g.d1[1] * this->d1[0] + 2 * g.d1[0] * this->val ) / g_2 + ( this->d1[0] * g.d1[1] - g.d2[2] * this->val - g.d1[0] * this->d1[1] ) / g_3;

      return res;
    }

    FwdAAD operator/=( FwdAAD const& g )
    {
      return *this / g;
    }

    FwdAAD()
        : val( 0 )
    {
    }

    /// \brief creates FwdAAD with value v
    constexpr explicit FwdAAD( double v )
        : val( v )
    {
    }

    /// \brief returns dx at the given point
    constexpr double X()
    {
      return d1[0];
    }

    /// \brief returns dy at the given point
    constexpr double Y()
    {
      return d1[1];
    }

    /// \brief returns dx^2 at the given point
    constexpr double XX()
    {
      return d2[0];
    }

    /// \brief returns dy^2 at the given point
    constexpr double YY()
    {
      return d2[1];
    }

    /// \brief returns dxy at the given point
    constexpr double XY()
    {
      return d2[2];
    }

    /// \brief creates FwdAAD with function f(x, y) = x
    constexpr static FwdAAD X( double v )
    {
      return { 0, v };
    }

    /// \brief creates FwdAAD with function f(x, y) = y
    constexpr static FwdAAD Y( double v )
    {
      return { 1, v };
    }
  };

  /// \brief first example function
  FwdAAD ExampleFunctionAAD( FwdAAD X, FwdAAD Y )
  {
    return sin( exp( X ) + Y * Y );
  }

  /// \brief second example function
  FwdAAD ExampleFunctionAAD2( FwdAAD X, FwdAAD Y )
  {
    return -X * X / Y + cos( X * Y );
  }

} // namespace ADAAI::Diff::AAD