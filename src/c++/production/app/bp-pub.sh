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
    echo "    --log4cpp-conf  configration file path"
    echo ""
    echo "EXAMPLE:"
    echo "  ./bp-pub.sh --domain=blood --device-id=BP --spawn=5 --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf"
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
	--log4cpp-conf=*)
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

rm -f /var/log/netspective/blood/*
for (( j = 1 ; j <= $spawn; j++ ))
do


echo "./bp-pub --data-gen-ip 127.0.0.1 --domain $domain --device-id $deviceid{$j} --log-info blood.info --log-data blood.data  --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf > /dev/null &"
./bp-pub --data-gen-ip 127.0.0.1 --domain $domain --device-id $deviceid{$j} --log-info blood.info --log-data blood.data  --log4cpp-conf ../src/c++/production/conf/simulation_log_bp.conf  > /dev/null &
	
	sleep 5

done
