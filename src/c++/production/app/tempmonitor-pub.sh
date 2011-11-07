#IF NO ARGUMENTS WERE PROVIDED
function USAGE ()
{
    echo ""
    echo "USAGE: "
    echo "    Temperature Monitor Publisher"
    echo ""
    echo "OPTIONS:"
    echo "    --domain  Device Domain"
    echo "    --device-id  Device ID to identify the particular device"
    echo "    --spawn  Number of publishers to run"
    echo "    --log-file  Log file path"
    echo ""
    echo "EXAMPLE:"
    echo "    tempmonitor-pub.sh --domain=tempmonitor --device-id=A10 --spawn=100 --log-file=/var/log/netspective/"
    echo ""
    exit $E_OPTERROR    # Exit and explain usage, if no argument(s) given.
}
#USAGE
#PROCESS ARGS
for i in $*
do
	case $i in
    	--domain=*)
		domain=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
    	--device-id=*)
		deviceid=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
    	--spawn=*)
		spawn=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
		;;
	--log-file=*)
                logfile=`echo $i | sed 's/[-a-zA-Z0-9]*=//'`
                ;;
    	--default)
		USAGE
		DEFAULT=YES
		;;
  	esac
done

if [ $# -eq 0 ] ; then
    USAGE
fi

rm -f /var/log/netspective/*
for (( j = 1 ; j <= $spawn; j++ ))
do
	echo "./tempmonitor-pub --domain $domain --device-id $deviceid{$j} --log-file $logfile/tempmonitor-pub-deviceid{$j}.log"
	./tempmonitor-pub --domain $domain --device-id $deviceid{$j} --log-file /var/log/netspective/tempmonitor-pub-$deviceid{$j}.log  > /dev/null &
	sleep 5
done
