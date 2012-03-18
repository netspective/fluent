# Graylog2 Installation

## Elastic Search

Download [elasticsearch 0.18.6](http://www.elasticsearch.org/download/) and unpack the downloaded file. 

	tar -xvf elasticsearch-0.18.6.tar.gz
    cd elasticsearch-0.18.6
		
Create a data and log directory

	mkdir /var/data/elasticsearch
    mkdir /var/log/elasticsearch

Configure basic elasticsearch values in the existing config/elasticsearch.yml.

	network.host: <elasticsearch_ipaddress>
	path.logs : /var/log/elasticsearch
	path.data : /var/data/elasticsearch
	cluster.name : graylog2 
	http port : <PORT>
	http.enabled: true
	http.max_content_length: 100mb

More information about configuration details, refer [link](http://www.elasticsearch.org/guide/reference/setup/configuration.html)
    
Download elasticsearch-servicewrapper and unpack into elasticserach/bin installation directory using the following commands.

	wget  --no-check-certificate https://github.com/elasticsearch/elasticsearch-servicewrapper/zipball/master
	mv master elasticsearch-servicewrapper.zip && unzip elasticsearch-servicewrapper.zip
	mv elasticsearch-elasticsearch-servicewrapper-*/* . && rm -rf elasticsearch-elasticsearch-servicewrapper-*

Update the set.default.ES_HOME path in bin/service/elasticsearch.conf to elastic search installation directory.
	
Start elasticsearch instance using the command

	./elasticsearch start 

Elasticsearch instance started successfully the we can check it in the log files either in the same directory or default in **/var/log/elasticsearch/graylog2.log**

### Startup Script

	#!/bin/bash
	#DESCRIPTION:ELASTICSEARCH STARTUP SCRIPT
	ES_HOME=/opt/elasticsearch-0.18.6
	ES_MIN_MEM=256m
	ES_MAX_MEM=2g
	DAEMON=$ES_HOME/bin/service/elasticsearch
	NAME=elasticsearch
	CONFIG_FILE=/opt/elasticsearch-0.18.6/config/elasticsearch.yml
	case "$1"  in
		start)
			$DAEMON start
			echo "Elasticsearch Started "
			;;
		stop)
			$DAEMON stop
			echo "Elasticsearch Stopped "
			;;
		*)
			N=/etc/init.d/$NAME
			echo "Usage: $N {start|stop}" >&2
			exit 1
			;;
    esac
    exit 0

Start the script using the following command, 

	/etc/init.d/elasticsearch {start/stop}

## Graylog2 Server

Download [graylog2 server0.9.6](https://github.com/Graylog2/graylog2-server/downloads)
 
Extract the graylog2 server tar file using the command and change to the installation directory

	tar -xvf graylog2-server-0.9.6.tar.gz
	cd graylog2-server-0.9.6/
    
Create a copy graylog2.conf using the example configuration file.The paremeters to be updated are listed below 

	cp graylog2.conf.example graylog2.conf

	Parameters:
          
	syslog_listen_port = 514 [default]
	syslog_protocol = udp [default]
	elasticsearch_url = http://<ipaddress>:portno/
	elasticsearch_index_name = graylog2
	mongodb_useauth = true
	mongodb_user = <USERNAME>
	mongodb_password = <PASSWORD>
	mongodb_host = <ipaddress>
	mongodb_database = <DATABASE_NAME>
	mongodb_port = 27017
   
Start the graylog2-server using given below commands

	java -jar graylog2-server.jar -f ./graylog2.conf
  
**Note:The configurations in elasticsearch.yml,graylog2.conf should be as common.**

### Startup Script

	#!/bin/sh 
	#DESCRIPTION:GRAYLOG SERVER STARTUP SCRIPT
	CMD=$1
	NOHUP=`which nohup`
	JAVA_CMD=/usr/bin/java
	GRAYLOG2_SERVER_HOME=/opt/graylog2-server-0.9.6
	start() {
		echo "Starting graylog2-server ..."
		$NOHUP $JAVA_CMD -jar $GRAYLOG2_SERVER_HOME/graylog2-server.jar -f $GRAYLOG2_SERVER_HOME/graylog2.conf &
		echo "Graylog2-server Started"
	}
	stop() {
		PID=`cat /tmp/graylog2.pid`
		echo "Stopping graylog2server $PID ..."
		kill -9 $PID
		echo "Graylog2-server Stopped $PID"
	}
	case "$CMD" in
		start)
			start
			;;
		stop)
			stop
			;;
		*)
        	echo "Usage $0 {start|stop}"
        	RETVAL=1
	esac
	exit 0
        
Start the script using the following command, 

	/etc/init.d/graylog-server {start/stop}

 
## Graylog2-Web Interface

Download [graylog2-web interface 0.9.6](https://github.com/Graylog2/graylog2-web-interface/downloads)
 
Extract the downloaded zip file using the following command abd change to the installation directory

	tar -xvf graylog2-webinterface-0.9.6.tar.gz.
	cd graylog2-webinterface-0.9.6/
 
Update config/indexer.yml with

	production
	url: http://<elasticsearch-ip>:<port>/
	index_name: graylog2

Update config/mongoid.yml with

	production:
	host: <mongodb:ipaddress>
	port: <mongodb:portno>
	username: <USERNAME>
	password: <PASSWORD>
	database: <DATABASE_NAME>

Install the latest version of ruby on rails which should be 1.9.2, follow the steps for installation by using below steps shall make to install successfully   

Check for older version RUBY installed via RPM using the following command change to the previous directory, 

	cd ../
	rpm -qa |grep "ruby"

If older version exists, uninstall it before proceeding using the command

	rpm -e package_name
	wget ftp://ftp.ruby-lang.org//pub/ruby/1.9/ruby-1.9.2-p0.tar.gz
	tar -xvf ruby-1.9.2-p0.tar.gz
	cd ruby-1.9.2-p0/
	./configure --prefix=/usr/local/ruby
	make 
	make install

Export the PATH and GEM_HOME enviornment variables as given below and add the variables in ~/.bash_profile.

	export PATH=/usr/local/ruby/bin:/opt/ruby-1.9.2-p0/bin:$PATH
	export GEM_HOME=/opt/ruby-1.9.2-p0

Once the install is complete, verify the version of Ruby:

	ruby -v
	ruby 1.9.2p0 (2010-08-18 revision 29036)

Ruby source package also installs RubyGems, the Ruby package manager. Verify the version of RubyGems:

	gem -v
	1.3.7

Check for updated gems and ensure you have the latest gem versions by running this command:

	gem update --system

Install the rake build language

	gem install rake

Install rails

	gem install rails
       
List the installed gems.

	gem list
        
Change to the web interface from the installation folder by using the command

	cd <WEB_INTERFACE_INSTALLTION_PATH>

Update the  IP Address of  **Apache** configuaration in the file app/views/layouts/application.html.erb

	<ip-or-hostname-apachehttp-server>
                    
To make successful installation of all files from gem package, use the command to install missing libraries 

	gem install bundler 
	bundle install 

Start the web inrterface using the following command

	script/rails server -e production -p <PORTNO>

**Note:The configured database name,user name,password, ipaddress,port numbers of MongoDB in graylog2.conf,mongoid.yml should be as common.**

### Startup Script

	#!/bin/bash
	#DESCRIPTION:GRAYLOG@-WEB-INTERFACE STARTUP SCRIPT
	NPATH=/opt/netspective-webinterface
	export PATH=$PATH:/usr/local/ruby/bin/
	WEBID=`ps -ef | grep 'script/rails' | grep -v grep | awk '{print $2}'`
	case "$1" in
		start)
			echo "Starting Web Interface"
			cd $NPATH
			$NPATH/script/rails server -e production &
			echo "Started Graylog2-Web-Interface"
			;;
		stop)
			echo "Stopping Web Interface"
			if [ ! -z "$WEBID" ]; then
				kill -9 $WEBID
				echo "Stopped $WEBID"
			fi
			;;
		*)
			echo "Usage: /etc/init.d/graylog-web {start|stop}"
			exit 1
	esac
	exit 0

Start the script using the following command

	/etc/init.d/graylog-web {start/stop}

## Troubleshooting

### WebInterface

`require': no such file to load -- zlib (LoadError)

        $ cd /opt/ruby-1.9.2-p0/ext/zlib
        $ ruby extconf.rb
        $ make && make install 

`require': no such file to load -- openssl (LoadError)

        $ cd /opt/ruby-1.9.2-p0/ext/openssl
        $ ruby extconf.rb --with--openssl=/usr/bin/openssl --with--openssl-lib=/usr/lib/openssl
        $ make && make install
