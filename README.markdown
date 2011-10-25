############################################################################################
#                                                                                          #
#			Medical Device Gateway Implementation                              #
#                                                                                          #
############################################################################################

OVERVIEW
--------
Medical Device Gateway consists of interfaces to retrieve the medical data from the connected 
medical devices in the hospital enterprise. It formats the data from the medical device into 
standard patient information and transmits it into the hospital EHR and PHR. 
It also provides the required information to the medical device data center for archival.


EQUIREMENTS
------------
The requirements for compiling and running :

- OpenSplice DDS v5.x  
- SimD-CXX tool
- BOOST v1.40 (or higher)
- BOOST Process (Already shipped with SimD)
- GCC/G++ v4.1 (or higher)
- CMake v2.6 (or higher)

INSTALLATION STEPS
------------------

OpenSpliceDDS
-------------
OpenSplice DDS is one of several open source implementation of the OMG Data Distribution Service for Real-Time Systems (DDS) standard.

Download the OpenSplice From: 
http://www.prismtech.com/opensplice/opensplice-dds-community/software-downloads

download the OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz from the above link.

$ tar xf OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz

Just Extract the downloaded tar file with above command, After extracted the tar file you can find the HDE folder.

$ source release.com

Use the above command to source the path to run the OpenSplice.

$ ospl start

BOOST LIBRARY
------------

Boost libraries are intended to be widely useful, and usable across a broad spectrum of applications. 

$ apt-get install libboost-date-time-dev  libboost-dev libboost-doc libboost-filesystem-dev libboost-graph-dev 
libboost-iostreams-dev libboost-program-options-dev libboost-python-dev libboost-regex-dev 
libboost-signals-dev libboost-test-dev libboost-thread-dev



Simd-CXX
--------
SIMD is used to simplify the use of DDS. We can create dds application using API
(template and meta-programming )  from SimD.

It reduces the big lines of code. 

Downloaded the SimD from http://code.google.com/p/simd-cxx/ - latest version.

The following packages are required to complie the SimD:

$ tar xf simd-cxx<version>.tar.gz

$ cd simd-cxx<version<.tar.gz

$ mkdir build

$ cd build

$ cmake -DCMAKE_INSTALL_PREFIX=$SIMD_INSTALL_DIR ..

$ make install

CMAKE 
-------
Cmake is used to software compilation using simple platform and makes native makefiles and workspaces 
that can be used in the compiler evnvironment.

$ apt-get install cmake

Development Libraries
---------------------
GCC is an integrated distribution of compilers for several major programming languages.

$ apt-get install build-essential


COMPILATION STEPS
-----------------

This package contains the following files:

support/build/Makefile
src/c++/production/app/data-generator.cpp

src/c++/production/app/command-controller.cpp

src/c++/production/app/bp/bloodPressure-pub.cpp

src/c++/production/app/bp/bloodPressure-echo.cpp

src/c++/production/app/bp/bloodPressure-persist.cpp

src/idl/bp.idl

To build, do the following

Include the OpenSliceDDS environment variables.
The path depends on where you installed it.

$ . /usr/local/HDE/x86.linux2.6/release.com

Export the directory where you installed SIMD

$ export SIMD_INSTALL_DIR=/usr/local/simd

This directory should include the header and library files
/usr/local/simd/include/dds/dds.hpp
/usr/local/simd/lib/libSimD.so

Run the makefile

$ cd support/build

$ make

This will create four programs:

src/c++/production/app/bp/bp-pub

src/c++/production/app/bp/bp-sub

src/c++/production/app/data-generator

src/c++/production/app/command-controller

To clean all the generated files run

$ make clean



