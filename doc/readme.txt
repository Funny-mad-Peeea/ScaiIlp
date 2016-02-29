Building pthreads-win32 with VS 2012 (only if Cbc should support multi-threading)
=================================================================================

(1) Download pthreads-win32 from the Pthreads-Win32 project (https://sourceware.org/pthreads-win32/)

Download: ftp://sourceware.org/pub/pthreads-win32/

(2) Open pthread.dsw with VS 2012 and let it upgrade the project

(3) Right-click onto the project "pthread" in the Solution Explorer and choose "Properties" -> "Configuration Properties".
    Select "All Configurations" and use the following settings:

  General / Output Directory:       $(SolutionDir)$(Configuration)\
  General / Intermediate Directory: $(SolutionDir)obj\$(Configuration)\
  General / Platform Toolset:       v110_xp

  C/C++ / General             / Debug Information Output:       Program Database (/Zi)
  C/C++ / Preprocessor        / Preprocessor Definitions:       prepend "_ITERATOR_DEBUG_LEVEL=0;" (without double quotes)
  C/C++ / Precompiled Headers / Precompiled Header Output File: $(IntDir)pthread.pch
  C/C++ / Output Files        / ASM List Location:              $(IntDir)
  C/C++ / Output Files        / Object File Name:               $(IntDir)
  C/C++ / Output Files        / Program Database File Name:     $(IntDir)

  Linker / General   / Output File:         $(OutDir)$(TargetName)$(TargetExt)
  Linker / Debugging / Generate Debug Info: Yes (/DEBUG)
  Linker / Advanced  / Import Library:      $(OutDir)$(TargetName).lib

  If you want multi processor compilation:

  C/C++ / General         / Multi-processor Compilation: Yes (/MP)
  C/C++ / Code Generation / Enable Minimal Rebuild:      No (/Gm-)

(4) In the Solution Explorer, find the filter "Resource Files", right-click onto "version.rc" and choose "Properties".
    Select "All Configurations" and use the following settings:

  Resources / General / Preprocessor Definitions: prepend "PTW32_ARCHx86;" (without double quotes) when compiling for 32 bit platforms
                                                  or      "PTW32_ARCHx64;" (without double quotes) otherwise.


Building Cbc with VS 2012
=========================

(1) Download Cbc from the COIN project (http://www.coin-or.org/).

Download: http://www.coin-or.org/download/source/Cbc/
via SVN:  svn co https://projects.coin-or.org/svn/Cbc/stable/2.8 coin-Cbc
Wiki:     https://projects.coin-or.org/Cbc/wiki

(2) Open Cbc\MSVisualStudio\v10\Cbc.sln with VS 2012

(3) Let VS update the projects

(4) Right-click onto "Solution" in the Solution Explorer and choose "Properties". Then use the following settings:

    Common Properties / Project Dependencies:

      - Choose "libOsiCbc" from "Projects" and check the following 2 projects:

         (1) libCbc
         (2) libOsi

      - Choose "libCbc" from "Projects" and check the following 5 projects:

         (1) libCgl
         (2) libClp
         (3) libCoinUtils
         (4) libOsi
         (5) libOsiClp

(5) In the Solution Explorer, mark the following 7 projects:

    (1) libCbc
    (2) libCgl
    (3) libClp
    (4) libCoinUtils
    (5) libOsi
    (6) libOsiCbc
    (7) libOsiClp

(6) Right-click onto the marked projects and choose "Properties" -> "Configuration Properties". Select "All Configurations" and use the following settings:

    General / Output Directory:        $(SolutionDir)$(Configuration)\
    General / Intermediate Directory:  $(SolutionDir)obj\
    General / Platform Toolset:        v110_xp

    C/C++   / Preprocessor / Preprocessor Definitions:   prepend "_ITERATOR_DEBUG_LEVEL=0;" (without double quotes)
    C/C++   / Output Files / Program Database File Name: $(OutDir)$(TargetName).pdb

    For faster compilation, you can additionally use the following settings (set none or both):

    C/C++   / General         / Multi-processor Compilation:  Yes
    C/C++   / Code Generation / Enable Minimal Rebuild:       No

(7) If you want to support multithreading: Ensure that you have proceeded as described in "Building pthreads-win32 with VS 2012"

    Right-click onto the project "libCbc" in the Solution Explorer and choose "Properties" -> "Configuration Properties".
    Select "All Configurations" and use the following settings:

    C/C++     / General      / Additional Include Directories: <path to pthreads-win32>
    C/C++     / Preprocessor / Preprocessor Definitions:       prepend "CBC_THREAD;" (without double quotes)
    Librarian / General      / Additional Dependencies:        pthread.lib
    Librarian / General      / Additional Library Directories: <path to pthreads-win32>\$(Configuration)

(8) Choose "Build" -> "Batch-Build" and select the following projects and configurations:

    libOsiCbc  Debug    Win32
    libOsiCbc  Release  Win32


Building IlpSolverDll with VS 2012
==================================

(1) Ensure that you have built Cbc as described above.

(2) Specify the location of Cbc by setting the environment variable CBC_DIR.

(3) Build IlpSolverDll and IlpSolverUnitTest.


Class Structure
===============

The published solver interface is ILPSolverInterface. To create a concrete solver, you must call create_XYZ_solver() from ilp_solver_factory.hpp.
To destroy the solver later, you MUST call destroy_solver() instead of deleting the pointer yourself.

Class Hierarchy:

ILPSolverInterface: published Interface
|
|-> ILPSolverInterfaceImpl: implements all methods of ILPSolverInterface, partially by introducing private virtual methods
    |
    |-> ILPSolverOsiModel:  implements all modelling methods (e.g., do_add_variable) of ILPSolverInterfaceImpl and all methods that influence the
        |                   modelling process (the prepare part of do_prepare_and_solve())
        |
        |-> ILPSolverCbc:   implements the remaining, solver specific methods for the Cbc solver
        |
        |-> ILPSolverOsi:   implements the remaining, solver specific methods for arbitrary solvers whose functionality is exposed via the
                            OsiSolverInterface

                            Currently, the run time limit and the maximum number of threads are ignored because the OsiSolverInterface does not provide
                            this functionality.

Adding a New Solver
===================

When you want to support a new solver, you must ask yourself at which level you want to hook into the class hierarchy.

(a) If you want to communicate with the solver via the OsiSolverInterface, then you derive a class from ILPSolverOsi, construct your solver and
    forward a pointer to this solver (its OsiSolverInterface) to the constructor of ILPSolverOsi.

    Note, however, that the OsiSolverInterface does not provide all the functionality that is exposed by ILPSolverInterface. If you can access
    the solver that is wrapped inside OsiSolverInterface, then you can overwrite* the method ILPSolverOsi::do_solve() to forward some information
    that is ignored in this method to the solver.

    *An alternative would be to modify ILPSolverOsi::do_solve() to forward the parameters that are currently ignored to a virtual function that
     can be implemented in a derived class and has an empty default implementation. But currently, there is no need for this, so we do not extra
     lines of code for this feature.

(b) If your solver is based on an LP-Solver it communicates with via the OsiSolverInterface and if your solver obtains its model via this LP-solver,
    then you should derive from ILPSolverOsiModel like Cbc does.

(c) As a last option, you can derive from ILPSolverInterfaceImpl and implement all the virtual methods yourself.
