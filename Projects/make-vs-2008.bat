REM copies library compiled with Visual Studio 2008 and .h source file to Resources directories

copy "Microsoft Visual Studio 2008\Release\ExternalMediaLib.lib" "..\Modelica\ExternalMedia 3.2\Resources\Library\ExternalMediaLib.lib" /Y
copy "Microsoft Visual Studio 2008\Release\ExternalMediaLib.lib" "..\Modelica\ExternalMedia 3.2\Resources\Library\ExternalMediaLib.vs2008.lib" /Y
copy "Sources\externalmedialib.h" "..\Modelica\ExternalMedia 3.2\Resources\Include" /Y

copy "Microsoft Visual Studio 2008\Release\ExternalMediaLib.lib" "..\Modelica\ExternalMedia 3.2.1\Resources\Library\ExternalMediaLib.lib" /Y
copy "Microsoft Visual Studio 2008\Release\ExternalMediaLib.lib" "..\Modelica\ExternalMedia 3.2.1\Resources\Library\ExternalMediaLib.vs2008.lib" /Y
copy "Sources\externalmedialib.h" "..\Modelica\ExternalMedia 3.2.1\Resources\Include" /Y


