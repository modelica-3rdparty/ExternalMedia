# External Media History

The ExternalMedia project was started in 2006 by Francesco Casella and 
Christoph Richter, with the aim of providing a framework for interfacing
external codes computing fluid properties to Modelica.Media-compatible
component models. The two main requirements are: maximizing the efficiency
of the code and minimizing the amount of extra code required to use your
own external code within the framework.

The first implementation featured a hidden cache in the C++ layer and used
integer unique IDs to reference that cache. This architecture worked well
if the models did not contain implicit algebraic equations involving medium
properties, but had serious issues when such equations were involved, which
is often the case when solving steady-state initialization problems.

The library has been restructured in 2012 by Francesco Casella and Roberto
Bonifetto. The main idea has been to get rid of the hidden cache and of the
unique ID references and use the Modelica state records for caching. In this
way, all optimizations performed by Modelica tools are guaranteed to give
correct results, which was previously not the case. The current
version implements the Modelica.Media.Interfaces.PartialTwoPhaseMedium
interface, so it can handle pure fluids, either one-phase or two-phase,
and is compatible with Modelica and Modelica Standard Library 3.2. Please note
that the paths of the medium packages have been changed from the previous
versions, so you might need some small changes if you want to upgrade your
models from previous versions of the ExternalMedia library.
