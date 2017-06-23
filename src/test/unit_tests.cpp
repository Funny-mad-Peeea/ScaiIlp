// definiert Funktion init_unit_test bzw init_unit_test_suite
// #define BOOST_TEST_MAIN muss vor #include <boost/test/unit_test.hpp> stehen.
// Deshalb darf diese Datei nicht mit Precompiled Header uebersetzt werden.
#define BOOST_TEST_MAIN


#include "ilp_solver_exception.hpp"
#include "ilp_solver_factory.hpp"
#include "ilp_solver_exception_t.hpp"
#include "ilp_solver_interface.hpp"
#include "ilp_solver_interface_t.hpp"
#include "serialization_t.hpp"

#include <boost/test/unit_test.hpp>
#include <functional>
#include <string>
#include <vector>

const auto c_solver_exe_name = "ScaiIlpExe.exe";


int main(int p_argc, char* p_argv[])
{
    auto ret = unit_test_main(init_unit_test_suite, p_argc, p_argv );

    test_serialization();

    auto cbc_stub_solver_generator = []() { return ilp_solver::create_solver_stub(c_solver_exe_name); };
    execute_test_and_destroy_solver(cbc_stub_solver_generator(), c_solver_exe_name, ilp_solver::test_bad_alloc);
    ilp_solver::test_create_exception();

    return ret;
}
