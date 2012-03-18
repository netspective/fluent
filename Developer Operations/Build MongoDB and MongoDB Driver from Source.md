# MongoDB Server
 
Download the version of mongodb-linux-x86_64-2.0.2 from the link .[Click here to download](http://www.mongodb.org/downloads)
 
Extract the mongodb-linux-x86_64-2.0.2.tar.gz  tar file using the command and change it into the bin/directory 

	tar -xvf mongodb-linux-x86_64-2.0.2.tar.gz
    cd /mongodb-linux-x86_64-2.0.2/bin/
        
To check successful installation ,execute database server binary file using below given command

	mkdir -p /data/db/
    chown `id -u` /data/db
    ./monogd

To open a client database connection execute the binary file using below given command

	./mongo
	> use <DATABASE_NAME>
	> db.addUser('<USERNAME>','<PASSWORD>');
	> db.auth('<USERNAME>','<PASSWORD>');
         
## MongoDB C++ Driver 
 
Download the C++ driver(mongodb-linux-x86_64-v1.8)from the link.[Click here to download](http://downloads.mongodb.org/cxx-driver/mongodb-linux-x86_64-v1.8-latest.tgz)

Extract  mongodb-linux-x86_64-v1.8-latest.tgz file using the command  

	tar -xvf mongodb-linux-x86_64-v1.8-latest.tar.gz 
 
Download **SCons** version scons-2.1.0-1.noarch.rpm from the folowing link.[Click here to download](http://sourceforge.net/projects/scons/files/scons/2.1.0/scons-2.1.0-1.noarch.rpm/download)

	rpm -ivh scons-2.1.0-1.noarch.rpm

For Ubuntu

	apt-get install scons

To compile the "standalone" C++ driver, run the scons command in the installation directory of the driver 

	cd mongo-cxx-driver-v1.8
	scons

Startup Script for MONGODB shall be used from the /etc/ini.d/ directory

## Startup

	#!/bin/bash
	#DESCRIPTION:MONGODB STARTUP SCRIPT
	PROGRAM=/opt/mongodb-linux-x86_64-2.0.2/bin/mongod
	MONGOPID=`ps -ef | grep 'mongod' | grep -v grep | awk '{print $2}'`
	case "$1" in
		start)
			echo "Starting MongoDB server"
			/opt/mongodb-linux-x86_64-2.0.2/bin/mongod --fork --quiet --dbpath /data/db  --logpath /var/log/mongodb.log
			echo "Started MongoDB Server\n"
			;;
		stop)
			echo "Stopping MongoDB server"
			if [ ! -z "$MONGOPID" ]; then
				kill -9 $MONGOPID
			fi
			;;
		*)
			echo "Usage: /etc/init.d/mongodb {start|stop}"
			exit 1
	esac
	exit 0

Example:

	/etc/init.d/mongodb {start/stop}
