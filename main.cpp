// #include "exp/TestExp.hpp"
// #include "diff/TestDiff.hpp"
#include "integration/cannon_problem/Cannon.cpp"

int main()
{
  // TestDiff();
  // TestExp();
  double ang = ADAAI::Integration::Cannon::findBestAngle();
  printf("Best angle: %f\n", ang);

  return 0;
}