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

    /// TODO: implement unary +, -, binary +=, -=, *, *=, /, /=, elemenatry functions: sin, cos, exp friend functions (with derivatives of course)

  public:
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
  };

} // namespace ADAAI::Diff::FwdAAD