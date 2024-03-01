namespace ADAAI::Diff::FwdAAD
{ // Automatic analytic differentiation
  class AAD
  {
  private:
    double val;           // f(x, y) at given point
    double d1[2] = { 0 }; // first derivatives by x and y
    double d2[3] = { 0 }; // second derivatives (by xx, yy, xy)

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

    /// TODO: implement unary +, -, binary +=, -=, *=, /, /=, elemenatry functions: cos friend function

  public:

    AAD operator+( AAD const& right )
    {
      AAD res = *this;
      res.val += right.val;
      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] += right.d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        res.d2[i] += right.d2[i];
      }

      return res;
    }

    AAD operator-( AAD const& right )
    {
      AAD res = *this;
      res.val -= right.val;
      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] -= right.d1[i];
      }

      for ( int i = 0; i < 3; ++i )
      {
        res.d2[i] -= right.d2[i];
      }
      return res;
    }

    AAD operator*( AAD const& right )
    {
      AAD res {};
      res.val = this->val * right.val;
      for ( int i = 0; i < 2; ++i )
      {
        res.d1[i] = this->d1[i] * right.val + right.d1[i] * this->val;
      }

      for ( int i = 0; i < 2; ++i )
      {
        res.d2[i] = this->d2[i] * right.val + 2 * this->d1[i] * right.d1[i] +
                    right.d2[i] * this->val;
      }

      res.d2[2] = this->d2[2] * right.val + this->d1[0] * right.d1[1] +
                  this->d1[1] * right.d1[0] + this->val * right.d2[2];

      return res;
    }

    AAD operator/( AAD const& right )
    {
      AAD res {};
      /// TODO
      return res;
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

} // namespace ADAAI::Diff::FwdAAD