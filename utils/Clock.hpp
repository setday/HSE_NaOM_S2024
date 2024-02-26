#pragma once

#include <chrono>

/// \brief Namespace for utility functions
/// \details Contains functions for testing and other purposes
namespace ADAAI::Utils
{
  /// \brief Measures execution time of a function
  /// \tparam ComputeFunction - Function to measure
  /// \return Execution time in milliseconds
  template<void ComputeFunction( void )>
  long long get_execution_time()
  {
    auto start = std::chrono::high_resolution_clock::now();
    ComputeFunction();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();
  }
} // namespace ADAAI::Utils
