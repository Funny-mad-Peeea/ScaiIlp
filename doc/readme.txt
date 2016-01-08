Building Cbc with VS 2012
=========================

(1) Download Cbc from the COIN project (http://www.coin-or.org/).

Download: http://www.coin-or.org/download/source/Cbc/
via SVN:  svn co https://projects.coin-or.org/svn/Cbc/stable/2.8 coin-Cbc
Wiki:     https://projects.coin-or.org/Cbc/wiki

(2) Open Cbc\MSVisualStudio\v10\Cbc.sln with VS 2012

(3) Let VS update the projects

(4) Right-click onto "Solution" in the solution explorer and choose "Properties". Then use the following settings:

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

(5) In the solution explorer, mark the following 7 projects:

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


Building IlpSolverDll with VS 2012
==================================

(1) Ensure that you have built Cbc as described above.

(2) Specify the location of Cbc by setting the environment variable CBC_DIR.

(3) Build IlpSolverDll and IlpSolverUnitTest.
