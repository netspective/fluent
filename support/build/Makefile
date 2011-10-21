# Makefile 

# The top level directory
TOPDIR=$(PWD)/../..

# The C/C++ compilers to use
CPP=/usr/bin/g++ 
CC=/usr/bin/gcc
IDLPP=idlpp

# The C/C++ compiler flags
CPPFLAGS= -g -Wall -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys
LDFLAGS= -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
LIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lpthread

# SIMD is used by the  publisher/generator examples
BOOST_INCFLAGS= 
BOOST_LIBS= -lboost_thread-mt -lboost_program_options-mt -lboost_system-mt
SIMD_INCFLAGS= -I$(SIMD_INSTALL_DIR)/include
SIMD_LIBS= -L$(SIMD_INSTALL_DIR)/lib -lSimD


# The source directory locations
IDL_DIR=$(TOPDIR)/src/idl
CPP_DIR=$(TOPDIR)/src/c++/production
CPP_TEST=$(TOPDIR)/src/c++/test
JAVA_DIR=$(TOPDIR)/src/java/production


# The target programs to build
TARGETS= \
  $(CPP_DIR)/app/bp/bp-pub \
  $(CPP_DIR)/app/bp/bp-sub \
  $(CPP_DIR)/app/command-controller \
  $(CPP_DIR)/app/data-generator


# Build all targets
all: check_idlpp check_simd $(TARGETS)


# Clean all files, including C++ generated from IDL
clean:
	rm -f $(CPP_DIR)/lib/gen/bp/*;  \
	rm -f $(CPP_DIR)/app/*.o; \
	rm -f $(CPP_DIR)/app/bp/*.o; \
	rm -f $(CPP_DIR)/test/*.o; \
	rm -f $(TOPDIR)/bin/*
	rm -f $(TARGETS)

# Check that the idlpp compiler is in the current path
check_idlpp:
	@$(IDLPP) | grep Usage > /dev/null;

# Check that the SIMD_INSTALL_DIR is defined
check_simd:
	if [ ! -f $(SIMD_INSTALL_DIR)/include/dds/dds.hpp ]; then \
	    echo "You must set SIMD_INSTALL_DIR to the path containing include/dds/dds.hpp."; \
	    echo "For example, export SIMD_INSTALL_DIR=/usr/local/simd"; \
	    exit 1; \
	fi;

# Generate the bp C++ source files from the IDL
$(CPP_DIR)/lib/gen/bp/bp.h:
	mkdir -p $(CPP_DIR)/lib/gen/bp; \
	cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/bp bp.idl;

# Compile the bp IDL-generated C++ files
$(CPP_DIR)/lib/gen/bp/bp.o: $(CPP_DIR)/lib/gen/bp/bp.h
	cd $(CPP_DIR)/lib/gen/bp; \
	$(CPP) $(CPPFLAGS) -c bp.cpp; \
	$(CPP) $(CPPFLAGS) -c bpDcps.cpp; \
	$(CPP) $(CPPFLAGS) -c bpDcps_impl.cpp; \
	$(CPP) $(CPPFLAGS) -c bpSplDcps.cpp; \

# Generate a bp archive library
$(CPP_DIR)/lib/gen/bp/libbp.a: $(CPP_DIR)/lib/gen/bp/bp.o
	cd $(CPP_DIR)/lib/gen/bp; \
	ar cr libbp.a bp.o bpDcps.o bpDcps_impl.o bpSplDcps.o; \
	ranlib libbp.a

# Build the bp-pub binary
$(CPP_DIR)/app/bp/bp-pub: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c bloodPressure-pub.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/bp-pub bloodPressure-pub.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);


# Build the bp-sub binary
$(CPP_DIR)/app/bp/bp-sub: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c bloodPressure-echo.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/bp-sub-echo bloodPressure-echo.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);


# Build the command-controller binary
$(CPP_DIR)/app/command-controller: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c command-controller.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/command-controller command-controller.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);

# Build the data-generator binary
$(CPP_DIR)/app/data-generator: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c data-generator.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/data-generator data-generator.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);






