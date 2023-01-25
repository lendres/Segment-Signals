clc
clear all


disp(getenv("TEMP"))

isOctave                = exist("OCTAVE_VERSION", "builtin") ~= 0;
includeDirectory        = strcat("-I", "\"", "..\\CPP Static Library\\", "\"")

intermediateDirectory   = strcat("-L", "\"", getenv("TEMP"), "\\\"")
#intermediateDirectory   = strcat("\"", "-Wl,-rpath=", "", getenv("TEMP"), "\\\"")

disp(" ")
disp("Run command")

if isOctave
  # Run Octave mex compiler.
  disp("\nRunning Octave compiler.")

  #mex("-v", includeDirectory, intermediateDirectory, "SegmentSignalMex.cpp")
  #mex -v -I"..\\CPP Static Library\\" -L"C:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\" SegmentSignalMex.cpp

  #mex "-v" "-I..\\CPP Static Library\\" "-LC:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\" "SegmentSignalMex.cpp"
  #mex("-v", "-I..\\CPP Static Library\\", "-LC:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\", "SegmentSignalMex.cpp")

  #mex -v -I"..\\CPP Static Library\\" "-Wl,-rpath=C:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\" .\\SegmentSignalMex.cpp


  intermediateDirectory   = strcat("-L", getenv("TEMP"), "\\")
  #intermediateDirectory   = strcat("-L", "C:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\")
  includeDirectory        = strcat("-I", "..\\CPP Static Library\\")
  disp(" ")
  #mex("-v", includeDirectory, "-LC:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\", "SegmentSignalMex.cpp")
  mex("-v", includeDirectory, intermediateDirectory, "SegmentSignalMex.cpp")
  #mex("-v", "-I..\\CPP Static Library\\", intermediateDirectory, "SegmentSignalMex.cpp")
  #mex("-v", includeDirectory, intermediateDirectory, "SegmentSignalMex.cpp")
  #mex -v -I"..\\CPP Static Library\\" eval(intermediateDirectory) .\\SegmentSignalMex.cpp
  #executeString = ["mex -v -I\"..\\CPP Static Library\\\"", " ", intermediateDirectory, " ", "SegmentSignalMex.cpp"]
  #eval(executeString)
  #mex("-v", "-I\"..\\CPP Static Library\\\"", "-L\"C:\\Users\\LANCE~1.END\\AppData\\Local\\Temp\\\"", "SegmentSignalMex.cpp")
else
  # Run Matlab mex compiler.
end

disp("\n\nDone compiling.\n")

SegmentSignalMex(1, 2)
