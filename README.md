# Segment Signals
A signal segmentation algorithm.

## Introduction
This library implements the algorithm found in:
[_Segmentation of well logs by maximum likelihood estimation: the algorithm and FORTRAN-77 implementation_](https://www.sciencedirect.com/science/article/pii/0098300491900237) by S.Radhakrishnan, G.Srikanth, and C.H.Mehta

The original algorithm was written in Fortran.  This version converts it to C++ and C#.  In addition, the following changes have been made:
	- Converted the binary event array (Q) from double to int.  Likely, the original was a double to keep all options as floats and prevent accidental round off.  C/C++ 
		automatically perform calculations as doubles when any of the parameters is a double.  The binary event sequence is, of course, meant to be binary, so conversion to int is logical.  This also meant a few lines could be simplified (such as comparisons).  E.g. you could directly test "if (Q[i])" rather than requiring the
		inequality.
	- Removed a bug from the GETSPK/GetSpikeyZone function.  The fix prevents over running the end of an array.
	- Addition of "FindSignificantZones" to allow simple filtering (removal) of small zones.

## Organization
A high-level view of the organization is as follows.

- CPP Static Library
	- SegmentSignalFunctions.
		- **It is suggested not to use these directly, but to use the class `SegmentSignal` instead.**
		These files implement the original algorithm in C++.  The function names have changed to something more readable.  The original function names can be found in the comments in
		the .cpp file.  The functions have otherwise been directly converted from Fortran and should be easily comparable to those in the source paper.
	- SegmentSignal.*
		- A class interface to the functions that makes use of the algorithm easier.
	- SegmentationResults.*
		- Small class to return results of the algorithm.
	- NoiseVarianceEstimateMethod.h
		- Enumeration for the noise variance estimation method.

- C DLL
	- DLL wrapper around the main SegmentSignal function.  The DLL exposes a C function for calling.

- C Sharp DLL Wrapper
	- C# interface to the DLL.  Classes mirror those in C++.

-  Unit Tests
	- Testing projects.  Also provides examples.  See the "LogSeparateTest" functions for a basic example.

- Python Extensions
	- Builds a library for use with Python.  See the `src/Python Extensions/Read Me.txt` file in that project for more information.

## Compiling
The Python Extension library requires knowledge of your local Python. See the `src/Python Extensions/Read Me.txt` file in that directory for more information.
