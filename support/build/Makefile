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
iCC=/usr/bin/gcc
IDLPP= idlpp

# The C/C++ compiler flags
CPPFLAGS= -g -Wall -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
LDFLAGS= -L$(OSPL_HOME)/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
LIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lpthread


# Boost Libraries is used by the  publisher/generator examples
BOOST_INCFLAGS= 
BOOST_LIBS= -lboost_thread-mt -lboost_program_options-mt -lboost_system-mt

WEBCPPFLAGS= -g -Wall -I/var/www/zaphoyd-websocketpp-e5b87e6/src -I/home/vicareplus/boost_1_48_0 -I$(OSPL_HOME)/include/dcps/C++/SACPP -I$(OSPL_HOME)/include -I$(OSPL_HOME)/include/sys -I$(CPP_LIB_DIR)
WEBLDFLAGS= -L$(OSPL_HOME)/lib -L/home/vicareplus/boost_1_48_0/stage/lib -L/usr/lib -Wl,-rpath,$(OSPL_HOME)/lib
WEBLIBS= -ldcpsgapi -ldcpssacpp -lddsdatabase -lddsos  -lpthread

# Boost Libraries is used by the  publisher/generator examples
WEBBOOST_LIBS= -lboost_thread-mt -lboost_program_options-mt -lboost_system-mt


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
        $(CPP_DIR)/app/temperature/temperature-pub \
        $(CPP_DIR)/app/temperature/temperature-sub-echo \
        $(CPP_DIR)/app/temperature/temperature-sub-alarm \
        $(CPP_DIR)/app/temperature/temperature-sub-persist \
	$(CPP_DIR)/app/ecg/ecg-pub \
	$(CPP_DIR)/app/ecg/ecg-sub-echo \
	$(CPP_DIR)/app/data-generator \
	$(CPP_DIR)/app/webserver


# Build all targets
all: check_idlpp $(TARGETS)


# Clean all files, including C++ generated from IDL
clean:
	rm -f $(CPP_DIR)/lib/gen/bp/*;  \
	rm -f $(CPP_DIR)/lib/*.o; \
	rm -f $(CPP_DIR)/lib/gen/pulseox/*;  \
	rm -f $(CPP_DIR)/lib/gen/temperature/*;  \
	rm -f $(CPP_DIR)/lib/gen/ecg/*;  \
	rm -f $(CPP_DIR)/app/*.o; \
	rm -f $(CPP_DIR)/app/bp/*.o; \
	rm -f $(CPP_DIR)/app/temperature/*.o; \
	rm -f $(CPP_DIR)/app/ecg/*.o; \
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

# Compile the Functions helper class
Functions.o: $(CPP_LIB_DIR)/Functions.cpp
	$(CPP) $(CPPFLAGS) -c $(CPP_LIB_DIR)/Functions.cpp


# Compile the bloodPressurePublish binary
$(CPP_DIR)/app/bp/bp-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-pub $(CPP_DIR)/app/bp/bloodPressure-pub.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS); \
	cp $(CPP_DIR)/app/bp-pub.sh $(TOPDIR)/bin;


# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/bp/bp-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/bp; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-sub-echo $(CPP_DIR)/app/bp/bloodPressure-echo.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/bp/bp-sub-alarm: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/bp; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-sub-alarm $(CPP_DIR)/app/bp/bloodPressure-alarm.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS);

# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/bp/bp-sub-persist: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/bp; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -c $(CPP_DIR)/app/bp/bloodPressure-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -lmongoclient -llog4cpp -lpthread  -o $(TOPDIR)/bin/bp-sub-persist $(CPP_DIR)/app/bp/bloodPressure-persist.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/bp/libbp.a $(LIBS);

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
$(CPP_DIR)/app/pulseox/pulseox-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/pulseox; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-pub $(CPP_DIR)/app/pulseox/pulseOximeter-pub.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS); \
	cp $(CPP_DIR)/app/pulseox-pub.sh $(TOPDIR)/bin;	


# Compile the pulseOximeterSubscribe binary
$(CPP_DIR)/app/pulseox/pulseox-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/pulseox; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-sub-echo $(CPP_DIR)/app/pulseox/pulseOximeter-echo.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS);

# Compile the pulseOximeterSubscribe binary
$(CPP_DIR)/app/pulseox/pulseox-sub-alarm: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-sub-alarm $(CPP_DIR)/app/pulseox/pulseOximeter-alarm.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS);

# Compile the pulseOximeterSubscribe binary
$(CPP_DIR)/app/pulseox/pulseox-sub-persist: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/pulseox; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/pulseox -c $(CPP_DIR)/app/pulseox/pulseOximeter-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -lmongoclient -llog4cpp -lpthread  -o $(TOPDIR)/bin/pulseox-sub-persist $(CPP_DIR)/app/pulseox/pulseOximeter-persist.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(LIBS);

# Generate the temperature C++ source files from the IDL
$(CPP_DIR)/lib/gen/temperature/temperature.h:
	mkdir -p $(CPP_DIR)/lib/gen/temperature; \
	cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/temperature temperature.idl; 

# Compile the temperature IDL-generated C++ files into libtemperature.a
$(CPP_DIR)/lib/gen/temperature/libtemperature.a: $(CPP_DIR)/lib/gen/temperature/temperature.h
	cd $(CPP_DIR)/lib/gen/temperature; \
	$(CPP) $(CPPFLAGS) -c temperature.cpp; \
        $(CPP) $(CPPFLAGS) -c temperatureDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c temperatureDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c temperatureSplDcps.cpp; \
        ar cr libtemperature.a temperature.o temperatureDcps.o temperatureDcps_impl.o temperatureSplDcps.o; \
        ranlib libtemperature.a

# Compile the temperaturePublish binary
$(CPP_DIR)/app/temperature/temperature-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/temperature; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/temperature -c $(CPP_DIR)/app/temperature/temperature-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/temperature-pub $(CPP_DIR)/app/temperature/temperature-pub.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(LIBS); \
	cp $(CPP_DIR)/app/temperature-pub.sh $(TOPDIR)/bin;


# Compile the temperatureSubscribe binary
$(CPP_DIR)/app/temperature/temperature-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(CPP_LIB_DIR)/Functions.o 
	cd $(CPP_DIR)/app/temperature; \
	$(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/temperature -c $(CPP_DIR)/app/temperature/temperature-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/temperature-sub-echo $(CPP_DIR)/app/temperature/temperature-echo.o \
	$(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(LIBS);

# Compile the temperatureSubscribe binary
$(CPP_DIR)/app/temperature/temperature-sub-alarm: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/temperature; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/temperature -c $(CPP_DIR)/app/temperature/temperature-alarm.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/temperature-sub-alarm $(CPP_DIR)/app/temperature/temperature-alarm.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(LIBS);

# Compile the temperatureSubscribe binary
$(CPP_DIR)/app/temperature/temperature-sub-persist: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(CPP_LIB_DIR)/Functions.o 
	cd $(CPP_DIR)/app/temperature; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/temperature -c $(CPP_DIR)/app/temperature/temperature-persist.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -lmongoclient -llog4cpp -lpthread  -o $(TOPDIR)/bin/temperature-sub-persist $(CPP_DIR)/app/temperature/temperature-persist.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(LIBS);

# Generate the bp C++ source files from the IDL
$(CPP_DIR)/lib/gen/ecg/ecg.h:
	mkdir -p $(CPP_DIR)/lib/gen/ecg; \
        cd $(IDL_DIR); $(IDLPP) -S -l cpp -d $(CPP_DIR)/lib/gen/ecg ecg.idl; 

# Compile the ecg IDL-generated C++ files into libecg.a
$(CPP_DIR)/lib/gen/ecg/libecg.a: $(CPP_DIR)/lib/gen/ecg/ecg.h
	cd $(CPP_DIR)/lib/gen/ecg; \
        $(CPP) $(CPPFLAGS) -c ecg.cpp; \
        $(CPP) $(CPPFLAGS) -c ecgDcps.cpp; \
        $(CPP) $(CPPFLAGS) -c ecgDcps_impl.cpp; \
        $(CPP) $(CPPFLAGS) -c ecgSplDcps.cpp; \
        ar cr libecg.a ecg.o ecgDcps.o ecgDcps_impl.o ecgSplDcps.o; \
        ranlib libecg.a

# Compile the bloodPressurePublish binary
$(CPP_DIR)/app/ecg/ecg-pub: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/ecg/libecg.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/ecg; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/ecg -c $(CPP_DIR)/app/ecg/ECG-pub.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/ecg-pub $(CPP_DIR)/app/ecg/ECG-pub.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/ecg/libecg.a $(LIBS); 


# Compile the bloodPressureSubscribe binary
$(CPP_DIR)/app/ecg/ecg-sub-echo: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/ecg/libecg.a $(CPP_LIB_DIR)/Functions.o
	cd $(CPP_DIR)/app/ecg; \
        $(CPP) $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/ecg -c $(CPP_DIR)/app/ecg/ECG-echo.cpp; \
        $(CPP) $(LDFLAGS) $(LIBS) $(BOOST_LIBS)  -llog4cpp -lpthread  -o $(TOPDIR)/bin/ecg-sub-echo $(CPP_DIR)/app/ecg/ECG-echo.o \
        $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_LIB_DIR)/Functions.o $(CPP_DIR)/lib/gen/ecg/libecg.a $(LIBS);


# Build the data-generator binary
$(CPP_DIR)/app/data-generator: $(CPP_DIR)/lib/gen/bp/libbp.a
	cd $(CPP_DIR)/app; \
	$(CPP) $(CPPFLAGS) $(BOOST_INCFLAGS) $(SIMD_INCFLAGS) -I$(CPP_DIR)/lib/gen/bp \
            -c data-generator.cpp; \
	$(CPP) $(CPPFLAGS) -o $(TOPDIR)/bin/data-generator data-generator.o \
            $(CPP_DIR)/lib/gen/bp/libbp.a $(LDFLAGS) \
            $(LIBS) $(BOOST_LIBS);

# Compile the bloodPressureSubscribe web-based binary
$(CPP_DIR)/app/webserver: $(CPP_LIB_DIR)/SimpleDDS.o $(CPP_DIR)/lib/gen/bp/libbp.a $(CPP_LIB_DIR)/Functions.o /var/www/zaphoyd-websocketpp-e5b87e6/libwebsocketpp.a $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(CPP_DIR)/lib/gen/ecg/libecg.a  
	cd $(CPP_DIR)/app; \
	g++ -I/var/www/zaphoyd-websocketpp-e5b87e6/src -I/home/vicareplus/boost_1_48_0  -L/home/vicareplus/boost_1_48_0/stage/lib -c -O2 -o $(CPP_DIR)/app/webServer.o $(CPP_DIR)/app/webServer.cpp $(WEBCPPFLAGS) $(WEBBOOST_LIBS) $(WEBLIBS); \
	g++  -I$(CPP_DIR)/lib/gen/EbD_Sensor $(CPPFLAGS) -I$(CPP_DIR)/lib/gen/bp -I$(CPP_DIR)/lib/gen/pulseox -I$(CPP_DIR)/lib/gen/temperature -I$(CPP_DIR)/lib/gen/ecg -c -O2 -o $(CPP_DIR)/app/web-listener.o $(CPP_DIR)/app/web-listener.cpp $(WEBCPPFLAGS) $(WEBLIBS) ; \
	g++ -O2  $(WEBLDFLAGS) $(CPP_DIR)/app/webServer.o $(CPP_LIB_DIR)/SimpleDDS.o  $(CPP_DIR)/app/web-listener.o -o $(TOPDIR)/bin/webserver  -L/home/vicareplus/boost_1_48_0/stage/lib/ -lboost_system -lboost_date_time -lboost_regex -lboost_random  -lboost_program_options $(CPP_DIR)/lib/gen/bp/libbp.a /var/www/zaphoyd-websocketpp-e5b87e6/libwebsocketpp.a   $(CPP_DIR)/lib/gen/pulseox/libpulseox.a $(CPP_DIR)/lib/gen/temperature/libtemperature.a $(CPP_DIR)/lib/gen/ecg/libecg.a $(WEBCPPFLAGS) $(WEBLIBS);

