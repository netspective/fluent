############################################################################################
#                                                                                          #
#                 Medical Device Gateway Implementation                                    #
#                                                                                          #
############################################################################################

OVERVIEW
========
  Medical Device Gateway consists of interfaces to retrieve the medical data from the connected 
medical devices in the hospital enterprise. It formats the data from the medical device into 
standard patient information and transmits it into the hospital EHR and PHR. 
It also provides the required information to the medical device data center for archival.


REQUIREMENTS
------------
The requirements for compiling and running :

* OpenSplice DDS v5.x  
* BOOST v1.40 (or higher)
* BOOST Process (Already shipped with SimD)
* GCC/G++ v4.1 (or higher)
* Log for c++ 1.0 (or higher)

INSTALLATION STEPS
------------------

OPENSPLICEDDS
-------------
  OpenSplice DDS is one of several open source implementation of the OMG Data Distribution Service for Real-Time Systems (DDS) standard.
  
  Download the OpenSplice  version of `OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz` from the following link.
  
    http://www.prismtech.com/opensplice/opensplice-dds-community/software-downloads
    
  Extract the downloaded tar file with following command, after extracted the tar file you could find the HDE folder.

      $ tar xf OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz
      
      $ source release.com
      
      $ ospl start

Use the above commands to source the path and to start  OpenSplice.

      
BOOST LIBRARY
------------

Boost libraries are intended to be widely useful, and usable across a broad spectrum of applications. 

    $ yum install libboost-date-time-dev  libboost-dev libboost-doc libboost-filesystem-dev libboost-graph-dev 
    
      libboost-iostreams-dev libboost-program-options-dev libboost-python-dev libboost-regex-dev 
      
      libboost-signals-dev libboost-test-dev libboost-thread-dev

DEVELOPMENT LIBRARIES
---------------------
GCC is an integrated distribution of compilers for several major programming languages.

    $ yum install build-essential

LOG FOR C++
-----------
Log4cpp is library of C++ classes for logging to files, syslog and other destinations.[Click here to download]  (http://sourceforge.net/projects/log4cpp/files/) for log4cpp libraries.

        $ ./configure

        $ make

        $ make check

        $ make install


COMPILATION STEPS
-----------------

This package contains the following file: ../support/build/Makefile

* To build, do the following steps 
 
    1.Include the OpenSliceDDS environment variables from the Opensplice installed directory as ahown below.

        $ source /../../HDE/x86.linux2.6/release.com

* Run the makefile

        $ cd support/build

        $ make

* This will be created in following directory and to clean all the generated files run the command .
                    
        $ ../../bin/
        
        $ make clean


STEPS TO RUN THE APPLICATIONS 
=============================

**1**. Set OpenSpliceDDS environment using the following command

        $ source /usr/local/HDE/x86.linux2.6/release.com

**2**. Start OpenSpliceDDS

        $ ospl start

**3**. Start the data generator at the terminal using the following command

        $ ./data-generator 
     
  * Data Generator will wait until it receives a request from publisher and once the request is received then it generates the data 

randomly corresponding to the publisher and send it to the publisher. The request can be from any of the three publishers listed below,
     
**BLOOD PRESSURE:**

**3.1.** Start the `blood pressure publisher` shall be started by passing the various options suffix to the command .

      $. /bp-pub --

Available options  are:

        --help                   Produce help message

        --data-gen-ip arg        Data Generator IP 
        
        --data-gen-port          Data Generator Port

        --domain arg             Device Domain 

        --device-id arg          Device ID - for device identification

        --log-info arg           Log information category

        --log-data arg           Log data category 

        --log4cpp-conf arg       Log configuration and format specification file
    
Example:

    
    ./bp-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.data --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf


  * Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format`


**3.1.1** Multiple publishers need to publish the data on the topic use the following command with the options

        $./bp-pub.sh --

Available Options are:

        --domain        Device Domain
        
        --device-id     Device ID to identify the particular device
        
        --spawn         Number of publishers to run
        
        --log4cpp-conf  Configration file path

EXAMPLE:
        $./bp-pub.sh --domain=blood --device-id=BP --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf

NOTE : `The  arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format`.

**3.2**. Start the `blood pressure subscribers` on the other terminal by passing the various options suffix to the command .

    $ ./bp-sub-echo --

Available options are:

        --help                  Produce help message

        --domain arg            Device Domain 

        --device-id arg         Device ID - for device identification

        --log-info arg          Log information category

        --lod-data arg   	      Log data category

        --log4cpp-conf arg      Log configuration and format specification file

Example:

         $ /bp-sub-echo --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.echo --log4cpp-conf ../src/c++/production/conf/

            simulation_log_bp_sub.conf         

      
  * Once the blood pressure subscriber is started it will retrieve data from the Topic. Subscriber uses ContentFilterTopic to retrieve messages based on the Device ID from a single topic.
  
  NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**3.3**. Start the `blood pressure alarm` on the other terminal by passing the various options suffix to the command. 

        $ ./bp-sub-alarm 

Available options are:
  
        --help                    Produce help message

        --domain arg              Device Domain

        --device-id arg           Device ID for identification

        --log-info arg            Log information category

        --log-data arg            Log data category 

        --log4cpp-conf arg        Log configuration and format specification file

        --systolic-low arg        Systolic low pressure alarm specification - default <90

        --systolic-high arg       Systolic high Pressure alarm specification - default >140

        --diatolic-low arg        Diatolic low pressure alarm specification - default <60

        --diatolic-high arg       Diatolic high pressure alarm specification - default>90

        --pulse-rate-low arg      Pulse low rate alarm specification - default <60

        --pulse-rate-high arg     Pulse high rate alarm specification - default >90

Example :

    $./bp-sub-alarm --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.alarm --log4cpp-conf ../src/c++/production/conf/

      simulation_log_bp_sub.conf


  * Once the blood pressure alarm is started it will retrieve the data and the displays in log file based on the default assessment or from the specified 

arguments.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**3.4**. Start the `blood pressure persists` in the other terminal by passing the various options suffix to the command. 

      $./bp-sub-persist 

Available options are:

        --help                Produce help message

        --domain arg          Device Domain
  
        --device-id arg       Device ID for identification
        
        --host                Host Ip_Address
        
        --database            Database Name

        --log-info arg        Log information category

        --log-data arg        Log data category
 
        --log4cpp-conf arg    Log configuration and format specification file

Example

    ./bp-sub-persist --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.persist --log4cpp-conf ../src/c++/production/conf/

      simulation_log_bp_sub.conf

  * Once the blood pressure persistence is started it  will update the data in to the data base and displays the data in the log file.

NOTE :` The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**3.5 Websocket**
  
  * Start the `Bloodpressure Webserver` in the terminal by passing various options suffix to the command

        $./bp-sub-web 

Available Options are:
        
        --web server name      Webserver Name
        
        --port_no              Port Number
        
  Example
      
        $./bp-sub-web netspective-webserver.com 9003
        
  * Once the blood pressure subscriber started as webserver the data will be recieved on the web browser.
  
    The web browser can be access data through the following URL 
      
      `http://netspective-webserver.com/medigraph/index.php?domain_id=DOMAINID&device_id=DEVICEID` 

NOTE: `Domain name and device id are missed in the URL then the entire device data will be displayed.`

              
**PULSE OXIMETER:**

**4.1.** `Pulse oximeter publisher` shall be started by passing the various options suffix to the command .

        $./pulseox-pub 

Available options are:

        --help                Produce help message

        --data-gen-ip arg     Data Generator IP 
        
        --data-gen-port       Data Generator Port No.

        --domain arg          Device Domain 
       
        --device-id arg       Device ID - for device identification

        --log-info arg        Log information category

        --log-data arg        Log data category  

        --log4cpp-conf arg    Log configuration and format specification file

Example :

       ./pulseox-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain pulse --device-id PULSE_LAB3 --log-info pulse.info --log-data pulse.data --log4cpp-conf ../src/c++/production/conf/simulation_log_pulse.conf

  * Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

NOTE :` The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**4.1.1.** `Multiple publishers` need to publish the data on the topic use the following command with the options

      ./pulseox-pub.sh --

Available Options are:

        --domain        Device Domain
        
        --device-id     Device ID to identify the particular device
        
        --spawn         Number of publishers to run
        
        --log4cpp-conf  Configration file path

EXAMPLE:
        $./pulse-oximeter-pub.sh --domain=pulse --device-id=Pulse --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf

NOTE : `The arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format`

**4.2.** Start the `pulse oximeter subscribers` on the other terminal using by passing various options suffix to the command,

      $./pulseox-sub-echo -

Available options are:

      --help                Produce help message

      --domain arg          Device Domain 

      --device-id arg       Device ID - for device identification

      --log-info arg        Log information category 

      --lod-data arg        Log data category 

      --log4cpp-conf arg    Log configuration and format specification file

Example:

      $ ./pulse-sub-echo --domain pulse --device-id Pulse_LAB3 --log-info pulse.info --log-data pulse.echo --log4cpp-conf ../src/c++/production/conf/

        simulation_log_pulse_sub.conf     

  * Once the pulse oximeter subscriber is started it will retrieve data from the Topic. Subscriber uses ContentFilterTopic to retrieve messages based on the Device ID from a single topic.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`
 
**4.3.** Start the pulseox oximeter alarm on the other terminal using by passing various options suffix to the command

        $./pulseox-sub-alarm -

Available options are:

        --help               Produce help message

        --domain arg         Device Domain

        --device-id arg      Device ID for identification
  
        --log-info arg       Log information category
  
        --log-data arg       Log data category 
 
        --log4cpp-conf arg   Log configuration and format specification file
  
        --spo2-low arg       SPO2 low level alarm specification - default <88
  
        --spo2-high arg      SPO2 high level alarm specification - default >92

Example:

        $./pulseox-sub-alarm --domain pulse --device-id PULSE_LAB3 --log-info pulse.info --log-data pulse.alarm --log4cpp-conf ../src/c++/production/conf/

        simulation_log_pulse_sub.conf

  * Once the pulse oximeter alarm is started will retrieve the data and the displays in log file based on the default assessment or from the specified 

  arguments.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**4.4.** Start the pulse oximeter persist on the other terminal using by passing various options suffix to the command 

        $./pulseox-sub-persist -- 

Available options are:
  
        --help               Produce help message
  
        --domain arg         Device Domain
  
        --device-id arg      Device ID for identification
        
        --host               Host Ip_Address
        
        --database           Database Name
  
        --log-info arg       Log information category
  
        --log-data arg       Log data category
  
        --log4cpp-conf arg   Log configuration and format specification file

Example:
 
        $./pulseox-sub-persist --domain pulse --device-id PULSE_LAB3 --log-info pulse.info --log-data pulse.persist --log4cpp-conf ../src/c++/production/conf/

          simulation_log_pulse_sub.conf
  
  * Once the pulse oximeter persistence is started it will update the data in to the database and displays the data in the log file.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**4.5 Websocket**
  
  * Start the `Pulseox Webserver` in the terminal by passing various options suffix to the command

        $./pulseox-sub-web 

Available Options are:
        
        --web server name      Webserver Name
        
        --port_no              Port Number
        
  Example
      
        $./pulseox-sub-web netspective-webserver.com 9003
        
  * Once the pulseox subscriber started as webserver the data will be recieved on the web browser.
  
    The web browser can be access data through the following URL 
      
      `http://netspective-webserver.com/medigraph/index.php?domain_id=DOMAINID&device_id=DEVICEID` 

NOTE: `Domain name and device id are missed in the URL then the entire device data will be displayed.`

**TEMPERATURE MONITOR:**

**5.1.** Temperature monitor publisher shall be started by passing the various options suffix to the command.

    $./tempmonitor-pub --

Available options are:
  
        --help                Produce help message

        --data-gen-ip arg     Data Generator IP 

        --data-gen-port       Data Generator Port No.
        
        --domain arg          Device Domain 

        --device-id arg       Device ID - for device identification

        --log-info arg        Log information category

        --log-data arg        Log data category  

        --log4cpp-conf arg    Log configuration and format specification file

Example :

        $./tempmonitor-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain temp --device-id TEMP_LAB3 --log-info temp.info --log-data temp.data --log4cpp-conf ../src/c++/production/conf/simulation_log_temp.conf/simulation_log_temp.conf


  * Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`


**5.1.1**  `Multiple publishers` need to publish the data on the topic use the following command with the options

      $./tempmonitor-pub.sh --

Available options are:

        --domain        Device Domain
        
        --device-id     Device ID to identify the particular device
        
        --spawn         Number of publishers to run
        
        --log4cpp-conf  Configration file path

EXAMPLE:
       
        $./temperature-pub.sh --domain=blood --device-id=BP --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf
  
  
 NOTE : `The arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format`

 
**5.2.** Start the `temperature monitor subscribers` on the other terminal by passing the various options suffix to the command ,

          $./tempmerature-sub-echo

Available options are:

        --help                  Produce help message

        --domain arg            Device Domain 

        --device-id arg         Device ID - for device identification

        --log-info arg          Log information category

        --lod-data arg      		Log data file 

        --log4cpp-conf arg      Log configuration and format specification file

Example:

        $ ./temperature-sub-echo --domain temp --device-id Temp_LAB123 --log-info temp.info --log-data temp.echo --log4cpp-conf ../src/c++/production/conf/simulation_log_temp_sub.conf  

  * Once the temp subscriber is started it will retrieve data from the Topic. Subscriber uses ContentFilterTopic to retrieve messages based on the Device ID from a single topic.

NOTE :  `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**5.3.**  Start the `temperature-monitor alarm` by passing the various options suffix to the command ,

        $./tempmerature-sub-alarm

Available options are:

        --help		              Produce help message

        --domain arg	    	    Device Domain
  
        --device-id arg         Device ID for identification
  
        --log-info arg          Log information category
  
        --log-data arg          Log data category 
  
        --log4cpp-conf arg      Log configuration and format specification file
  
        --avg-time-period arg   Average time period for temperature - default 1 min
  
        --temp-low arg          Temperature low level alarm  specification -default<88
  
        --temp-high arg         Temperature high level alarm specification-default>92

Example:

        $ ./temperature-sub-alarm --domain pulse --device-id TEMP_LAB3 --log-info temp.info --log-data temp.alarm --log4cpp-conf ../src/c++/production/conf/simulation_log_temp_sub.conf

  * Once the pulse oximeter alarm is started it will retrieve the data and the displays in log file based on the default assessment or from the specified 

arguments.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**5.4.** Start the `temperature-monitor persistence` by passing the various options suffix to the command,

         $./temperature-sub-persist 

Available options are:

        --help                Produce help message

        --domain arg          Device domain

        --device-id arg       Device ID for identification
        
        --host                Host Ip_Address
        
        --database            Database Name

        --log-info arg        Log information category
  
        --log-data arg        Log data category 
  
        --log4cpp-conf arg    Log configuration and format specification file

Example:

        $./temperature-sub-persist --domain pulse --device-id TEMP_LAB3 --log-info temp.info --log-data temp.persist --log4cpp-conf ../src/c++/production/conf/

        simulation_log_temp_sub.conf

  * Once the temperature monitor persistence is started it will update the data in to the database and displays the data in the log file.

NOTE : `The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.`

**5.5 Websocket**
  
  * Start the `Teperature Monitor Webserver` in the terminal by passing various options suffix to the command

        $./temperature-sub-web 

Available Options are:
    
        --web server name      Webserver Name
        
        --port_no              Port Number
        
  Example
      
        $./temperature-sub-web netspective-webserver.com 9003
        
  * Once the temperature subscriber started as webserver the data will be recieved on the web browser.
  
  The web browser can be access data through the following URL 
      
      `http://netspective-webserver.com/medigraph/index.php?domain_id=DOMAINID&device_id=DEVICEID` 

NOTE: `Domain name and device id are missed in the URL then the entire device data will be diaplyed.`

STEPS TO RUN THE DISTRIBUTED APPLICATION:
========================================

**1.** From the installation location of OpenspliceDDS, we need to change the few confiuration in the ospl config file located in,

          ${OSPL_INSTALLTION_DIR}/HDE/x86_64.linux2.6/etc/config/ospl.xml
 
**2.** Following statements should be modified in the config file,
    
        <General>

        <NetworkInterfaceAddress> first available </NetworkInterfaceAddress>

        </General>

        <Partitioning> <GlobalPartition Address="broadcast,Central-IP"/> </Partitioning>

* In config file the above statements should be included and the IP_Address should be entered into the specified locations.


NOTE : `The same configuration changes needs to reflected in all the machines running in the same domain.`


**3.** Architecture of Distributed Application will be  as following structure:

  
  `DataGenerator & Publishers[OsplDDS]  ---------------------->   Central[OsplDDS]  -------------------->   Subscribers[OsplDDS]`
      
    
    

**4.** In the given architecture each entities can reside in different machines or publishers and data generator can be reside in same machine.

**5.** Central OsplDDS acts as an gateway between the publishers and the subscribers.

**6.** Publisher can reside in different machine and makes the request to the data generator to generate the data randomly and receives the data .

**7.** Publishers & Subscribers Ospl should contain the IP_Address of central ospl IP_Address in its config file.

**8.** The process of all the entities will remain the same as single machine implementation and the extension is it has been distributed with few 

configurations.
     











