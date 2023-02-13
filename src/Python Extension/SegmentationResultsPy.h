#ifndef SEGMENTATIONRESULTSPY_H
#define SEGMENTATIONRESULTSPY_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "SegmentationResults.h"

namespace py = pybind11;

namespace PythonAlgorithms
{
	class SegmentationResults
	{
		private:
			int						_signalLength;
			py::array_t<double>		_binaryEventSequence;
			int						_numberOfBinaryEvents;
			py::array_t<double>		_filteredSignal;
			py::array_t<double>		_segmentedLog;
			py::array_t<double>		_noiseVariance;
			double					_jumpSequenceVariance;
			double					_segmentDensity;
			int						_iterations;
			int						_error;

		public:
			/// <summary>
			/// Default constructor.
			/// </summary>
			SegmentationResults();
			
			/// <summary>
			/// Constructor.
			/// </summary>
			/// <param name="segmentationResults">SegmentationResults which will be converted for use in Python.</param>
			SegmentationResults(Algorithms::SegmentationResults* segmentationResults);

			/// <summary>
			/// destructor.
			/// </summary>
			~SegmentationResults();

		public:
			/// <summary>
			/// Length of the input signal.  The output sequences are the name length.
			/// </summary>
			int GetSignalLength();

			/// <summary>
			/// Array that contains 1s at segmented log boundaries and 0s elsewhere.
			/// </summary>
			py::array_t<double> GetBinaryEventSequence();

			/// <summary>
			/// Number of binary event sequences detected (number of "1"s in the BinaryEventSequence array).
			/// </summary>
			int GetNumberOfBinaryEvents();

			/// <summary>
			/// Filtered estimate of the signal.
			/// </summary>
			py::array_t<double> GetFilteredSignal();

			/// <summary>
			/// Average of filter log for each segment.
			/// </summary>
			py::array_t<double> GetSegmentedLog();

			/// <summary>
			/// Estimate noise variance.
			/// </summary>
			py::array_t<double> GetNoiseVariance();

			/// <summary>
			/// Estimated variance of the jump sequence.
			/// </summary>
			double GetJumpSequenceVariance();

			/// <summary>
			/// Estimate segment density.
			/// </summary>
			double GetSegmentDensity();
		
			/// <summary>
			/// SMLR iteration count.
			/// </summary>
			int GetIterations();

			/// <summary>
			/// Error flag.
			///		Zero - no error.
			///		Less than zero - invalid event density estimated after threshold.  Reduce/increase
			///			threshold and rerun.
			///		Greater than zero - logarithm argument became zero at sample number returned in error during
			///			the calculation of likelihood ratios in SMLR.  There may be more samples of this type which
			///			may give rise to this problem.  Edit/rescale data values and rerun.
			/// </summary>
			int GetError();

	}; // End class.
} // End namespace.

#endif