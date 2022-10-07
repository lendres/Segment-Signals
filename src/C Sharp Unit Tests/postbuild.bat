@echo off
rem First input is solution path.
rem Second input is the configuration name.
rem Third input is the platform name.
rem Use the "~" on the input parameters to strip the existing quotes.
set source="%~1%3\%2\SegmentSignalC.dll"
set destination="%~1C Sharp Unit Tests\bin\%2\"

echo.
echo Copying C DLL to C Sharp Unit Testing directory.
echo From: %source%
echo To: %destination%
echo.
xcopy %source% %destination% /y
echo.