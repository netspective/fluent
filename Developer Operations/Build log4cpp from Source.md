# LOG For C++

Log4cpp is library of C++ classes for logging to files, syslog and other destinations. 

Download [log4c++](http://sourceforge.net/projects/log4cpp/files/).

Follow the steps given below to install Log4cpp

	tar -xvf log4cpp-1.0.tar.gz
    cd log4cpp-1.0/
    ./configure
    make
    make check
    make install

Export the log4cpp path to the environment variable using following command         

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

## Troubleshooting

Constructor error in the Basiclayout.cpp

	add header file  include<memory> in src/BasicLayout.cpp.

Parsing error in the  Patternlayout.cpp

	Change the line 373 as follows

	Before
		"component = new FormatModifierComponent(component, std::abs(minWidth), maxWidth, minWidth < 0);"
	After
		"component = new FormatModifierComponent(component, std::abs((float)minWidth), maxWidth, minWidth < 0);"

