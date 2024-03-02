namespace ADAAI::Diff::FwdAAD
{ // Automatic analytic differentiation
  class AAD
  {
  private:
    double val;           // f(x, y) at the given point
    double d1[2] = { 0 }; // First derivatives with respect to x and y
    double d2[3] = { 0 }; // Second partial derivatives (xx, yy, xy)

    constexpr AAD( double is_y, double v )
        : val( v ), d1( 1 - is_y, is_y )
    {
    }

    friend AAD exp( AAD v )
    {
      AAD res {};
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

    friend AAD sin( AAD v )
    {
      AAD    res {};
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

    friend AAD cos( AAD v )
    {
      AAD    res {};
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
    AAD operator-()
    {
      AAD res {};
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

    AAD operator+( AAD const& g )
    {
      AAD res {};
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

    AAD operator+=( const AAD& g )
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

    AAD operator-( AAD const& g )
    {
      AAD res {};
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

    AAD operator-=( AAD const& g )
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

    AAD operator*( AAD const& g )
    {
      AAD res {};
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

    AAD operator*=( AAD const& g )
    {
      return *this * g;
    }

    AAD operator/( AAD const& g )
    {
      AAD res {};
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

    AAD operator/=( AAD const& g )
    {
      return *this / g;
    }

    AAD()
        : val( 0 )
    {
    }

    constexpr explicit AAD( double v )
        : val( v )
    {
    }

    constexpr double X()
    {
      return d1[0];
    }

    constexpr double Y()
    {
      return d1[1];
    }

    constexpr double XX()
    {
      return d2[0];
    }

    constexpr double YY()
    {
      return d2[1];
    }

    constexpr double XY()
    {
      return d2[2];
    }

    constexpr static AAD X( double v )
    {
      return { 0, v };
    }

    constexpr static AAD Y( double v )
    {
      return { 1, v };
    }
  };

  AAD ExampleFunctionAAD( AAD X, AAD Y )
  {
    return sin( exp( X ) + Y * Y );
  }

  AAD ExampleFunctionAAD2( AAD X, AAD Y )
  {
    return -X * X / Y + cos( X * Y );
  }

} // namespace ADAAI::Diff::FwdAAD