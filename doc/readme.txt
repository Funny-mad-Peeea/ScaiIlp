Table of Contents
=================

1. General Information
   1.1 About
   1.2 Questions and Answers
2. Building
   2.1 Building Cbc with VS 2012
   2.2 Building IlpSolverDll with VS 2012
3. Code Structure
   3.1 Projects in Visual Studio
   3.2 Usage
   3.3 Class Hierarchy
   3.4 Adding a New Solver
   
   
1. Genral Information
=====================

1.1 About
---------

ScaiIlp can provide an interface to different ILP solvers.
Currently, the only supported solver is Cbc.


1.2 Questions and Answers
-------------------------

Q: What is the purpose of ScaiIlp?
A: There are two.
   1. Provide a unified and simplified interface for different ILP solvers.
   2. Allow dynamic linking without having to modify the sources/makefiles of solvers that do not support this natively.
   
Q: What about Osi? Why another interface?
A: On the one hand, Osi is quite a complex interface. As it spreads over several files with several classes, it is
   difficult to use as the interface of a DLL.
   On the other hand, some functions that we needed are missing in Osi.

Q: When should I use IlpSolverStub and ScaiIlpExe?
A: Having the solver in a separate process insulates it from your program.
   If the solver crashes, your program can survive this.
   On any crashes we know of, IlpSolverStub does silently the same as if the solver just found no solution.
   On unknown crashes and unknown problems, IlpSolverStub throws an exception, which can be caught in your code.
   
Q: When should I use IlpSolverCbc directly?
A: If you don't experience solver crashes, you can avoid some overhead by using IlpSolverCbc directly.


2. Building
===========

2.1 Building Cbc with VS 2012
-----------------------------

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

(7) Choose "Build" -> "Batch-Build" and select the following projects and configurations:

    libOsiCbc  Debug    Win32
    libOsiCbc  Release  Win32


2.2 Building IlpSolverDll with VS 2012
--------------------------------------

(1) Ensure that you have built Cbc as described above.

(2) Ensure that you have built Boost.

(3) Specify the location of Cbc by setting the environment variable CBC_DIR.

(4) Specify the location of Boost by setting the environment variable BOOST_DIR. Note that the include files must be located in $(BOOST_DIR)\include and the lib files must be located in $(BOOST_DIR)\lib.

(5) Build ScaiIlpDll, ScaiIlpExe, and UnitTest.


3. Code Structure
=================

3.1 Projects in Visual Studio
-----------------------------

The Visual Studio Solution (.sln) contains three projects:

- ScaiIlpDll creates ScaiIlp.dll
  ScaiIlp.dll contains the Cbc solver and a stub to communicate with ScaiIlp.exe
  It can be linked dynamically into other programs.
- ScaiIlpExe creates ScaiIlp.exe
  ScaiIlp.exe also contains the Cbc solver.
  ScaiIlp.exe can be started in a separate process and communicates via shared memory.
- UnitTest demonstrates usage for both of above projects.


3.2 Usage
---------

The published solver interface is ILPSolverInterface.
Include ilp_solver_interface.hpp.

3.2.1 Use as a DLL 

The recommended way to use ScaiIlp is to use it as a DLL (dynamic linking)
- Link against ScaiIlp.dll.
- Include ilp_solver_factory.hpp.
- Create your objects via create_solver_cbc() from ilp_solver_factory.hpp.
- To destroy the solver later, you MUST call destroy_solver() instead of deleting the pointer yourself.

3.2.2. Static linking

Alternatively, you may include your ilp_solver_cbc.cpp and all its dependencies in your project.
This way, your code gets statically linked with a part of ScaiIlp.

3.2.3 ScaiIlp.exe

To use ScaiIlp.exe, there is a class ILPSolverStub.
IlpSolverStub can be used like IlpSolverCbc either as described in 3.2.1 or as described in 3.2.2.
The constructor of IlpSolverStub and create_solver_stub() expect the base name of a solver executable (in the 
same directory, should be ScaiIlp, unless you rename it) and a name for a shared memory segment. 


3.3 Class Hierarchy
-------------------

ILPSolverInterface: published Interface
|
|-> ILPSolverInterfaceImpl: implements all methods of ILPSolverInterface, partially by introducing private virtual methods
    |
    |-> ILPSolverOsiModel:  implements all modelling methods (e.g., do_add_variable) of ILPSolverInterfaceImpl and all methods that influence the
    |   |                   modelling process (the prepare part of do_prepare_and_solve()) for arbitrary solvers whose modelling functionality is
    |   |                   exposed via the OsiSolverInterface
    |   |
    |   |-> ILPSolverCbc:   implements the remaining, solver specific methods for the Cbc solver
    |   |
    |   |-> ILPSolverOsi:   implements the remaining, solver specific methods for arbitrary solvers whose functionality is exposed via the
    |                       OsiSolverInterface
    |                       Currently, the run time limit and the maximum number of threads are ignored because the OsiSolverInterface does not
    |                       provide this functionality.
    |
    |-> ILPSolverCollect:   implements all modelling methods (e.g., do_add_variable) of ILPSolverInterfaceImpl and all methods that influence the
        |                   modelling process (the prepare part of do_prepare_and_solve()) and stores these data in ILPData.
        |
        |-> ILPSolverStub:  do_solve() writes the ILPData to shared memory and calls an external solver. The external solver writes the result
                            (in form of ILPSolutionData) back to the shared memory. The other methods of ILPSolverStub simply query ILPSolutionData.


3.4 Adding a New Solver
-----------------------

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

If you want your solver to be accessible via the DLL, then you must declare and define a function

  extern "C" ILPSolverInterface* __stdcall create_solver_xyz(<parameters>)

in ilp_solver_factory.hpp and ilp_solver_factory.cpp, respectively, and add this function in the module definition file ScaiIlpDll.def, which can be
found in vc\ScaiIlpDll.
