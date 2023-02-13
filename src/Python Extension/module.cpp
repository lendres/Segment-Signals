#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "SegmentSignalFunctions.h"
#include "SegmentSignal.h"
#include "SegmentationResults.h"
#include "SegmentationResultsPy.h"

namespace py = pybind11;

PythonAlgorithms::SegmentationResults* Segment(py::array_t<double> signalAsPyList, int signalLength, double threshold, int jumpSequenceWindowSize, int noiseVarianceWindowSize)
{
    py::buffer_info info        = signalAsPyList.request();
    double* signalDataPointer   = static_cast<double*>(info.ptr);

    Algorithms::SegmentationResults*		cppResults		= Algorithms::SegmentSignal::Segment(signalDataPointer, signalLength, threshold, jumpSequenceWindowSize, noiseVarianceWindowSize);
    PythonAlgorithms::SegmentationResults*	pythonResults	= new PythonAlgorithms::SegmentationResults(cppResults);
	
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