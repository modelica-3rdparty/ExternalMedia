REM builds release of ExternalMedia with lib file compiled by Visual Studio 2005

rmdir /S /Q "ExternalMedia 3.2 VS2005"
mkdir "ExternalMedia 3.2 VS2005"
cd "ExternalMedia 3.2 VS2005"
mkdir ExternalMedia
cd ExternalMedia
xcopy "..\..\..\..\Modelica\ExternalMedia 3.2\*.*" . /S
mkdir Resources\Include
copy ..\..\..\Sources\externalmedialib.h .\Resources\Include
mkdir Resources\Library
copy "..\..\..\Microsoft Visual Studio 2005\Release\ExternalMediaLib.lib" .\Resources\Library


