#include "ilp_solver_interface.hpp"

#include <cassert>
#include <cmath>

using std::vector;

namespace ILPSolver
{
    int round(double x)
    {
        return (int) (x+0.5);
    }

    void test_sorting(ILPSolverInterface* p_solver)
    {
        int number_array[] = { 6, 2, 0, 4, 9, 7, 3, 5, 1, 8};
        const auto numbers = vector<int>(std::begin(number_array), std::end(number_array));
        const auto num_vars = (int) numbers.size();

        // xi - target position of numbers[i]
        //
        // min x0 + ... + x9
        // s.t. xk - xl >= 1 for every (k,l) for which numbers[k] > numbers[l]
        //      xi >= 0 integral

        // Add variables
        for (auto i = 0; i < num_vars; ++i)
            p_solver->add_variable_integer(1, 0, DBL_MAX, "x" + std::to_string(i));

        // Add constraints
        for (auto i = 0; i < num_vars; ++i)
        {
            for (auto j = i+1; j < num_vars; ++j)
            {
                // Reorder i and j (naming them k and l) s.t. numbers[k] > numbers[l]
                auto k = i;
                auto l = j;
                if (numbers[k] < numbers[l])
                    std::swap(k, l);

                vector<int> indices;
                vector<double> values;

                indices.push_back(k);
                values.push_back(1);

                indices.push_back(l);
                values.push_back(-1);

                p_solver->add_constraint_lower(indices, values, 1.0, "x" + std::to_string(k) + ">x" + std::to_string(l));
            }
        }

        p_solver->minimize();

        // Check correctness of objective
        const auto obj_value = p_solver->get_objective();
        const auto expected_obj_value = num_vars*(num_vars-1)/2;
        assert(fabs(obj_value - expected_obj_value) < 0.0001);  // objective should be 0+1+...+(num_numbers-1)

        // Check correctness of solution
        const auto permutation = p_solver->get_solution();
        auto sorted = vector<int>(num_vars, INT_MIN);       // sort according solution
        for (auto i = 0; i < num_vars; ++i)
        {
            const auto pos = round(permutation[i]);
            assert(fabs(pos - permutation[i]) < 0.0001);    // solution must be integral
            sorted[pos] = numbers[i];
        }
        for (auto i = 0; i < num_vars; ++i)
        {
            assert(sorted[i] != INT_MIN);                   // solution must be a permutation
            if (i > 0)
                assert(sorted[i-1] < sorted[i]);            // solution must sort
        }
    }

    void test_linear_programming(ILPSolverInterface* p_solver)
    {
        const auto num_vars = 5;
        const auto num_dirs = num_vars;

        // expected solution
        double x0[num_vars] = { 2.72, 42.0, -1.41, 3.14, -1.62 };

        // constraint directions
        double a[num_dirs][num_vars] = {{ 1.24, -3.47, 8.32, 4.78, -5.34 },
                                        { -7.23, 4.90, -3.21, 0.39, 9.45 },
                                        { 2.40, 9.38, -6.67, -6.43, 5.38 },
                                        { -4.79, 1.47, 6.47, 4.30, -8.39 },
                                        { 8.32, -7.20, 4.96, -9.41, 3.64 }};

        double scalar[num_dirs] = { 7, 2, 5, 6, 3 };

        // objective c = sum_j scalar[j]*a[j] (conical combination)
        double c[num_vars];
        for (auto i = 0; i < num_vars; ++i)
        {
            c[i] = 0.0;
            for (auto j = 0; j < num_dirs; ++j)
                c[i] += scalar[j]*a[j][i];
        }

        // right hand sides b[j] = expected_solution*a[j] (scalar product)
        double b[num_dirs];
        for (auto j = 0; j < num_dirs; ++j)
        {
            b[j] = 0.0;
            for (auto i = 0; i < num_vars; ++i)
                b[j] += x0[i]*a[j][i];
        }

        // max c*(x0,...,x4)
        // s.t. b[j] - 10.0 <= a[j]*(x0,...,x4) <= b[j] for all j

        // Add variables
        for (auto i = 0; i < num_vars; ++i)
            p_solver->add_variable_continuous(c[i], -DBL_MAX, DBL_MAX, "x" + std::to_string(i));

        // Add constraints
        for (auto j = 0; j < num_dirs; ++j)
        {
            auto indices = vector<int>();
            for (auto i = 0; i < num_vars; ++i)
                indices.push_back(i);
            const auto values = vector<double>(a[j], a[j] + num_vars);

            p_solver->add_constraint_lower(indices, values, b[j] - 10.0, "x*dir" + std::to_string(j) + " >= b" + std::to_string(j) + " - 10");
            p_solver->add_constraint_upper(indices, values, b[j],        "x*dir" + std::to_string(j) + " <= b" + std::to_string(j));
        }

        p_solver->minimize();
        
        // Check correctness of solution
        const auto x = p_solver->get_solution();
        for (auto i = 0; i < num_vars; ++i)
            assert(fabs(x[i] - x0[i]) <= 0.001);
    }
}