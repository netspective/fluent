# Syslog Installation

Syslog supported by a wide variety of devices and receivers across multiple platforms and can be used to integrate log data from many different types of systems into a central repository.

Check for existing syslog. If not present, follow the document and install syslog

	rpm -aq | grep syslog

For Ubuntu

	dpkg --get-selections | grep syslog

Download the syslog and eventlog rpm package, using following commands

	wget ftp://ftp.pbone.net/mirror/centos.karan.org/el5/extras/testing/x86_64/RPMS/syslog-ng-2.0.3-1.el5.kb.x86_64.rpm
    wget ftp://ftp.pbone.net/mirror/centos.karan.org/el5/extras/testing/x86_64/RPMS/eventlog-0.2.5-6.el5.kb.x86_64.rpm

Install syslog and eventlog rpm uisng the following command

	rpm -ivh eventlog-0.2.5-6.el5.kb.x86_64.rpm
    rpm -ivh syslog-ng-2.0.3-1.el5.kb.x86_64.rpm
	
For Ubuntu

	apt-get install syslog-ng


Update the configuration in syslog-ng.conf using the following command

	vim /etc/syslog-ng/syslog-ng.conf

    destination netspective { udp(<Syslog_IP> port(514)); };
    source s_all {internal(); 
    unix-stream("/dev/log");
    file("/proc/kmsg" log_prefix("kernel: "));};
    log { source(s_all); destination(netspective); };

Replace <Syslog_IP> with machine IP address.

Command used to start the syslog are given below

	/etc/init.d/syslog-ng start
