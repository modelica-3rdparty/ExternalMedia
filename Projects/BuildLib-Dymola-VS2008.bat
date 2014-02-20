REM ******** set the variables ************
REM call both to ensure that one works
call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat"

call BuildLib-VS

lib *.obj /OUT:ExternalMediaLib.lib
erase *.obj

echo "Copying files to External Media 3.2.1"
copy ExternalMediaLib.lib "..\Modelica\ExternalMedia 3.2.1\Resources\Library\win32\ExternalMediaLib.Dymola-vs2008.lib"
copy Sources\externalmedialib.h "..\Modelica\ExternalMedia 3.2.1\Resources\Include"
echo "All done"