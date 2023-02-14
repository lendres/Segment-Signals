#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "SegmentSignalFunctions.h"
#include "SegmentSignal.h"
#include "SegmentationResults.h"
#include "SegmentationResultsPy.h"

#include <iostream>
#include <fstream>
#include <iomanip>      // std::setprecision

namespace py = pybind11;

PythonAlgorithms::SegmentationResults* Segment(py::array_t<double> signalAsPyList, double threshold, int jumpSequenceWindowSize, int noiseVarianceWindowSize, int noiseVarianceEstimateMethod, int maxSMLRIterations)
{
	// Gets the information about the object and a pointer to the actual data (buffer).
    py::buffer_info info        = signalAsPyList.request();
    double* signalDataPointer   = static_cast<double*>(info.ptr);

	ofstream myfile;
	myfile.open("C:\\Projects\\ddosi\\Segment Signals\\src\\Python Extension\\debugoutput.txt");
	myfile << "Number of dimensions: " << info.ndim << std::endl;
	for (int i = 0; i < info.ndim; i++)
	{
		myfile << "Dimension " << i << " size: " << info.shape[i] << std::endl;
	}


	// Make sure a 1-dimensional array/list was passed and not a scalar, two-dimensional, or other.
	if (info.ndim != 1)
	{
		myfile << "Error thrown.";
		throw std::runtime_error("Error: The array passed to \"Segment\" function is not 1 dimensional.");
	}



	// Get the length of the array/list.  It is required for the lower level functions operation on C arrays.
	int signalLength = static_cast<int>(info.shape[0]);

	myfile << "Signal length " << signalLength << std::endl;


	// Call the algorithm, then convert the results for returning to Python.
    Algorithms::SegmentationResults*		cppResults		= Algorithms::SegmentSignal::Segment(signalDataPointer, signalLength, threshold, jumpSequenceWindowSize, noiseVarianceWindowSize, static_cast<NoiseVarianceEstimateMethod>(noiseVarianceEstimateMethod), maxSMLRIterations);
    PythonAlgorithms::SegmentationResults*	pythonResults	= new PythonAlgorithms::SegmentationResults(cppResults);
	
	myfile << std::setprecision(12);
	myfile << std::endl <<  "CPP RESULTS" << std::endl;
	myfile << "Segment density: " << cppResults->GetSegmentDensity() << std::endl;
	myfile << "Jump sequence variance: " << cppResults->GetJumpSequenceVariance() << std::endl;

	myfile << std::endl <<  "PYTHON RESULTS" << std::endl;
	myfile << "Segment density: " << pythonResults->GetSegmentDensity() << std::endl;
	myfile << "Jump sequence variance: " << pythonResults->GetJumpSequenceVariance() << std::endl;

	myfile.close();

	// The C results are not longer needed.  Everything has been copied to the Python results.
	delete cppResults;

    return pythonResults;
}

PYBIND11_MODULE(SegmentSignalPy, m)
{
    m.def("Segment", &Segment, "Segments a signal based on maximum likelihood estimation");

    //py::class_<Algorithms::SegmentSignal>(m, "SegmentSignal")
    //    .def_static("Segment", static_cast<Algorithms::SegmentationResults* (*)(double[], int, double, int, int)>(&Algorithms::SegmentSignal::Segment));

    py::class_<PythonAlgorithms::SegmentationResults>(m, "SegmentationResults")
        .def(py::init<>())
		.def_property_readonly("SignalLength",			&PythonAlgorithms::SegmentationResults::GetSignalLength)
		.def_property_readonly("BinaryEventSequence",	&PythonAlgorithms::SegmentationResults::GetBinaryEventSequence)
        .def_property_readonly("NumberOfBinaryEvents",	&PythonAlgorithms::SegmentationResults::GetNumberOfBinaryEvents)
        .def_property_readonly("FilteredSignal",		&PythonAlgorithms::SegmentationResults::GetFilteredSignal)
        .def_property_readonly("SegmentedLog",			&PythonAlgorithms::SegmentationResults::GetSegmentedLog)
        .def_property_readonly("NoiseVariance",			&PythonAlgorithms::SegmentationResults::GetNoiseVariance)
        .def_property_readonly("JumpSequenceVariance",	&PythonAlgorithms::SegmentationResults::GetJumpSequenceVariance)
        .def_property_readonly("SegmentDensity",		&PythonAlgorithms::SegmentationResults::GetSegmentDensity)
        .def_property_readonly("Iterations",			&PythonAlgorithms::SegmentationResults::GetIterations)
        .def_property_readonly("Error",					&PythonAlgorithms::SegmentationResults::GetError);

    #ifdef VERSION_INFO
        m.attr("__version__") = VERSION_INFO;
    #else
        m.attr("__version__") = "dev";
    #endif
}