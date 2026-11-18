# ExternalMedia Change Log

## v4.1.2 - unreleased
- Uses Modelica Standard Library 4.1.0
- CoolProp updated to v8.0.0, the library is now compiled as C++17
- Added the enable_SVDSBTL option, a third table-based interpolation method next to
  enable_TTSE and enable_BICUBIC. It uses the SVD-compressed spline-based tables
  (SVDSBTL backend) introduced in CoolProp 8 for pure fluids.
- New example media for R134a (CoolProp with and without TTSE and bicubic tables)
  and for water, CO2 and R134a with SVDSBTL tables
- Added binaries for Visual Studio 2026
- The OpenModelica binaries for Windows are built with the MSYS2 UCRT64 toolchain of
  OpenModelica 1.23 and later and installed in Resources/Library/ucrt64. They are
  64 bit only and link the GCC runtime statically. There are no binaries for 32 bit
  OpenModelica or OpenModelica 1.22 and older anymore.
- The macOS binaries require macOS 11 or later

## v4.1.1 - 2025/11/03
- Uses Modelica Standard Library 4.1.0
- CoolProp updated to v7.2.0
- Added binaries for macOS ARM (M1/M2)

## v4.1.0 - 2025/08/01
- Uses Modelica Standard Library 4.1.0
- CoolProp updated to v6.5.0
- Fixed issue with ModelicaError that causes OMEdit to crash when opening ExternalTwoPhaseMedium

## v4.0.0 - 2023/04/19
- Compatible with Modelica Standard Library 4.0.0.
- Works out of the box in Dymola and OpenModelica under Windows and Linux.
- Uses dynamically linked libraries, no dependencies on specific compilers.
- No need to compile the binaries yourself, no special installation procedure required.
- CoolProp updated to v6.4.4 with some extra patches

## v3.3.2 - 2023/04/19
- Works out of the box in Dymola and OpenModelica under Windows and Linux.
- Uses dynamically linked libraries, no dependencies on specific compilers.
- No need to compile the binaries yourself, no special installation procedure required.
- CoolProp updated to v6.4.4 with some extra patches

## v3.3.1 - 2022/02/17
- Updated CoolProp to v6.4.1.
- Fixed problems with CoolProp interpolation tables.
- Added more precompiled binaries.
- Use git to retrieve the OpenModelica development environment.

## v3.3.0 - 2021/05/05
- The first release after a long period of inactivity.
- Added precompiled binaries in subfolders.
- Updated the documentation and restructured the help files.
