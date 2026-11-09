import os
from   setuptools                     import setup
from   setuptools                     import Extension
from   pybind11                       import get_include


sourceDirectory = os.path.join("src", "CPP Static Library")
pythonDirectory = os.path.join("src", "Python Extension")

# PyQt5 in Spyder can load its bundled MSVCP140.dll (14.26) before this
# extension. It cannot lock a mutex initialized by the newer MSVC STL's
# constexpr constructor. The flag restores runtime initialization, so
# mutex construction makes a DLL call and loses constant initialization.
# Remove this once updated PyQt and all supported Windows hosts actually
# load a runtime at least as new as the MSVC toolset used for the extension
# (14.51 for the affected build). Check the DLL loaded in the process,
# including any bundled copy, then rebuild without the flag, import QtCore
# before SegmentSignalPy, and run the full suite in a fresh Spyder kernel.
windowsMacros = [("_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR", "1")] if os.name == "nt" else []

setup(
    ext_modules=[
        Extension(
            name="SegmentSignalPy",
            sources=[
                os.path.join(pythonDirectory, file)
                for file in ["module.cpp", "SegmentationResultsPy.cpp"]
            ] + [
                os.path.join(sourceDirectory, file)
                for file in ["SegmentSignalFunctions.cpp", "SegmentationResults.cpp", "SegmentSignal.cpp"]
            ],
            include_dirs=[get_include(), sourceDirectory],
            language="c++",
            define_macros=windowsMacros
        )
    ]
)
