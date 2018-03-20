# test
Test MSSQL proxy project with SQL commands logging.

Built on Windows with QTCreator using:
    * MSVC 2017 compiler 15.0 x64 in c++17 mode
    * CMake 3.10
    * Boost 1.66.0

# Usage:

For all avaliable options please run:

*test.ext --help*

but application should normally run with default settings.

You can use:

*sqlcmd.exe -S tcp:127.0.0.1,1321*

to test proxy functionality.


