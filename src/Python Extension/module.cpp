//#include <Windows.h>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "SegmentSignalFunctions.h"
#include "SegmentSignal.h"
#include "SegmentationResults.h"
#include "SegmentationResultsPy.h"

#include <iostream>
#include <fstream>
using namespace std;

namespace py = pybind11;

const double e = 2.7182818284590452353602874713527;

double sinh_impl(double x)
{
    return (1 - pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double cosh_impl(double x)
{
    return (1 + pow(e, (-2 * x))) / (2 * pow(e, -x));
}

double tanh_impl(double x)
{
    return sinh_impl(x) / cosh_impl(x);
}

struct Pet
{
    Pet(const std::string& name) : name(name) { }
    void setName(const std::string& name_) { name = name_; }
    const std::string& getName() const { return name; }

    std::string name;
};

// Passing in an array of doubles
py::array_t<double> twice(py::array_t<double> xs)
{
    py::buffer_info info = xs.request();
    double* ptr = static_cast<double*>(info.ptr);

    int n = 1;
    for (int r: info.shape)
    {
        n *= r;
    }

    for (int i = 0; i <n; i++)
    {
        *ptr++ *= 2;
    }

    return xs;
}

PythonAlgorithms::SegmentationResults* Segment(py::array_t<double> signalAsPyList, int signalLength, double threshold, int jumpSequenceWindowSize, int noiseVarianceWindowSize)
{
    py::buffer_info info        = signalAsPyList.request();
    double* signalDataPointer   = static_cast<double*>(info.ptr);



	ofstream myfile;
	myfile.open("C:\\Projects\\ddosi\\Segment Signals\\src\\Python Extension\\debugoutput.txt");
	for (int i = 0; i < signalLength; i++)
	{

		myfile << signalDataPointer[i] << std::endl;
	}
	myfile.close();



    Algorithms::SegmentationResults*		cppResults		= Algorithms::SegmentSignal::Segment(signalDataPointer, signalLength, threshold, jumpSequenceWindowSize, noiseVarianceWindowSize);
    PythonAlgorithms::SegmentationResults*	pythonResults	= new PythonAlgorithms::SegmentationResults(cppResults);
	
	//delete cppResults;

    return pythonResults;
}


PYBIND11_MODULE(SegmentSignalPy, m)
{
    m.def("sinh_impl", &sinh_impl, "Compute a hyperbolic sin of a single argument expressed in radians.");
    m.def("fast_tanh2", &tanh_impl, "Compute a hyperbolic tangent of a single argument expressed in radians.");
    m.def("twice", &twice, "Double every element in a list.");

    m.def("Segment", &Segment, "Segments a signal based on maximum likelihood estimation");

    py::class_<Pet>(m, "Pet")
        .def(py::init<const std::string&>())
        .def("setName", &Pet::setName)
        .def("getName", &Pet::getName);

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