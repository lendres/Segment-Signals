from   distutils.core                            import setup
from   distutils.core                            import Extension
from   distutils                                 import sysconfig
import pybind11


sourceDirector = "..\\CPP Static Library\\"

sfc_module = Extension(
    "SegmentSignalPy",
    sources=["module.cpp", "SegmentationResultsPy.cpp", sourceDirector+"SegmentSignalFunctions.cpp", sourceDirector+"SegmentationResults.cpp", sourceDirector+"SegmentSignal.cpp"],
    include_dirs=[pybind11.get_include(), sourceDirector],
    language="c++",
    extra_compile_args=[],
)

setup(
    name="SegmentSignalPy",
    version="1.0",
    description="Signal segmentation algorithm by S.Radhakrishnan, et al.  This is a Python interface for the C++ algorithm.",
    ext_modules=[sfc_module],
)

"""
To install:
- Open the Anaconda prompt.
- Activate your environment.
- Navigate to this directory.
- Run: "pip install ."
- Restart the kernal.
"""