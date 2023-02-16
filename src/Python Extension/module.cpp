#include <stdexcept>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
//#include <pybind11/stl.h>

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
    py::buffer_info info		= signalAsPyList.request();
    double* signalDataPointer	= static_cast<double*>(info.ptr);

	// Make sure a 1-dimensional array/list was passed and not a scalar, two-dimensional, or other.
	if (info.ndim != 1)
	{
		throw std::runtime_error("Error: The array passed to \"Segment\" function is not 1 dimensional.");
	}

	// Get the length of the array/list.  It is required for the lower level functions operation on C arrays.
	int signalLength = static_cast<int>(info.shape[0]);

	// Call the algorithm, then convert the results for returning to Python.
    Algorithms::SegmentationResults*		cppResults		= Algorithms::SegmentSignal::Segment(signalDataPointer, signalLength, threshold, jumpSequenceWindowSize, noiseVarianceWindowSize);
    PythonAlgorithms::SegmentationResults*	pythonResults	= new PythonAlgorithms::SegmentationResults(cppResults);
	
	// The C results are not longer needed.  Everything has been copied to the Python results.
	delete cppResults;

    return pythonResults;
}


py::array_t<int> FindSignificantZones(py::array_t<int> binaryEvents, py::array_t<double> xData, double threshold, bool includeBoundries)
{
	// Gets the information about the object and a pointer to the actual data (buffer).
	py::buffer_info eventsInfo	= binaryEvents.request();
	int* binaryEventsPointer	= static_cast<int*>(eventsInfo.ptr);
	py::buffer_info xDataInfo	= xData.request();
	double* xDataPointer		= static_cast<double*>(xDataInfo.ptr);

	// Make sure a 1-dimensional array/list was passed and not a scalar, two-dimensional, or other.
	if (eventsInfo.ndim != 1 || xDataInfo.ndim != 1)
	{
		throw std::runtime_error("Error: An array passed to \"FindSignificantZones\" function is not 1 dimensional.");
	}

	// Make sure a data is the same length.
	if (eventsInfo.shape[0] != xDataInfo.shape[0])
	{
		throw std::runtime_error("Error: Arrays passed to \"FindSignificantZones\" function are not the same length.");
	}

	// Get the length of the array/list.  It is required for the lower level functions operation on C arrays.
	int signalLength = static_cast<int>(eventsInfo.shape[0]);

	std::vector<std::array<int, 2>>* cppResults = Algorithms::SegmentSignal::FindSignificantZones(binaryEventsPointer, xDataPointer, signalLength, threshold, includeBoundries);

	//py::array_t<py::array_t<int>> pyResults = py::array_t<py::array_t<int>>(results->size());
	//py::array_t<py::array_t<int>>* pyResultsBuffer	= static_cast<py::array_t<py::array_t<int>>*>(pyResults.request().ptr);

	//for (int i = 0; i < results->size(); i++)
	//{
	//	vector<int> indexSet = (*results)[i];

	//	pyResultsBuffer[i] = py::array_t<int>(2);
	//	int* pyIndexSetPointer = static_cast<int*>(pyResultsBuffer[i].request().ptr);
	//	pyIndexSetPointer[0] = indexSet[0];
	//	pyIndexSetPointer[1] = indexSet[1];
	//};

	//py::array_t<double>		pyResults = py::array_t<double>(2);
	//py::array_t<double> pyResults = py::array_t<double>(2);
	size_t size					= cppResults->size();
	py::array_t<int> pyResults	= py::array_t<int>(2*size);
	int* pyResultsBuffer		= static_cast<int*>(pyResults.request().ptr);

	for (int i = 0; i < size; i++)
	{
		std::array<int, 2> indexSet	= (*cppResults)[i];
		pyResultsBuffer[2*i]		= indexSet[0];
		pyResultsBuffer[2*i+1]		= indexSet[1];
	}

	
	ofstream myfile;
	myfile.open("C:\\Projects\\ddosi\\Segment Signals\\src\\Python Extension\\debugoutput.txt");
	myfile << "Events dimensions: " << eventsInfo.ndim <<  std::endl;
	myfile << "Events length: " << eventsInfo.shape[0] <<  std::endl;
	myfile << "X data dimensions: " << xDataInfo.ndim <<  std::endl;
	myfile << "X data length: " << xDataInfo.shape[0] <<  std::endl;
	myfile << "Threshold: " << threshold <<  std::endl;
	myfile << "" <<  std::endl;

	myfile << "Binary events" <<  std::endl;
	for (int i = 0; i < size; i++)
	{
		myfile << binaryEventsPointer[i] << " ";
	}

	myfile << std::endl << std::endl << "X Data" <<  std::endl;
	for (int i = 0; i < size; i++)
	{
		myfile << xDataPointer[i] << " ";
	}

	myfile << std::endl << std::endl << "CPP" << std::endl;
	for (int i = 0; i < size; i++)
	{
		std::array<int, 2> indexSet	= (*cppResults)[i];
		myfile << indexSet[0] << "\t" << indexSet[1];
	}

	myfile << std::endl << std::endl << "Python Before Resize" << std::endl;
	for (int i = 0; i < size; i++)
	{
		myfile << pyResultsBuffer[2*i] << "\t" << pyResultsBuffer[2*i+1];
	}


	pyResults.resize({(int)size, 2});


	myfile.close();


	delete cppResults;

	return pyResults;
}


PYBIND11_MODULE(SegmentSignalPy, m)
{
    m.def("Segment", &Segment, "Segments a signal based on maximum likelihood estimation");

	m.def("FindSignificantZones", &FindSignificantZones, "Post processes a binary event sequence to find regions that are greater than the specified threashold.");

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