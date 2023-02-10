//#include <Windows.h>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "SegmentSignalFunctions.h"
#include "SegmentationResults.h"
#include "SegmentSignal.h"

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

Algorithms::SegmentationResults* Segment(py::array_t<double> xs, int signalLength, double threshold, int jumpSequenceWindowSize, int noiseVarianceWindowSize)
{
    py::buffer_info info = xs.request();
    double* ptr = static_cast<double*>(info.ptr);

    Algorithms::SegmentationResults* results = Algorithms::SegmentSignal::Segment(ptr, signalLength, threshold, jumpSequenceWindowSize, noiseVarianceWindowSize);

    return results;
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

    py::class_<Algorithms::SegmentSignal>(m, "SegmentSignal")
        .def_static("Segment", static_cast<Algorithms::SegmentationResults* (*)(double[], int, double, int, int)>(&Algorithms::SegmentSignal::Segment));

    py::class_<Algorithms::SegmentationResults>(m, "SegmentationResults")
        .def(py::init<double*, int, double*, double*, double*, double, double, int, int>())
        .def_property_readonly("BinaryEventSequence", &Algorithms::SegmentationResults::GetBinaryEventSequence)
        .def_property_readonly("NumberOfBinaryEvents", &Algorithms::SegmentationResults::GetNumberOfBinaryEvents)
        .def_property_readonly("FilteredSignal", &Algorithms::SegmentationResults::GetFilteredSignal)
        .def_property_readonly("SegmentedLog", &Algorithms::SegmentationResults::GetSegmentedLog)
        .def_property_readonly("NoiseVariance", &Algorithms::SegmentationResults::GetNoiseVariance)
        .def_property_readonly("JumpSequenceVariance", &Algorithms::SegmentationResults::GetJumpSequenceVariance)
        .def_property_readonly("SegmentDensity", &Algorithms::SegmentationResults::GetSegmentDensity)
        .def_property_readonly("Iterations", &Algorithms::SegmentationResults::GetIterations)
        .def_property_readonly("Error", &Algorithms::SegmentationResults::GetError);

    #ifdef VERSION_INFO
        m.attr("__version__") = VERSION_INFO;
    #else
        m.attr("__version__") = "dev";
    #endif
}