# ============================================================================
# Name        : Makefile
# Author      : Jorrit Wronski (jowr@ipu.dk) 
# Version     : 1.1
# Copyright   : Use and modify at your own risk.
# Description : Makefile for ExternalMedia and CoolProp
# ============================================================================
# The installation procedure should be as follows:
# 1) make header library
# 2) make install
# ============================================================================
# 
# Changelog:
# ==========
# 2014-06-30 - v1.0: Initial release that includes CoolProp v4.2.5
# 2015-07-15 - v1.1: Updated external CoolProp dependency to v5.1.1 
#
###########################################################
#  Setting the directories for library, header and 
#  binary files created in this makefile, change these 
#  variables to install the library for Dymola or OpenModelica
###########################################################
SRCDIR     :=./Sources
ROOTDIR    := $(abspath $(lastword $(MAKEFILE_LIST)))
# For Dymola integration
# DYMDIR     =/opt/dymola
# LIBINST    =$(DYMDIR)/bin/lib
# HEADINST   =$(DYMDIR)/source
# For OpenModelica integration
# LIBINST    =/usr/lib/omc
# HEADINST   =/usr/include/omc
# For system-wide installation
# LIBINST    =/usr/local/lib
# HEADINST   =/usr/local/include
# For relative path installation
LIBINST     =$(abspath "$(ROOTDIR)/../Modelica/ExternalMedia $(FULLVERSION)/Resources/Library/$(SYSDIR)")
HEADINST    =$(abspath "$(ROOTDIR)/../Modelica/ExternalMedia $(FULLVERSION)/Resources/Include")
#
#BINDIR     :=./build/LinuxGCC

# ============================================================================
# ============ No customisation is necessary beyond this point ===============
# ============================================================================
# general commands:
RM    := rm -f
CP    := cp 
CD    := cd 
CH    := chmod 0644 
MK    := mkdir -p 
LD    := ldconfig 
LN    := ln -sf 
LT    := libtool 
AR    := ar rvs 
SVN   := svn 

# Emulate Gnu make utilities
lc    = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

USERNAME   :=$(shell whoami)

# used for the output
MAJORVERSION     :=3
MINORVERSION     :=2
FULLVERSION      :=$(MAJORVERSION).$(MINORVERSION).1
THENAME          :=ExternalMediaLib
LIBRARYEXTENSION :=.a
THETEST          :=ExternalMediaLibTest

COOLPROPDIR      :=../externals/coolprop.git
COOLPROPTMP      :=../externals/coolprop.build.gcc

ifeq ($(OS),Windows_NT)
  SYSDIR      :=win32
  COOLPROPCMP :=MinGW Makefiles
  BINDIR      :=./build/MinGW
else
  UNAME_S := $(shell uname -s)
  ifeq ($(UNAME_S),Linux)
    SYSDIR      :=linux32
    COOLPROPCMP :=Unix Makefiles
    BINDIR      :=./build/Linux
  else
    SYSDIR      :=mac64
    COOLPROPCMP :=Unix Makefiles
    BINDIR      :=./build/Darwin
  endif
endif

LIBS       =
OPTFLAGS   =-O3 -funroll-loops -ffast-math #-loleaut32 #-DCOOLPROP=1 # optimisation, remove for debugging


###########################################################
#  Change these lines if you are using a different C++ 
#  compiler or if you would like to use other flags. 
###########################################################
CPPC         =g++
DEBUGFLAGS   =-g -O3
CPPFLAGS     =$(OPTFLAGS)# -Wall -pedantic -fbounds-check -ansi -Wpadded -Wpacked -malign-double -mpreferred-stack-boundary=8

.PHONY          : install

LIBFILE         :=$(THENAME)
LIBRARY         :=lib$(THENAME)
#HEADERFILE      =$(THENAME)
HEADERFILE      :=$(call lc,$(THENAME))
HEADEREXTENSION :=.h
###########################################################
#  Change these lines if you have other needs regarding
#  the generated shared library file.  
###########################################################
ifeq ($(LIBRARYEXTENSION),.so)
  install         : install_dynamic
  LIBFLAGS        =-rdynamic -fPIC -shared -Wl,-soname,$(LIBRARY)$(LIBRARYEXTENSION).$(MAJORVERSION)
endif


###########################################################
#  Change these lines if you have other needs regarding
#  the generated static library file.  
###########################################################
ifeq ($(LIBRARYEXTENSION),.a)
  install         : install_static
  LIBFLAGS        =-fPIC -static
endif


###########################################################
#  Copy files to places recognised by the system.
###########################################################
.PHONY     : install_dynamic
install_dynamic : header library
	$(MK) $(HEADINST) $(LIBINST)
	$(CP) $(BINDIR)/$(HEADERFILE)$(HEADEREXTENSION) $(HEADINST)/$(HEADERFILE)$(HEADEREXTENSION)
	$(CP) $(BINDIR)/$(LIBRARY).so $(LIBINST)/$(LIBRARY).so.$(MAJORVERSION).$(MINORVERSION)
	$(CH) $(HEADINST)/$(HEADERFILE)$(HEADEREXTENSION)
	$(CH) $(LIBINST)/$(LIBRARY).so.$(MAJORVERSION).$(MINORVERSION)
ifeq ($(USERNAME),root)
	$(LD) -l $(LIBINST)/$(LIBRARY).so.$(MAJORVERSION).$(MINORVERSION)
endif
	$(LN) $(LIBINST)/$(LIBRARY).so.$(MAJORVERSION) $(LIBINST)/$(LIBRARY).so
ifeq ($(USERNAME),root)
	$(LD) 
endif 

.PHONY     : install_static
install_static : header library
	$(MK) $(HEADINST) $(LIBINST)
	$(CP) $(BINDIR)/$(HEADERFILE)$(HEADEREXTENSION) $(HEADINST)/$(HEADERFILE)$(HEADEREXTENSION)
	$(CP) $(BINDIR)/$(LIBRARY).a  $(LIBINST)/$(LIBRARY).a 
	$(CH) $(HEADINST)/$(HEADERFILE)$(HEADEREXTENSION)
	$(CH) $(LIBINST)/$(LIBRARY).a 
ifeq ($(USERNAME),root)
	$(LD) 
endif 

.PHONY     : uninstall
uninstall  : 
	$(RM) $(HEADINST)/$(HEADERFILE)$(HEADEREXTENSION)
	$(RM) $(LIBINST)/$(LIBRARY)$(LIBRARYEXTENSION)

.PHONY     : all
all        : header library


############################################################
##  Compile sources to static files
############################################################
EXMECPP_FILES  := $(wildcard $(SRCDIR)/*.cpp)
EXMEOBJ_FILES  := $(addprefix $(BINDIR)/,$(notdir $(EXMECPP_FILES:.cpp=.o)))
# Files do not change here, := assignments work. 
USE_FLUIDPROP  := $(shell grep FLUIDPROP $(SRCDIR)/include.h | head -n 1 | cut -d " " -f3)
USE_FLUIDPROP  := 0
USE_COOLPROP   := $(shell grep COOLPROP $(SRCDIR)/include.h | head -n 1 | cut -d " " -f3)


ifeq ($(USE_FLUIDPROP),1)

else
# Remove the FluidProp files, they are not available for Linux
EXMEOBJ_FILES  := $(filter-out $(BINDIR)/FluidProp_IF.o, $(EXMEOBJ_FILES))
EXMEOBJ_FILES  := $(filter-out $(BINDIR)/fluidpropsolver.o, $(EXMEOBJ_FILES))
endif


ifeq ($(USE_COOLPROP),1)

else
COOLCPP_FILES  :=  
COOLOBJ_FILES  :=  
EXMEOBJ_FILES  := $(filter-out $(BINDIR)/coolpropsolver.o, $(EXMEOBJ_FILES))
endif

# Define search path for prerequisite files and g++ include 
# parameters to be used later.
INC_LIST:=# 0
INC_LIST+=$(COOLPROPDIR)/include #                     1
INC_LIST+=$(SRCDIR) #                                  2
INC_LIST+=$(COOLPROPDIR)/externals/msgpack-c/include # 3

CPPINCLUDES:=$(foreach i,$(INC_LIST), -I$(i))
#VPAINCLUDES:=$(foreach i,$(INC_LIST),$(i):)
#
#vpath %.cpp $(VPAINCLUDES)


##########################################################
#  Compile the C++ sources into a library file that can 
#  be used as shared or static object.
###########################################################
.PHONY     : header
header     : $(BINDIR)/$(HEADERFILE)$(HEADEREXTENSION)
$(BINDIR)/$(HEADERFILE)$(HEADEREXTENSION): $(SRCDIR)/$(HEADERFILE)$(HEADEREXTENSION)
	$(MK) $(BINDIR)
	$(CP) $(SRCDIR)/$(HEADERFILE)$(HEADEREXTENSION) $(BINDIR)

.PHONY     : library
library    : $(BINDIR)/$(LIBRARY)$(LIBRARYEXTENSION)

$(BINDIR)/$(LIBRARY).so: coolprop $(EXMEOBJ_FILES) #$(SRCDIR)/$(LIBFILE).o
	$(MK) $(BINDIR)
	$(CPPC) $(LIBFLAGS) $(CPPFLAGS) -o $(BINDIR)/$(LIBRARY).so $(EXMEOBJ_FILES) #$(SRCDIR)/$(LIBFILE).o

$(BINDIR)/$(LIBRARY).a: coolprop $(EXMEOBJ_FILES) #$(SRCDIR)/$(LIBFILE).o
	$(MK) $(BINDIR)
	$(AR) $(BINDIR)/$(LIBRARY).a $(BINDIR)/*.o $^

.PHONY: coolprop
ifeq ($(USE_COOLPROP),1)
coolprop:
	if [ ! -d "$(COOLPROPDIR)" ]; then git clone --recursive https://github.com/CoolProp/CoolProp.git "$(COOLPROPDIR)" ; fi
	pushd "$(COOLPROPDIR)" ; git pull origin master ; popd 
	pushd "$(COOLPROPDIR)" ; git submodule init ; popd 
	pushd "$(COOLPROPDIR)" ; git submodule update ; popd 
	$(MK) "$(COOLPROPTMP)"
	pushd "$(COOLPROPTMP)" ; \
    cmake "../$(COOLPROPDIR)" -DCOOLPROP_OBJECT_LIBRARY=ON -DCOOLPROP_RELEASE=ON -G "$(COOLPROPCMP)" ; \
    cmake --build . --config Release ; \
    popd 
	$(CP) "$(COOLPROPTMP)/CoolProp.dir/Release/"*.o $(BINDIR)/
else
coolprop:
	@echo "Not using CoolProp"
endif


###########################################################
#  General rulesets for compilation.
###########################################################
.PHONY: clean
clean:
	$(RM) $(BINDIR)/*solver.o  $(BINDIR)/*solver.obj $(BINDIR)/*xternal*

.PHONY: very-clean
very-clean: clean
	$(RM) $(BINDIR)/* **.so **.a **.dylib **.lib **.o **.obj **.mod
	$(RM) -rf "$(COOLPROPTMP)"

$(BINDIR)/%.o : %.cpp
	$(CPPC) $(CPPFLAGS) -o $(BINDIR)/$*.o $(CPPINCLUDES) -c $<


############################################################
##  Create the documentation from annotations in the source
##  files with DOXYGEN, a configuration file is needed.
############################################################
#.PHONY     : doc
#doc        : doc/Doxyfile
#	doxygen doc/Doxyfile
#	cd doc/latex ; \
#	make all 
	
.PHONY : print-flags
print-flags:
	@printf "\nBuild settings:\n\
	  USERNAME     :$(USERNAME)\n\
	  MAJORVERSION :$(MAJORVERSION)\n\
	  MINORVERSION :$(MINORVERSION)\n\
	  LIBRARY      :$(LIBRARY)\n\
	  LIBFLAGS     :$(LIBFLAGS)\n\
	  OPTFLAGS     :$(OPTFLAGS)\n\
	  CPPFLAGS     :$(CPPFLAGS)\n\
	  CPPINCLUDES  :$(CPPINCLUDES)\n\
	  USE_FLUIDPROP:$(USE_FLUIDPROP)\n\
	  USE_COOLPROP :$(USE_COOLPROP)\n\n\
	  EXMEOBJ_FILES:\n$(EXMEOBJ_FILES)\n\n"
