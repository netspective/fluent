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
  $(CPP_DIR)/app/bp/bp-sub-echo \
  $(CPP_DIR)/app/bp/bp-sub-persist \
  $(CPP_DIR)/app/bp/bp-sub-alarm \
  $(CPP_DIR)/app/command-controller \
  $(CPP_DIR)/app/data-generator \
  $(CPP_DIR)/app/pulseox/pulseox-pub \
  $(CPP_DIR)/app/pulseox/pulseox-sub-echo \
  $(CPP_DIR)/app/pulseox/pulseox-sub-alarm \
  $(CPP_DIR)/app/pulseox/pulseox-sub-persist \
  $(CPP_DIR)/app/temp-monitor/tempmonitor-pub \
  $(CPP_DIR)/app/temp-monitor/tempmonitor-sub-echo \
  $(CPP_DIR)/app/temp-monitor/tempmonitor-sub-alarm \
  $(CPP_DIR)/app/temp-monitor/tempmonitor-sub-persist 


# Build all targets
all: check_idlpp check_simd $(TARGETS)


# Clean all files, including C++ generated from IDL
clean:
	rm -f $(CPP_DIR)/lib/gen/bp/*;  \
	rm -f $(CPP_DIR)/lib/gen/pulseox/*;  \
	rm -f $(CPP_DIR)/lib/gen/tempmonitor/*;  \
	rm -f $(CPP_DIR)/app/*.o; \
	rm -f $(CPP_DIR)/app/bp/*.o; \
	rm -f $(CPP_DIR)/app/temp-monitor/*.o; \
	rm -f $(CPP_DIR)/app/pulseox/*.o; \
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
	$(CPP) $(CPPFLAGS) -c bpSplDcps.cpp; 


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
            $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread $(SIMD_LIBS); \
	    cp $(CPP_DIR)/app/bp-pub.sh $(TOPDIR)/bin;


# Build the bp-sub-echo binary
$(CPP_DIR)/app/bp/bp-sub-echo: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c bloodPressure-echo.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/bp-sub-echo bloodPressure-echo.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);


# Build the bp-sub-persist binary
$(CPP_DIR)/app/bp/bp-sub-persist: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c bloodPressure-persist.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/bp-sub-persist bloodPressure-persist.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  -lmongoclient $(SIMD_LIBS);


# Build the bp-sub-alarm binary
$(CPP_DIR)/app/bp/bp-sub-alarm: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c bloodPressure-alarm.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/bp-sub-alarm bloodPressure-alarm.o \
	    $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
	    $(LIBS) $(BOOST_LIBS)  -lmongoclient $(SIMD_LIBS);


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

# Generate the pulseox c++ source files from the IDL
$(CPP_DIR)/lib/gen/pulseox/pulseox.h:
	mkdir -p $(CPP_DIR)/lib/gen/pulseox; \
        cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/pulseox pulseox.idl;


# Compile the pulseox IDL-generated C++ files
$(CPP_DIR)/lib/gen/pulseox/pulseox.o: $(CPP_DIR)/lib/gen/pulseox/pulseox.h
	cd $(CPP_DIR)/lib/gen/pulseox; \
        $(CPP) $(CPPFLAGS) -c pulseox.cpp; \
        $(CPP) $(CPPFLAGS) -c pulseoxDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c pulseoxDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c pulseoxSplDcps.cpp; \

# Generate a pulseox archive library
$(CPP_DIR)/lib/gen/pulseox/libpulseox.a: $(CPP_DIR)/lib/gen/pulseox/pulseox.o
	cd $(CPP_DIR)/lib/gen/pulseox; \
        ar cr libpulseox.a pulseox.o pulseoxDcps.o pulseoxDcps_impl.o pulseoxSplDcps.o; \
        ranlib libpulseox.a


# Build the pulseox-pub binary
$(CPP_DIR)/app/pulseox/pulseox-pub: $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/pulseox \
            -c pulseOximeter-pub.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/pulseox-pub pulseOximeter-pub.o \
            $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread $(SIMD_LIBS); \
	    cp $(CPP_DIR)/app/pulseox-pub.sh $(TOPDIR)/bin;


# Build the pulseox-sub-echo binary
$(CPP_DIR)/app/pulseox/pulseox-sub-echo: $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/pulseox \
            -c pulseOximeter-echo.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/pulseox-sub-echo pulseOximeter-echo.o \
            $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);

# Build the pulseox-sub-alarm binary
$(CPP_DIR)/app/pulseox/pulseox-sub-alarm: $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/pulseox \
            -c pulseOximeter-alarm.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/pulseox-sub-alarm pulseOximeter-alarm.o \
            $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);

# Build the pulseox-sub-persist binary
$(CPP_DIR)/app/pulseox/pulseox-sub-persist: $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/pulseox \
            -c pulseOximeter-persist.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/pulseox-sub-persist pulseOximeter-persist.o \
            $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  -lmongoclient $(SIMD_LIBS);


# Generate the tempmonitor c++ source files from the IDL
$(CPP_DIR)/lib/gen/tempmonitor/tempmonitor.h:
	mkdir -p $(CPP_DIR)/lib/gen/tempmonitor; \
        cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/tempmonitor tempmonitor.idl;


# Compile the tempmonitor IDL-generated C++ files
$(CPP_DIR)/lib/gen/tempmonitor/tempmonitor.o: $(CPP_DIR)/lib/gen/tempmonitor/tempmonitor.h
	cd $(CPP_DIR)/lib/gen/tempmonitor; \
        $(CPP) $(CPPFLAGS) -c tempmonitor.cpp; \
        $(CPP) $(CPPFLAGS) -c tempmonitorDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c tempmonitorDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c tempmonitorSplDcps.cpp; \

# Generate a tempmonitor archive library
$(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a: $(CPP_DIR)/lib/gen/tempmonitor/tempmonitor.o
	cd $(CPP_DIR)/lib/gen/tempmonitor; \
        ar cr libtempmonitor.a tempmonitor.o tempmonitorDcps.o tempmonitorDcps_impl.o tempmonitorSplDcps.o; \
        ranlib libtempmonitor.a


# Build the tempmonitor-pub binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-pub: $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor \
            -c tempMonitor-pub.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/tempmonitor-pub tempMonitor-pub.o \
            $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS) -llog4cpp -lpthread  $(SIMD_LIBS); \
	    cp $(CPP_DIR)/app/tempmonitor-pub.sh $(TOPDIR)/bin;


# Build the tempmonitor-sub-echo binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-sub-echo: $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor \
            -c tempMonitor-echo.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/tempmonitor-sub-echo tempMonitor-echo.o \
            $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);

# Build the tempmonitor-sub-alarm binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-sub-alarm: $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor \
            -c tempMonitor-alarm.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/tempmonitor-sub-alarm tempMonitor-alarm.o \
            $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  $(SIMD_LIBS);

# Build the tempmonitor-sub-persist binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-sub-persist: $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
        $(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor \
            -c tempMonitor-persist.cpp; \
        $(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/tempmonitor-sub-persist tempMonitor-persist.o \
            $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS)  -lmongoclient $(SIMD_LIBS);



