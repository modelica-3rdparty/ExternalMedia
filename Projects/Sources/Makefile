# quick and dirty makefile for OpenModelica
# Adrian.Pop@liu.se

CFLAGS = -O2 -loleaut32
SOURCES=FluidProp_IF.cpp basesolver.cpp errorhandling.cpp externalmedialib.cpp fluidpropsolver.cpp solvermap.cpp testsolver.cpp mingw_gcc_comutil.cpp

all:
	gcc ${CFLAGS} -c ${SOURCES}
	ar -r libExternalMediaLib.a *.o
	ranlib libExternalMediaLib.a

clean:
	rm -f *.o *.a