# quick and dirty makefile for OpenModelica
# Adrian.Pop@liu.se

CFLAGS = -O2 -loleaut32
SOURCES=FluidProp_IF.cpp basesolver.cpp errorhandling.cpp externalmedialib.cpp fluidpropsolver.cpp solvermap.cpp testsolver.cpp

all:
	gcc ${CFLAGS} -c ${SOURCES}
	ar -r ExternalMediaLib.a *.o
	ranlib ExternalMediaLib.a

clean:
	rm -f *.o *.a