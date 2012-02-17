############################################################################################
#                                                                                          #
#                 <h2>Netspective Fluent M2M Platform</h2>                                 #
#                                                                                          #
############################################################################################

<h4>OVERVIEW:</h4>
------------------

Netspective Fluent consists of interfaces to retrieve data from connected devices through RS232, 
Ethernet, USB, GSM, and other connection channels. 

<h4>REQUIREMENTS:</h4>
----------------------
The requirements for compiling and running :

* OpenSplice DDS v5.x  

* BOOST v1.48(or higher)

* GCC/G++ v4.1(or higher)

* Log for c++ 1.0(or higher)

* Websocketpp (WebSocket C++ Library)

* Syslog Configuration
  
<h4>INSTALLATION STEPS</h4>

<h6>OPENSPLICEDDS</h6>
----------------------

Note: Keep all the packages in /opt path and Troubleshooting steps available in bottom of this document

OpenSplice DDS is one of several open source implementation of the OMG Data Distribution Service for Real-Time Systems (DDS) standard.
  
* OpenJDK is pre-requisite for OpenSpliceDDS. Install latest version of OpenJDK (JDK 1.5 minimum required).

* Download the OpenSplice  version of **OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz** from the following link.[Click here to download](http://www.prismtech.com/opensplice/opensplice-dds-community/software-downloads)
        
* Extract the downloaded tar file with following command, after extracted the tar file you could find the HDE folder and configure the release.com with installation directory.

        $ tar -xvf OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz

* Use the given commands to setup the environment and start DDS service

* Replace "@@INSTALLDIR@@" with the installed opsl directory path

      	$ source release.com
      
      	$ ospl start

<h6>DEVELOPMENT LIBRARIES</h6>

* GCC is an integrated distribution of compilers for several major programming languages.

        $ yum groupinstall "Development Tools"
        
        $ yum install zlib zlib-devel bzip2 bzip2-devel python python-devel libicu gcc44 gcc44-c++ pcre pcre-devel openssl openssl-devep httpd
              
<h6>BOOST LIBRARY</h6>

Boost libraries are intended to be widely used, and usable across a broad spectrum of applications. 

* Check for existing boost library,if it's exists remove using the following command   

        $ rpm -qa|grep boost
  
        $ rpm -e <package_name>

* Download  Boost.1.48.0 version of boost library [Click here to download](http://www.boost.org/users/download/)

* Extract  boost.1.48.0.tar.gz file using the following command

      	$ tar -xvf boost_1_48_0.tar.gz

* Change to installation directory and run the following command. Follow the instructions until the installation was successful.

        $ ./bootstrap.sh
	
* Required create symbolic link with the appropriate path

        $  ln -s /opt/boost_1_48_0/stage/lib/* /lib
 
        $  ln -s /opt/boost_1_48_0/boost /usr/include

<h6>LOG FOR C++</h6>

* Log4cpp is library of C++ classes for logging to files, syslog and other destinations.[Click here to download](http://sourceforge.net/projects/log4cpp/files/) for log4cpp libraries. Follow the steps given below to install Log4cpp

        $ tar -xvf log4cpp-1.0.tar.gz
      
        $ cd log4cpp-1.0/
                     
        $ ./configure
        
        $ make
       
        $ make check

        $ make install
        
<h4>MONGO-DB  INSTALLATION:</h4>
-------------------------------

<h6>MONGODB CLIENT SERVER:</h6>
 
 * Download the version of mongodb-linux-x86_64-2.0.2 from the link .[Click here to download](http://www.mongodb.org/downloads)
 
 * Extract the mongodb-linux-x86_64-2.0.2.tar.gz  tar file using the command and change it into the bin/directory 

        $ tar -xvf mongodb-linux-x86_64-2.0.2.tar.gz

        $ cd /mongodb-linux-x86_64-2.0.2/bin/
        
 * To check successful installation ,execute database server binary file using below given command

        $ mkdir -p /data/db/
        
        $ chown `id -u` /data/db
        
        $ ./monogd

 * To open a client database connection execute the binary file using below given command

        $ ./mongo
        
        $ > use <DATABASE_NAME>
        
        $ > db.addUser('<USERNAME>','<PASSWORD>');
        
        $ > db.auth('<USERNAME>','<PASSWORD>');
         
<h6>C++ DRIVER :</h6>
 
* Download the C++ driver(mongodb-linux-x86_64-v1.8)from the link.[Click here to download](http://downloads.mongodb.org/cxx-driver/mongodb-linux-x86_64-v1.8-latest.tgz)

* Extract  mongodb-linux-x86_64-v1.8-latest.tgz file using the command  

        $ tar -xvf mongodb-linux-x86_64-v1.8-latest.tar.gz 
 
* Download **SCons** version scons-2.1.0-1.noarch.rpm from the folowing link.[Click here to download](http://sourceforge.net/projects/scons/files/scons/2.1.0/scons-2.1.0-1.noarch.rpm/download)
	  
        $ rpm -ivh scons-2.1.0-1.noarch.rpm
        
* To compile the "standalone" C++ driver, run the scons command in the installation directory of the driver 

        $ cd mongo-cxx-driver-v1.8
                                
        $ scons

<h4>SYSLOG INSTALLATION:</h4>

Syslog supported by a wide variety of devices and receivers across multiple platforms and can be used to integrate log data from many different types of systems into a central repository.

* Syslog installation should be done in **Subscriber machine**  
  
* Install the rsyslog.x86_64 version using the following command

        $ yum install rsyslog.x86_64

* Once the installation got completed update the configurations in the file /etc/syslog.conf with the **server ip address:port.**

         @@ 0.0.0.0:514
 
* Command used to restart the syslog are given below

        $ /etc/init.d/syslog restart
                         
<h4>GRAYLOG2 INSTALLATION:</h4>
------------------------------

</h6>Graylog2 Server:</h6>

* Download the version of graylog2 server0.9.6 from the following link.[Click here to download](https://github.com/Graylog2/graylog2-server/downloads)
 
* Extract the graylog2 server tar file using the command and change to the installation directory

        $ tar -xvf graylog2-server-0.9.6.tar.gz
      
        $ cd graylog2-server-0.9.6/
    
* Create a copy graylog2.conf using the example configuration file.The paremeters to be updated are listed below 

        $ cp graylog2.conf.example graylog2.conf

          Parameters:
          
          syslog_listen_port = 514 [default]
        
          syslog_protocol = udp [default]

          #MongoDB & ElasticSearch configuration 

          elasticsearch_url = http://<ipaddress>:portno/
        
          elasticsearch_index_name = graylog2
        
          mongodb_useauth = true

          mongodb_user = <USERNAME>

          mongodb_password = <PASSWORD>
    
          mongodb_host = <ipaddress>
       
          #mongodb_replica_set = localhost:27017,localhost:27018,localhost:27019 [default]
        
          mongodb_database = <DATABASE_NAME>
          
          mongodb_port = 27017
   
* Start the graylog2-server using given below commands

         $ java -jar graylog2-server.jar -f ./graylog2.conf
  
<h6>Elastic Search </h6>

* Download elasticsearch 0.18.6 version from the link [Click here to download](http://www.elasticsearch.org/download/) and unpack it, 

        $ tar -xvf elasticsearch-0.18.6.tar.gz
		 
        $ cd elasticsearch-0.18.6
		
* Create a data and log directory

        $ mkdir /var/data/elasticsearch
		
        $ mkdir /var/log/elasticsearch

* Configure basic elasticsearch values in the existing config/elasticsearch.yml [elasticsearch configurations details](http://www.elasticsearch.org/guide/reference/setup/configuration.html)

        network.host: <elasticsearch_ipaddress>
        
        path.logs : /var/log/elasticsearch
        
        path.data : /var/data/elasticsearch
        
        cluster.name : graylog2 
        
        http port : <PORT>
        
        http.enabled: true
        
        http.max_content_length: 100mb

        
* Download elasticsearch-servicewrapper into your elasticserach/bin installation directory and unpack it there,using the following commands from the terminal 

        $ wget  --no-check-certificate https://github.com/elasticsearch/elasticsearch-servicewrapper/zipball/master
  
        $ mv master elasticsearch-servicewrapper.zip && unzip elasticsearch-servicewrapper.zip
	
        $ mv elasticsearch-elasticsearch-servicewrapper-*/* . && rm -rf elasticsearch-elasticsearch-servicewrapper-*

* Update the set.default.ES_HOME path in bin/service/elasticsearch.conf to elastic search installation directory.
	
* Start elasticsearch instance using the command

        $ ./elasticsearch start 

* Elasticsearch instance started successfully the we can check it in the log files either in the same directory or default in **/var/log/elasticsearch/graylog2.log**
 
**Note:The configurations in elasticsearch.yml,graylog2.conf should be as common.**
 
<h6>Graylog2-Web Interface:</h6>

* Download  version of graylog2-web interface 0.9.6 from the link.[Click here to download](https://github.com/Graylog2/graylog2-web-interface/downloads)
 
* Extract the downloaded zip file using the following command abd change to the installation directory

        $ tar -xvf graylog2-webinterface-0.9.6.tar.gz.
		
        $ cd graylog2-webinterface-0.9.6/
 
* Update config/indexer.yml with

        production
      
        url: http://0.0.0.0:0000/
         
        index_name: graylog2

* Update config/mongoid.yml with

        production:
         
         host: <mongodb:ipaddress>
      
         port: <mongodb:portno>
       
         username: <USERNAME>
        
         password: <PASSWORD>
        
         database: <DATABASE_NAME>


* Install the latest version of ruby on rails which should be 1.9.2,follow the steps for installation by using below steps shall make to install successfully   

* Check for older version RUBY installed via RPM using the following command change to the previous directory, 

         $ cd ../

         $ rpm -qa |grep "ruby"

* If older version exists, uninstall it before proceeding using the command

        $ rpm -e package_name
        
        $ wget ftp://ftp.ruby-lang.org//pub/ruby/1.9/ruby-1.9.2-p0.tar.gz

        $ tar -xvf ruby-1.9.2-p0.tar.gz

        $ cd ruby-1.9.2-p0/

        $ ./configure --prefix=/usr/local/ruby
	
        $ make 
        
        $ make install

* Export the PATH and GEM_HOME enviornment variables as given below.

        $ export export PATH=/usr/local/ruby/bin:/opt/ruby-1.9.2-p0/bin:$PATH

        $ export export GEM_HOME=/opt/ruby-1.9.2-p0

* Once the install is complete, verify the version of Ruby:

        $ ruby -v
                   
        ruby 1.9.2p0 (2010-08-18 revision 29036)

* Ruby source package also installs RubyGems, the Ruby package manager. Verify the version of RubyGems:

        $ gem -v
          
        1.3.7

* Check for updated gems and ensure you have the latest gem versions by running this command:

        $ gem update --system

* Install the rake build language

        $ gem install rake

* Install rails

        $ gem install rails
       
* List the installed gems.

        $ gem list
        
* Change to the web interface from the installation folder by using the command

        $ cd <WEB_INTERFACE_INSTALLTION_PATH>
	
* To make successful installation of all files from gem package, use the command to install missing libraries 

        $ gem install bundler 
        
        $ bundle install 

* Start the web inrterface using the following command

        $ script/rails server -e production -p <PORTNO>

 **Note:The configured database name,user name,password, ipaddress,port numbers of MongoDB in graylog2.conf,mongoid.yml should be as common.**
 
**WEBSOCKETPP LIBRARY**

* Dowload the websocketpp library version **zaphoyd-websocketpp-e94825b.zip** from the following link.[Click here to download](https://github.com/zaphoyd/websocketpp)

* Unzip the webscocket library using the following command 

        $ unzip zaphoyd-websocketpp-e94825b.zip

* Change to the installation directory and build static library using the following commands

        $ cd zaphoyd-websocketpp-e94825b/
       
        $ make 
       
        $ make install
    
<h4>COMPILATION STEPS</h4>
--------------------------

* Set OpenSliceDDS environment variables from the Opensplice installed directory as shown below.

        $ source /../../HDE/x86.linux2.6/release.com

        
* Run the following command to clean the build files.

        $ make clean

* Run the makefile

        $ cd support/build
        
        $ make

* After successful completion of compilation, binary files will be created in following directory.

        $ cd ../../bin/


<h4>Apache Configuration </h4>

        $ vim /etc/httpd/conf/httpd.conf
        
	* append the following configuration 
 
	Listen 80
	<VirtualHost *:80>
	DocumentRoot "/opt/netspective-fluent/src/web"
	<Directory "/opt/netspective-fluent/src/web">
	DirectoryIndex index.html index.htm index.php
	Options Indexes FollowSymLinks
	AllowOverride None
	Order allow,deny
	Allow from all
	Options +ExecCGI
	AddHandler cgi-script cgi pl
	</Directory>
	</VirtualHost>



<h4>STEPS TO RUN THE APPLICATIONS </h4>

**1**. Set OpenSpliceDDS environment using the following command

        $ source /opt/HDE/x86.linux2.6/release.com

**2**. Start OpenSpliceDDS

        $ ospl start

**3**. Start the data generator at the terminal using the following command

        $ ./data-generator <ipaddress> <port>
     
  * Data Generator will wait until it receives a request from publisher and once the request is received then it generates the data randomly corresponding to the publisher and send it to the publisher. The request can be from any of the three publishers listed below,
     
<h6>BLOOD PRESSURE:</h6>

**3.1.** Start the **blood pressure publisher** shall be started by passing the various options suffix to the command .

        $ ./bp-pub --

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
    
       $ ./bp-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.data --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf

  * Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format**

**3.1.1 Multiple publishers** need to publish the data on the topic use the following command with the options

       $ ./bp-pub.sh --

Available Options are:

        --domain        Device Domain
        
        --device-id     Device ID to identify the particular device
        
        --spawn         Number of publishers to run
        
        --log4cpp-conf  Configration file path

EXAMPLE:
       
       $ ./bp-pub.sh --domain=blood --device-id=BP --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf

**NOTE :The  arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**3.2**. Start the **blood pressure subscribers** on the other terminal by passing the various options suffix to the command .

       $ ./bp-sub-echo --

Available options are:

        --help                  Produce help message

        --domain arg            Device Domain 

        --device-id arg         Device ID - for device identification

        --log-info arg          Log information category

        --lod-data arg   	      Log data category

        --log4cpp-conf arg      Log configuration and format specification file

Example:

       $ ./bp-sub-echo --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.echo --log4cpp-conf ../src/c++/production/conf/simulation_log_bp_sub.conf         
      
  * Once the blood pressure subscriber is started it will retrieve data from the Topic. Subscriber uses ContentFilterTopic to retrieve messages based on the Device ID from a single topic.
  
**NOTE: The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**3.3**. Start the **blood pressure alarm** on the other terminal by passing the various options suffix to the command. 

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

       $ ./bp-sub-alarm --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.alarm --log4cpp-conf ../src/c++/production/conf/simulation_log_bp_sub.conf

  * Once the blood pressure alarm is started it will retrieve the data and the displays in log file based on the default assessment or from the specified arguments.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**3.4**. Start the **blood pressure persists** in the other terminal by passing the various options suffix to the command. 

       $ ./bp-sub-persist 

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
  
       $ ./bp-sub-persist --domain blood --device-id BP_LAB3 --log-info blood.info --log-data blood.persist --log4cpp-conf ../src/c++/production/conf/simulation_log_bp_sub.conf

  * Once the blood pressure persistence is started it  will update the data in to the data base and displays the data in the log file.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

<h6>PULSE OXIMETER:</h6>

**4.1. PulseoxMeter publisher** shall be started by passing the various options suffix to the command .

        $ ./pulseox-pub 

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

       $ ./pulseox-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain pulse --device-id PULSE_LAB3 --log-info pulse.info --log-data pulse.data --log4cpp-conf ../src/c++/production/conf/simulation_log_pulse.conf

  * Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

**NOTE :The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**4.1.1. Multiple publishers** need to publish the data on the topic use the following command with the options

       $ ./pulseox-pub.sh --

Available Options are:

        --domain        Device Domain
        
        --device-id     Device ID to identify the particular device
        
        --spawn         Number of publishers to run
        
        --log4cpp-conf  Configration file path

EXAMPLE:
       
       $ ./pulse-oximeter-pub.sh --domain=pulse --device-id=Pulse --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf

**NOTE :The arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format**

**4.2.** Start the **pulseox meter subscribers** on the other terminal using by passing various options suffix to the command,

       $ ./pulseox-sub-echo -

Available options are:

      --help                Produce help message

      --domain arg          Device Domain 

      --device-id arg       Device ID - for device identification

      --log-info arg        Log information category 

      --lod-data arg        Log data category 

      --log4cpp-conf arg    Log configuration and format specification file

Example:

      $ ./pulse-sub-echo --domain pulse --device-id Pulse_LAB3 --log-info pulse.info --log-data pulse.echo --log4cpp-conf ../src/c++/production/conf/simulation_log_pulse_sub.conf     

  * Once the pulse oximeter subscriber is started it will retrieve data from the Topic. Subscriber uses ContentFilterTopic to retrieve messages based on the Device ID from a single topic.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**
 
**4.3.** Start the **pulseox meter alarm** on the other terminal using by passing various options suffix to the command

      $ ./pulseox-sub-alarm --

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

       $ ./pulseox-sub-alarm --domain pulse --device-id PULSE_LAB3 --log-info pulse.info --log-data pulse.alarm --log4cpp-conf ../src/c++/production/conf/simulation_log_pulse_sub.conf

  * Once the pulse oximeter alarm is started will retrieve the data and the displays in log file based on the default assessment or from the specified arguments.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**4.4.** Start the **pulseox meter persists** on the other terminal using by passing various options suffix to the command

       $ ./pulseox-sub-persist -- 

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
 
       $ ./pulseox-sub-persist --domain pulse --device-id PULSE_LAB3 --log-info pulse.info --log-data pulse.persist --log4cpp-conf ../src/c++/production/conf/simulation_log_pulse_sub.conf
  
  * Once the pulse oximeter persistence is started it will update the data in to the database and displays the data in the log file.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

<h6>TEMPERATURE MONITOR:</h6>

**5.1. Temperature monitor** publisher shall be started by passing the various options suffix to the command

       $ ./temperature-pub --

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

       $ ./temperature-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain temp --device-id TEMP_LAB3 --log-info temp.info --log-data temp.data --log4cpp-conf ../src/c++/production/conf/simulation_log_temp.conf/simulation_log_temp.conf

  * Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**5.1.1. Multiple publishers** need to publish the data on the topic ,using the following command with the options

       $ ./temperature-pub.sh --

Available options are:

        --domain        Device Domain
        
        --device-id     Device ID to identify the particular device
        
        --spawn         Number of publishers to run
        
        --log4cpp-conf  Configration file path

EXAMPLE:
       
        $ ./temperature-pub.sh --domain=blood --device-id=BP --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf
    
**NOTE : The arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format**

**5.2.** Start the **temperature-monitor subscribers** on the other terminal by passing the various options suffix to the command ,

        $ ./tempmerature-sub-echo

Available options are:

        --help                  Produce help message

        --domain arg            Device Domain 

        --device-id arg         Device ID - for device identification

        --log-info arg          Log information category

        --lod-data arg      	Log data file 

        --log4cpp-conf arg      Log configuration and format specification file

Example:

       $ ./temperature-sub-echo --domain temp --device-id Temp_LAB123 --log-info temp.info --log-data temp.echo --log4cpp-conf ../src/c++/production/conf/simulation_log_temp_sub.conf  

  * Once the temp subscriber is started it will retrieve data from the Topic. Subscriber uses ContentFilterTopic to retrieve messages based on the Device ID from a single topic.

**NOTE :  The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**5.3.**  Start the **temperature-monitor alarm** by passing the various options suffix to the command ,

       $ ./tempmerature-sub-alarm

Available options are:

        --help		        Produce help message

        --domain arg	        Device Domain
  
        --device-id arg         Device ID for identification
  
        --log-info arg          Log information category
  
        --log-data arg          Log data category 
  
        --log4cpp-conf arg      Log configuration and format specification file
  
        --avg-time-period arg   Average time period for temperature - default 1 min
  
        --temp-low arg          Temperature low level alarm  specification -default<88
  
        --temp-high arg         Temperature high level alarm specification-default>92

Example:

       $ ./temperature-sub-alarm --domain pulse --device-id TEMP_LAB3 --log-info temp.info --log-data temp.alarm --log4cpp-conf ../src/c++/production/conf/simulation_log_temp_sub.conf

  * Once the pulse oximeter alarm is started it will retrieve the data and the displays in log file based on the default assessment or from the specified arguments.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**5.4.** Start the **temperature-monitor persists** by passing the various options suffix to the command,

       $ ./temperature-sub-persist 

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

       $ ./temperature-sub-persist --domain pulse --device-id TEMP_LAB3 --log-info temp.info --log-data temp.persist --log4cpp-conf ../src/c++/production/conf/simulation_log_temp_sub.conf

  * Once the temperature monitor persists is started it will update the data in to the database and displays the data in the log file.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

<h6> ECG </h6>

**6.1. Ecg publisher** shall be started by passing the various options suffix to the command.

       $ ./ecg-pub --

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

       $ ./ecg-pub --data-gen-ip 127.0.0.1 --data-gen-port 5000 --domain ECG --device-id ECG_LAB44 --log-info ecg.info --log-data ecg.data --log4cpp-conf ../src/c++/production/conf/simulation_log_ecg.conf 

* Once the publisher binds with the data generator and send a command, it receives data from data-generator and displays the data in the log files.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**

**6.2.** Start the **ecg subscribers** on the other terminal by passing the various options suffix to the command ,

       $ ./ecg-sub-echo

Available options are:

        --help                  Produce help message

        --domain arg            Device Domain 

        --device-id arg         Device ID - for device identification

        --log-info arg          Log information category

        --lod-data arg        	Log data file 

        --log4cpp-conf arg      Log configuration and format specification file

Example:

       $ ./ecg-sub-echo --domain ECG --device-id ECG_LAB44 --log-info ecg.info --log-data ecg.echo --log4cpp-conf ../src/c++/production/conf/simulation_log_ecg_sub.conf
  
* Once the ecg subscriber is started it will retrieve data from the ContentFilterTopic to retrieve messages based on the Device ID from a single topic.

**NOTE : The category name arguments passed to the application needs to be configured in the log4cpp configuration file with the appender and layout format.**


<h4>STEPS TO RUN THE DISTRIBUTED APPLICATION:</h4>

**1.** From the installation location of OpenspliceDDS, we need to change the few confiuration in the ospl config file located in,

          $ {OSPL_INSTALLTION_DIR}/HDE/x86_64.linux2.6/etc/config/ospl.xml
 
**2.** Following statements should be modified in the config file,
    
        <General>

        <NetworkInterfaceAddress> first available </NetworkInterfaceAddress>

        </General>

        <Partitioning> <GlobalPartition Address="broadcast,Central-IP"/> </Partitioning>

* In config file the above statements should be included and the IP_Address should be entered into the specified locations.


**NOTE : The same configuration changes needs to reflected in all the machines running in the same domain.**


**3.** Architecture of Distributed Application will be  as following structure:

  
 **DataGenerator & Publishers[OsplDDS]  ---------------------->   Central[OsplDDS]  -------------------->   Subscribers[OsplDDS]**
      
  
**4.** In the given architecture each entities can reside in different machines or publishers and data generator can be reside in same machine.

**5.** Central OsplDDS acts as an gateway between the publishers and the subscribers.

**6.** Publisher can reside in different machine and makes the request to the data generator to generate the data randomly and receives the data.

**7.** Publishers & Subscribers Ospl should contain the IP_Address of central ospl IP_Address in its config file.

**8.** The process of all the entities will remain the same as single machine implementation and the extension is it has been distributed with few configurations.
     
<h6>TROUBLESHOOTING</h6>

<h4>Log4cpp</h4>

* Constructor error in the Basiclayout.cpp

        $ add header file  include<memory> in src/BasicLayout.cpp.

* Parsing error in the  Patternlayout.cpp 

        $ replace the line 373 "component = new FormatModifierComponent(component, std::abs(minWidth), maxWidth, minWidth < 0);" with "component = new FormatModifierComponent(component, std::abs((float)minWidth), maxWidth, minWidth < 0);"
              
<h4>Graylog-Webinterface installation</h4>

* `require': no such file to load -- zlib (LoadError)

        $ cd /opt/ruby-1.9.2-p0/ext/zlib
        
        $ ruby extconf.rb
        
        $ make && make install 

* `require': no such file to load -- openssl (LoadError)

        $ cd /opt/ruby-1.9.2-p0/ext/openssl
        
        $ ruby extconf.rb --with--openssl=/usr/bin/openssl --with--openssl-lib=/usr/lib/openssl
        
        $ make && make install

<h4>WebServer compilation in Netspective Fluent</h4>

* error: ‘INT32_MIN’ or ‘INT32_MAX’ was not declared in this scope

        $ Replace INT32_MIN with 0 and INT32_MAX with 2147483647 
        







