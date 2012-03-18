# Fluent Compilation and Configuration Steps

Set OpenSliceDDS environment variables from the Opensplice installed directory as shown below.

	source /../../HDE/x86.linux2.6/release.com

Download the netspective-fluent from the Netspective github.

	wget --no-check-certificate https://github.com/netspective/fluent/zipball/master
    mv master fluent.zip && unzip fluent.zip

Run the following command to clean the build files.

	cd <netspective-fluent>/support/build

Edit the Makefile and replace @@topdir-netspective-fluent@@ with netspective-fluent parent directory

Run the following command to clean the build files.

	make clean

Run the makefile

	make

After successful completion of compilation, binary files will be created in following directory.

	cd ../../bin/

Now, open <netspective-fluent>/src/web/config.php and change the following configuration properties with appropriate values

	<Graylog-Server-IP>
	<Apache-Server-IP>
	<WebSocket-IP>:<WebSocket-Port>
	<Fluent-Domain>

## Troubleshooting

error: .INT32_MIN. or .INT32_MAX. was not declared in this scope

	Replace INT32_MIN with 0 and INT32_MAX with 2147483647
