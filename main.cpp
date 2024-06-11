//#define EXP_TEST
#ifdef EXP_TEST
#  include "exp/TestExp.hpp"
#endif

//#define DIFF_TEST
#ifdef DIFF_TEST
#  include "diff/TestDiff.hpp"
#endif

//#define INTEGRATION_CANNON_PROBLEM
#ifdef INTEGRATION_CANNON_PROBLEM
#  include "integration/cannon_problem/Cannon.cpp"
#endif
//#define INTEGRATION_ORBITAL_PROBLEM
#ifdef INTEGRATION_ORBITAL_PROBLEM
#  include "integration/orbital_problem/SatelliteLauncher.cpp"
#endif

#define PDE_BSM_PROBLEM
#ifdef PDE_BSM_PROBLEM
#  include "integration/pde_bsm/AucLauncher.cpp"
#endif

int main()
{
  // Exp part
#ifdef EXP_TEST
  TestExp();
#endif

  // Diff part
#ifdef DIFF_TEST
  TestDiff();
#endif

  // Integration part
#ifdef INTEGRATION_CANNON_PROBLEM
  double ang = ADAAI::Integration::Cannon::findBestAngle();
  std::cout << "Best angle: " << ang << std::endl;
#endif
#ifdef INTEGRATION_ORBITAL_PROBLEM
  ADAAI::Integration::Satellite::launchSatellite();
#endif

  // PDE BSM part
#ifdef PDE_BSM_PROBLEM
  double analytical = ADAAI::Integration::PDE_BSM::launchAuc( ADAAI::Integration::PDE_BSM::SolutionApproach::ANALYTICAL );
  double explicit_  = ADAAI::Integration::PDE_BSM::launchAuc( ADAAI::Integration::PDE_BSM::SolutionApproach::EXPLICIT );
  double implicit_  = ADAAI::Integration::PDE_BSM::launchAuc( ADAAI::Integration::PDE_BSM::SolutionApproach::IMPLICIT );

  std::cout << "Actual Premium = " << analytical << '\n';
  std::cout << "Explicit Premium = " << explicit_ << '\n';
  std::cout << "Implicit Premium = " << implicit_ << '\n';
#endif

  return 0;
}
