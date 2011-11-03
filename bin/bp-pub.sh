#IF NO ARGUMENTS WERE PROVIDED
function USAGE ()
{
    echo ""
    echo "USAGE: "
    echo "    BloodPressure Publisher"
    echo ""
    echo "OPTIONS:"
    echo "    --domain  Device Domain"
    echo "    --device-id  Device ID to identify the particular device"
    echo "    --spawn  Number of publishers to run"
    echo "    --log-file  Log file path"
    echo ""
    echo "EXAMPLE:"
    echo "    bp-pub.sh --device=blood --device-id=A10 --spawn=100 --log-file=/usr/local/dds.log"
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
#    	*)
#		USAGE
                # unknown option
#		;;
  	esac
done

if [ $# -eq 0 ] ; then
    USAGE
fi

rm -f /var/log/netspective/*
for (( j = 1 ; j <= $spawn; j++ ))
do
	echo "./bp-pub --domain $domain --device-id $deviceid{$j} --log-file $logfile/bp-pub-deviceid{$j}.log"
	./bp-pub --domain $domain --device-id $deviceid{$j} --log-file /var/log/netspective/bp-pub-$deviceid{$j}.log > /var/log/netspective/bp-pub-$deviceid{$j}.log &
done
