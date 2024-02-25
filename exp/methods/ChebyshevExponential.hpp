#pragma once

#include <gsl/gsl_chebyshev.h>
#include <gsl/gsl_linalg.h>

#include "../../utils/Consts.hpp"
#include "TaylorExponential.hpp"

namespace ADAAI::Exp::Core::Chebyshev {
    /// \brief Function to be used with gsl_cheb_alloc
    /// \tparam T - Floating point type
    /// \param x - Value to compute
    /// \param p - Pointer to void
    /// \return e^x
    template<typename T>
    T f(T x, void *p) {
        return std::exp(x);
    }

    /// \brief Computes exp(x) using Chebyshev series
    /// \example \code Exp_Chebyshev( 0.1 ); \endcode
    /// \tparam T - Floating point type
    /// \param x - Value to compute
    /// \return e^x
    template<typename T>
    requires std::is_floating_point_v<T>
    constexpr T
    Exp_Chebyshev(T
                  x) {
        gsl_cheb_series *cs = gsl_cheb_alloc(Taylor::N<T> + 1);

        gsl_function F = {.function = &f};

        gsl_cheb_init(cs, &F,
                      -1, 1);

        T res = gsl_cheb_eval(cs, x);

        gsl_cheb_free(cs); // for proper coding

        return
                res;
    }

    namespace experimental {
        constexpr int get_a(int n, int k) {
            if (n % 2 == 0) {
                if (k % 2 == 0)
                    return 0;
                return 2 * n;
            }
            if (k % 2 == 1) // n is odd now
                return 0;
            if (k == 0)
                return n;
            return 2 * n;
        }

        constexpr int get_T0(int n) {
            if ((n & 1) == 1)
                return 0;
            if ((n & 3) == 0)
                return 1;
            return -1;
        }

        template<typename T>
        constexpr T
        ChebyshevExp(T
                     x) {
            const std::size_t SIZE = Taylor::N<T> + 1;

            T a_data[SIZE * SIZE] = {0.0};

            for (
                    int k = 0;
                    k < SIZE;
                    ++k) {
                if (k == SIZE - 1) {
                    for (
                            int n = 0;
                            n < SIZE;
                            ++n)
                        a_data[
                                k * SIZE
                                + n] =
                                get_T0(n);
                } else {
                    a_data[
                            k * SIZE
                            + k] = -1.0;
                    for (
                            int n = k + 1;
                            n < SIZE;
                            ++n)
                        a_data[
                                k * SIZE
                                + n] =
                                get_a(n, k
                                );
                }
            }

            T b_data[SIZE] = {0.0};
            b_data[SIZE - 1] = 1.0;

// TODO: here we need to somehow implement gsl_matrix_float_view and gsl_matrix_long_double_view
            gsl_matrix_view A = gsl_matrix_view_array(a_data, SIZE, SIZE);
            gsl_vector_view b = gsl_vector_view_array(b_data, SIZE);

            int s;
            gsl_permutation *p = gsl_permutation_alloc(SIZE);
            gsl_vector *sol = gsl_vector_alloc(SIZE);
            gsl_linalg_LU_decomp(&A
                    .matrix, p, &s);
            gsl_linalg_LU_solve(&A
                    .matrix, p, &b.vector, sol);
            gsl_cheb_series *cs = gsl_cheb_alloc(Taylor::N<T>);
            cs->
                    c = sol->data;
            cs->c[0] *= 2;
            cs->
                    order = SIZE;
            cs->
                    a = -1;
            cs->
                    b = 1;
            return
                    gsl_cheb_eval(cs, x
                    );
        }
    } // namespace experimental
} // namespace ADAAI::Exp::Core::Chebyshev
