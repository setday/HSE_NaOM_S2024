//#define EXP_TEST
#ifdef EXP_TEST
#include "exp/TestExp.hpp"
#endif

//#define DIFF_TEST
#ifdef DIFF_TEST
#include "diff/TestDiff.hpp"
#endif

//#define INTEGRATION_CANNON_PROBLEM
#ifdef INTEGRATION_CANNON_PROBLEM
#include "integration/cannon_problem/Cannon.cpp"
#endif
#define INTEGRATION_ORBITAL_PROBLEM
#ifdef INTEGRATION_ORBITAL_PROBLEM
#include "integration/orbital_problem/SatelliteLauncher.cpp"
#endif


int main()
{
  // Exp part
#ifdef EXP_TEST
   TestDiff();
#endif

   // Diff part
#ifdef DIFF_TEST
   TestExp();
#endif

   // Integration part
#ifdef INTEGRATION_CANNON_PROBLEM
  double ang = ADAAI::Integration::Cannon::findBestAngle();
  std::cout << "Best angle: " << ang << std::endl;
#endif
#ifdef INTEGRATION_ORBITAL_PROBLEM
  ADAAI::Integration::Satellite::launchSatellite();
#endif

  return 0;
}