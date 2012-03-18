# Apache Configuration

In CentOS, Set VirtualHost in /etc/httpd/conf/httpd.conf.

	vi /etc/httpd/conf/httpd.conf

In Ubuntu, Set VirtualHost in /etc/apache2/sites-available/default

	vi /etc/apache2/sites-available/default

Copy and Paste following lines in any of the above mentioned files based on respective operating systems.

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

Start Apache webserver

	/etc/init.d/httpd start

Stop Apache webserver

	/etc/init.d/httpd stop
