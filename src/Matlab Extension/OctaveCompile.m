
clc

#oldPath = addpath("..\\CPP Static Library\\");
#newPath = path();
##oldPath
##disp("\n\n\n")
##newPath

disp(getenv("TEMP"))

isOctave                = exist("OCTAVE_VERSION", "builtin") ~= 0;
intermediateDirectory   = strcat("-L", "\"", getenv("TEMP"), "\\\"")
includeDirectory        = strcat("-I", "\"", "..\\CPP Static Library\\", "\"")

intermediateDirectory   = strcat("-Wl,-rpath=", "", getenv("TEMP"), "\\\"")


if isOctave
  # Run Octave mex compiler.
  disp("\nRunning Octave compiler.")

  #mex("-v", includeDirectory, intermediateDirectory, "SegmentSignalMex.cpp")
  #mex -v -I"..\\CPP Static Library\\" -L"C:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\" .\\SegmentSignalMex.cpp
  #mex -v -I"..\\CPP Static Library\\" "-Wl,-rpath=C:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\" .\\SegmentSignalMex.cpp
  mex -v -I"..\\CPP Static Library\\" eval(intermediateDirectory) .\\SegmentSignalMex.cpp
else
  # Run Matlab mex compiler.
end

disp("Done.")
