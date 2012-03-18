# OpenSpliceDDS

OpenSplice DDS is one of several open source implementation of the OMG Data Distribution Service for Real-Time Systems (DDS) standard.

## Pre-Requisite

	OpenJDK (Minimum Required version 1.5)s

## Download and Install

Download [OpenSpliceDDS](http://www.prismtech.com/opensplice/opensplice-dds-community/software-downloads). Make sure the download version of OpenSpliceDDS is 5.4.1-x86_64.linux2.6-gcc412-gnu25-HDE.tar.gz


Extract the downloaded tar file with following command.

	tar -xvf OpenSpliceDDSV5.4.1-x86_64.linux2.6-gcc412-gnuc25-HDE.tar.gz

In the extracted director, you can find HDE folder. Config release.com by replacing "@@INSTALLDIR@@" with the installed OpenSpliceDDS directory path.

Setup the environment by running the following command and also add the same to ~/.bash_profile

	source release.com

Start OpenSpliceDDS with the following command

	ospl start

Make sure, OpenSpliceDDS started successfully by running the given command

	ospl list
