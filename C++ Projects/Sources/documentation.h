/*! 
  \mainpage External Media HowTo

  \section intro_sec Introduction
  
  Welcome to the ExternalMediaPackage HowTo guide. This package is meant to
  help you interfacing your favourite external fluid computation code(s)
  to Modelica, using the Modelica.Media interface. Currently, the
  ExternalMedia library only supports two-phase media, using Dymola 6 and
  Microsoft Visual Studio. In the future, we might add support for
  single-phase pure and mixture media, as well as for OpenModelica and
  gcc. Please contact the authors if you're interested in such
  developments.

  The ExternalMedia package has been implemented and is maintained by 
  Francesco Casella (casella@elet.polimi.it) and Christoph Richter 
  (ch.richter@tu-bs.de). The code is released under the Modelica license.

  Copyright (c) 2006, Politecnico di Milano and TU Braunschweig.

  \section architecture_sec 1. Architecture of the package

  At the top level there is a Modelica package (ExternalMedia), which
  contains all the basic infrastructure needed to use external fluid
  properties computation software through a Modelica.Media compliant
  interface. In particular, the
  ExternalMedia.Media.ExternalTwoPhaseMedium package is a full-fledged
  implementation of a two-phase medium model, compliant with the
  Modelica.Media.Interfaces.PartialTwoPhaseMedium interface. The
  ExternalTwoPhaseMedium package can be used with any external fluid
  property computation software; the specific software to be used is
  specified by changing the libraryName package constant, which is then
  handled by the underlying C code to select the appropriate external
  code to use.

  The functions within ExternalTwoPhaseMedium talk to a C/C++ interface
  layer (called externaltwophasemedium_lib.cpp) via external functions
  calls. This layer takes care of initializing the external packages, and
  maintains a collection of C++ objects (stored within a MediumMap C++
  object), corresponding to different instances of the medium model. More
  specifically, each instance of ExternalTwoPhaseMedium.BaseProperties
  corresponds to an instance of the C++ object TwoPhaseMedium, identified
  by an integer uniqueID; each TwoPhaseMedium object also contains a
  pointer to a C++ object inheriting from BaseSolver; this is a wrapper
  object around the actual external code (or solver) which has to be
  called to compute the fluid properties. It is possible to use multiple
  instances of possibly different solvers at the same time; the
  collection of active solver is managed by the SolverMap C++ object.

  The default implementation of the external medium works as follows. At
  initialization, each instance of the BaseProperties model calls the
  createMedium_() function of the interface layer. This triggers the
  allocation within the MediumMap object of a new TwoPhaseMedium object,
  which will contain all the medium properties, and a pointer to the
  appropriate solver. This object will be stored in the MediumMap, and
  identified with a uniqueID, which is returned by the createMedium()
  function and stored within the BaseProperties model. The&nbsp; solver
  object is created by the SolverMap object, based on the mediumName,
  libraryName and substanceName constants of the Modelica medium package
  - only one solver object is created for multiple instances of the same
  external medium.

  When one of the setState functions within BaseProperties is called, the
  interface layer will call the corresponding setState function of the
  TwoPhaseMedium object, which will in turn invoke the setState function
  of the appropriat solver object, to actually compute the fluid
  properties. These will be stored within the TwoPhaseMedium object, for
  later retrieval. The setState function then returns a
  ThermodynamicState record, which only contains integer uniqueID,
  indentifying the TwoPhaseMedium object in the MediumMap. Subsequent
  calls to compute any other functions of that state will just fetch the
  corresponding values from the TwoPhaseMedium object with the given
  uniqueID - the TwoPhaseMedium object acts like a cache for already
  computed properties. The default implementation assumes that all
  available properties are computed at once by the setState functions, so
  that subsequent calls to any property function will just result in a
  retrieval from the cache - more sophisticated implementations could be
  devised, where the setState function only computes some properties at
  first, and flags are provided within the TwoPhaseMedium object to keep
  into account what has already been computed and stored in the cache,
  and what has still to be computed if need.

  With the current implementation, it is not possible to use setState
  functions outside the BaseProperties model, unless one explicitly
  manages and sets the uniqueID integers - this is not possible if the
  user model has to be compatible with other Modelica.Media fluid models,
  which do not have the uniqueID. Support for calls to setState functions
  with automatic management of the uniqueID might be added in the future.

  \section using_sec 2. Using an already implemented external medium with Dymola

  If somebody else has already implemented the external medium, you only need a few 
  things to use it on your computer:

  - the ExternalMedia Modelica package
  - the ExternalTwoPhaseMedium.lib (and possibly the ExternalTwoPhaseMedium.dll) files, containing the compiled C/C++ code
  - the externaltwophasemedium.h C header file

  Copy the ExternalTwoPhaseMedium.lib file within the Dymola/bin/lib
  directory, the ExternalTwoPhaseMedium.dll file in the Windows/System32
  directory (only if the library has been compiled to a DLL), and the
  externaltwophasemedium.h file in the Dymola/Source directory.

  You can check that the compiler setup is working by running the
  TestMedium model in the ExternalMedia.Test package - this will simulate
  a dummy external fluid model, roughly corresponding to liquid water at
  low pressure and temperature.

  You can now use the external media provided in the ExternalMedia.Media
  package. There are two options here: the first is to use
  ExternalTwoPhaseMedium directly, using modifiers to change libraryName
  and substanceNames to suit your needs; the other is to use or modify
  one of the pre-packaged media. The libraryName string will be decoded
  by the SolverMap object - for further details on the syntax of the
  string, ask to the implementor of the interface to the code you'll
  actually use.

  \section development_sec 3. Developing a new external medium model

  The ExternalMedia package has been designed to ease your task, so that
  you will only have to write the mimum amount of code which is strictly
  specific to your external code - everything else is already provided.
  The following instructions apply if you want to develop an external
  medium model which include a (sub)set of the functions defined in
  Modelica.Media.Interfaces.PartialTwoPhaseMedium, and you want to
  compute all the properties at once when you call the setState function
  (this is a reasonable choice, since the majority of the time is
  usually spent to run preliminary computations (e.g. compute the
  Helmholtz function), and then computing all the properties only adds a
  small overhead on top of that time).

  First of all, you have to write you own solver object code; you can
  look at the code of the TestMedium and FluidPropMedium code as
  examples. Inherit from the BaseSolver object, which provides default
  implementations for most of the required functions, and then just add
  your own implementation for the following functions: object
  constructor, object destructor, setMediumConstants(), setSat_p(),
  setSat_T(), setState_ph(), setState_pT(), setState_ps(), setState_dT().
  Note that the setState functions need to compute and fill in all
  the fields of the& properties record, otherwise the corresponding
  functions will not work. For example, if your setState functions do not
  set the value of properties, then you won't be able to later
  call thermalConductivity(medium.state) in your Modelica models. On the
  other hand, you don't necessarily need to implement all of four
  setState functions: if you know in advance that your models will only
  use&nbsp;certain combinations of variable as inputs (e.g. p, h), then
  you might omit implementing the setState functions corresponding to the
  other ones.

  Then you must modify the SolverMap::addSolver() function, so that it
  will instantiate your new solver when it is called with the appropriate
  libraryName string. You are free to invent you own syntax for the
  libraryName string, in case you'd like to be able to set up the
  external medium with some additional configuration data from within
  Modelica - it's up to you to decode that syntax within the addSolver()
  function, and within the constructor of your solver object.

  Finally, add the .cpp and .h files to the C/C++ project, recompile to a
  static library (or to a DLL), and then follow the instructions of
  Section 2. of this document.

*/