# ExternalMedia Change Log

## v4.1.2 - unreleased
- Uses Modelica Standard Library 4.1.0
- CoolProp updated to v8.0.0, the library is now compiled as C++17
- Fixed a crash in OpenModelica with tabular CoolProp media, e.g. `CO2|enable_BICUBIC=1`
- `partialDeriv_state` works again with CoolProp, it used derivative names like `drhodp|h` that
  CoolProp no longer accepts, and the resulting exception terminated the process
- An unknown CoolProp fluid or backend name now raises an ExternalMedia error instead of
  terminating the process
- The OpenModelica DLL is built with the UCRT64 environment of MSYS2 (OMDevUCRT), as used by
  OpenModelica 1.23 and later, and installed in Resources/Library/ucrt64
- MinGW builds link libgcc, libstdc++ and winpthread statically
- Removed the 32-bit MinGW DLL (`win32/ExternalMediaLib.dll`), since OpenModelica has no 32-bit
  Windows build. There are no binaries for OpenModelica 1.22 and older anymore.
- New example media for R134a (CoolProp with and without TTSE and bicubic tables)
- The library exports the `TwoPhaseMedium_setState_xx_C_impl_wrap` functions, which fixes the error
  "External function ... could not be found" when OpenModelica evaluates setState_xx in parameters (#113)
- Added binaries for Visual Studio 2026
- The macOS binaries require macOS 11 or later
- The CoolProp solver keeps the results of recent setState_xx, setSat_p and setSat_T calls and
  returns them again for exactly the same inputs, which avoids repeated CoolProp calls during
  numerical Jacobian evaluations. The new substance option `cache_size` (default 1024, 0 switches
  the cache off) sets the number of stored results per medium, e.g. `R134a|cache_size=4096`.
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
