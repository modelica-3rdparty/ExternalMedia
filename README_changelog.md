# ExternalMedia Change Log

## v4.2.0 - unreleased
- New CoolProp option enable_SVDSBTL, e.g. "Water|enable_SVDSBTL=1". It uses the
  SVDSBTL tables of CoolProp 8 only as starting values for T and density and then
  evaluates the reference equation of state (HEOS) at the input state with a few
  Newton steps, so all properties and derivatives are those of HEOS. For
  single-phase p-h states this was 7 to 20 times faster than the plain model in
  the tests (water, CO2, R134a) with results within 4e-5 of it. Pure fluids with
  the HEOS (and, untested, REFPROP) backends only. The first use of a fluid builds
  the tables, which takes up to a few minutes. See UserGuide.Usage.CoolPropSVDSBTL.
- New example media WaterCoolPropSVDSBTL, CO2CoolPropSVDSBTL, R134aCoolPropSVDSBTL
  and test suite Test.CoolProp.CO2_SVDSBTL

## v4.1.2 - unreleased
- Uses Modelica Standard Library 4.1.0
- CoolProp updated to v8.0.0, the library is now compiled as C++17
- New example media for R134a (CoolProp with and without TTSE and bicubic tables)
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
