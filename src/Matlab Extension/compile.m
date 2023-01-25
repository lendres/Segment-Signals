# This script compiles the library for use as a mex file with Matlab or Octave.  It
# works as a standalone compiler.  That is, it does not try to link to prebuilt object
# or library files.

# Clears the command window, the clears the workspace.  It is necessary to clear the
# workspace because if the output file is in use, it will be locked and the compiler
# will not be able to overwrite it.
clc
clear all

disp(" ")

# The relative path to the main library source code files.
includeDirectory        = strcat("-I", "..\\CPP Static Library\\")

# The intermediate directory where object files are created.  The linker needs it to
# be able to find the files.
intermediateDirectory   = strcat("-L", getenv("TEMP"), "\\")

# The mex call is done as a function call so that the variables are evaluated and the
# strings passed instead of the literal names of the variables.
# Use "-v" for verbose outptu.
mex(includeDirectory, intermediateDirectory, "SegmentSignalMex.cpp")

disp("\nDone compiling.\n")
