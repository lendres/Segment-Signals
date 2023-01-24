#include "mex.h"
#include "SegmentSignal.h"


/*
  OCTAVE
  Compile by using the following at the command prompt:
  mkoctfile --mex [options] SegmentSignalMex.cpp


  The number of return arguments (# of left-hand side args).
  An array of pointers to return arguments.
  The number of input arguments (# of right-hand side args).
  An array of pointers to input arguments.
*/

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  mexPrintf ("Hello, World!\n");

  mexPrintf ("I have %d inputs and %d outputs\n", nrhs, nlhs);
}
