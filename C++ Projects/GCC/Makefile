#
# Makefile for ExternalTwoPhaseMedium.dll
#
# Compiler: g++ 3.4.2
# Author: Christoph C. Richter
#		  ch.richter@tu-bs.de
#

all:
	g++ -c -DBUILDING_MEDIA_DLL \
		..\Sources\externaltwophasemedium_lib.cpp \
		..\Sources\basesolver.cpp \
		..\Sources\basetwophasemedium.cpp \
		..\Sources\mediummap.cpp \
		..\Sources\solvermap.cpp \
		..\Sources\testsolver.cpp \
		..\Sources\twophasemedium.cpp

	g++ -shared -o ExternalTwoPhaseMedium.dll \
		externaltwophasemedium_lib.o \
		basesolver.o \
		basetwophasemedium.o \
		mediummap.o \
		solvermap.o \
		testsolver.o \
		twophasemedium.o \
		-Wl,--out-implib,ExternalTwoPhaseMedium.a \
		-Wl,--output-def,ExternalTwoPhaseMedium.def
		
	g++ -o tester main.cpp -L. -lExternalTwoPhaseMedium
			
clean:
	-del *.a *.def *.dll *.exp *.lib *.o 