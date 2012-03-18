# Development Libraries

GCC is an integrated distribution of compilers for several major programming languages.

	yum groupinstall "Development Tools"
	yum install zlib zlib-devel bzip2 bzip2-devel python python-devel libicu gcc44 gcc44-c++ pcre pcre-devel openssl openssl-devep httpd php

For Ubuntu
              
	aptitude install build-essential
	apt-get install zlib1g zlib1g-dbg zlib1g-dev libbz2-1.0 python python python-dev libicu-dev libpcre3 libpcre3-dev  libssl-dev apache2 php5

# Boost Library

Boost libraries are intended to be widely used, and usable across a broad spectrum of applications.

Check for existing boost library, if its exists remove using the following command

	rpm -qa|grep boost
	rpm -e <package_name>

For Ubuntu

	dpkg --get-selections | grep boost
	dpkg --remove <package_name>

Download and Install

Download  [Boost.1.48.0](http://www.boost.org/users/download/) of boost library

Extract the downloaded boost.1.48.0.tar.gz file using the following command

	tar -xvf boost_1_48_0.tar.gz

Change to installation directory and run the following command. Follow the instructions until the installation was successful.

	./bootstrap.sh

Required create symbolic link with the appropriate path

	ln -s <Boost_Install_Directory>/stage/lib/* /lib
	ln -s <Boost_Install_Directory>/boost /usr/include

Copy and Paste following line to ~/.bash_profile to set the environment variables.

	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
