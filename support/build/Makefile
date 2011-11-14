# Makefile 

# The top level directory
TOPDIR=$(PWD)/../..

# The source directory locations
IDL_DIR=$(TOPDIR)/src/idl
CPP_DIR=$(TOPDIR)/src/c++/production
CPP_TEST=$(TOPDIR)/src/c++/test
JAVA_DIR=$(TOPDIR)/src/java/production
CPP_LIB_DIR=$(TOPDIR)/src/c++/production/lib

# The C/C++ compilers to use
CPP=/usr/bin/g++ 
CC=/usr/bin/gcc
IDLPP=idlpp

# The C/C++ compiler flags
CPPFLAGS= -g -Wall -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
LDFLAGS= -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
LIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lpthread

# Boost Libraries is used by the  publisher/generator examples
BOOST_INCFLAGS= 
BOOST_LIBS= -lboost_thread-mt -lboost_program_options-mt -lboost_system-mt

# The target programs to build
TARGETS= \
	$(CPP_DIR)/app/bp/bp-pub \
	$(CPP_DIR)/app/bp/bp-sub-echo \
	$(CPP_DIR)/app/bp/bp-sub-alarm \
	$(CPP_DIR)/app/bp/bp-sub-persist \
	$(CPP_DIR)/app/pulseox/pulseox-pub \
        $(CPP_DIR)/app/pulseox/pulseox-sub-echo \
        $(CPP_DIR)/app/pulseox/pulseox-sub-alarm \
        $(CPP_DIR)/app/pulseox/pulseox-sub-persist \
	$(CPP_DIR)/app/temp-monitor/tempmonitor-pub \
        $(CPP_DIR)/app/temp-monitor/tempmonitor-sub-echo \
        $(CPP_DIR)/app/temp-monitor/tempmonitor-sub-alarm \
        $(CPP_DIR)/app/temp-monitor/tempmonitor-sub-persist \
	$(CPP_DIR)/app/command-controller \
	$(CPP_DIR)/app/data-generator 


# Build all targets
all: check_idlpp $(TARGETS)


# Clean all files, including C++ generated from IDL
clean:
	rm -f $(CPP_DIR)/lib/gen/bp/*;  \
	rm -f $(CPP_DIR)/lib/*.o; \
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


# Generate the bp C++ source files from the IDL
$(CPP_DIR)/lib/gen/bp/bp.h:
	mkdir -p $(CPP_DIR)/lib/gen/bp; \
	cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/bp bp.idl; 

# Compile the bp IDL-generated C++ files into libbp.a
$(CPP_DIR)/lib/gen/bp/libbp.a: $(CPP_DIR)/lib/gen/bp/bp.h
	cd $(CPP_DIR)/lib/gen/bp; \
	$(CPP) $(CPPFLAGS) -c bp.cpp; \
        $(CPP) $(CPPFLAGS) -c bpDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c bpDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c bpSplDcps.cpp; \
        ar cr libbp.a bp.o bpDcps.o bpDcps_impl.o bpSplDcps.o; \
        ranlib libbp.a

# Compile the SimpleDDS helper class
SimpleDDS.o: $(CPP_LIB_DIR)/SimpleDDS.cpp
	$(CPP) $(CPPFLAGS) -c $(CPP_LIB_DIR)/SimpleDDS.cpp


# Compile the bloodPressurePublish binary
$(CPP_DIR)/app/bp/bp-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-pub $(CPP_DIR)/app/bp/bloodPressure-pub.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS); \
	cp $(CPP_DIR)/app/bp-pub.sh $(TOPDIR)/bin;


# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/bp/bp-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-sub-echo $(CPP_DIR)/app/bp/bloodPressure-echo.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/bp/bp-sub-alarm: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-sub-alarm $(CPP_DIR)/app/bp/bloodPressure-alarm.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/bp/bp-sub-persist: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app/bp; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -lmongoclient -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-sub-persist $(CPP_DIR)/app/bp/bloodPressure-persist.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS);

# Generate the pulseox C++ source files from the IDL
$(CPP_DIR)/lib/gen/pulseox/pulseox.h:
	mkdir -p $(CPP_DIR)/lib/gen/pulseox; \
	cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/pulseox pulseox.idl; 

# Compile the pulseox IDL-generated C++ files into libpulseox.a
$(CPP_DIR)/lib/gen/pulseox/libpulseox.a: $(CPP_DIR)/lib/gen/pulseox/pulseox.h
	cd $(CPP_DIR)/lib/gen/pulseox; \
	$(CPP) $(CPPFLAGS) -c pulseox.cpp; \
        $(CPP) $(CPPFLAGS) -c pulseoxDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c pulseoxDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c pulseoxSplDcps.cpp; \
        ar cr libpulseox.a pulseox.o pulseoxDcps.o pulseoxDcps_impl.o pulseoxSplDcps.o; \
        ranlib libpulseox.a

# Compile the pulseOximeterPublish binary
$(CPP_DIR)/app/pulseox/pulseox-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-pub $(CPP_DIR)/app/pulseox/pulseOximeter-pub.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS); \
	cp $(CPP_DIR)/app/pulseox-pub.sh $(TOPDIR)/bin;	


# Compile the pulseOximeterSubscribe binary
$(CPP_DIR)/app/pulseox/pulseox-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-sub-echo $(CPP_DIR)/app/pulseox/pulseOximeter-echo.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS);

# Compile the pulseOximeterSubscribe binary
$(CPP_DIR)/app/pulseox/pulseox-sub-alarm: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-sub-alarm $(CPP_DIR)/app/pulseox/pulseOximeter-alarm.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS);

# Compile the pulseOximeterSubscribe binary
$(CPP_DIR)/app/pulseox/pulseox-sub-persist: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -lmongoclient -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-sub-persist $(CPP_DIR)/app/pulseox/pulseOximeter-persist.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS);

# Generate the tempmonitor C++ source files from the IDL
$(CPP_DIR)/lib/gen/tempmonitor/tempmonitor.h:
	mkdir -p $(CPP_DIR)/lib/gen/tempmonitor; \
	cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/tempmonitor tempmonitor.idl; 

# Compile the tempmonitor IDL-generated C++ files into libtempmonitor.a
$(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a: $(CPP_DIR)/lib/gen/tempmonitor/tempmonitor.h
	cd $(CPP_DIR)/lib/gen/tempmonitor; \
	$(CPP) $(CPPFLAGS) -c tempmonitor.cpp; \
        $(CPP) $(CPPFLAGS) -c tempmonitorDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c tempmonitorDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c tempmonitorSplDcps.cpp; \
        ar cr libtempmonitor.a tempmonitor.o tempmonitorDcps.o tempmonitorDcps_impl.o tempmonitorSplDcps.o; \
        ranlib libtempmonitor.a

# Compile the tempMonitorPublish binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor -c $(CPP_DIR)/app/temp-monitor/tempMonitor-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/tempmonitor-pub $(CPP_DIR)/app/temp-monitor/tempMonitor-pub.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LIBS); \
	cp $(CPP_DIR)/app/tempmonitor-pub.sh $(TOPDIR)/bin;


# Compile the tempMonitorSubscribe binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor -c $(CPP_DIR)/app/temp-monitor/tempMonitor-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/tempmonitor-sub-echo $(CPP_DIR)/app/temp-monitor/tempMonitor-echo.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LIBS);

# Compile the tempMonitorSubscribe binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-sub-alarm: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor -c $(CPP_DIR)/app/temp-monitor/tempMonitor-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/tempmonitor-sub-alarm $(CPP_DIR)/app/temp-monitor/tempMonitor-alarm.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LIBS);

# Compile the tempMonitorSubscribe binary
$(CPP_DIR)/app/temp-monitor/tempmonitor-sub-persist: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a
	cd $(CPP_DIR)/app/temp-monitor; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/tempmonitor -c $(CPP_DIR)/app/temp-monitor/tempMonitor-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -lmongoclient -llog4cpp -lpthread  -o $(TOPDIR)/bin/tempmonitor-sub-persist $(CPP_DIR)/app/temp-monitor/tempMonitor-persist.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/tempmonitor/libtempmonitor.a $(LIBS);

# Build the command-controller binary
$(CPP_DIR)/app/command-controller: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c command-controller.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/command-controller command-controller.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS);


# Build the data-generator binary
$(CPP_DIR)/app/data-generator: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c data-generator.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/data-generator data-generator.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS);

