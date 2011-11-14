#include <iostream>
#include "SimpleDDS.h"
#include <dds/dds.hpp>
#include "ccpp_tempmonitor.h"
// -- BOOST Include
#include <boost/program_options.hpp>
// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
using namespace DDS;
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
std::stringstream temp;
std::string domainid,deviceid;
int templow,temphigh,avgtime;
bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <Temperature Monitor> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("avg-time-period",po::value<int>(), "Average time period for tempetature - default 1 min")
    ("temp-low", po::value<int>(), "Temperature Low level Alarm Alarm Specification - default <88")
    ("temp-high", po::value<int>(), "Temperature High Level Alarm Specification - default >92")
    ;

  try {
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }
    
    if (vm.count("domain"))
      domainid = vm["domain"].as<std::string>();
	
    if (vm.count("device-id"))
    {
      deviceid = vm["device-id"].as<std::string>();
      //string key ("{");
      //size_t start,end;
      //string key1 ("}");
      //start=deviceid.rfind(key);
      //end=deviceid.rfind(key1);
      //deviceid = deviceid.substr(0,start)+deviceid.substr(start+1,end-start-1);
      //cout<<"\n"<<deviceid;
    }
    
    if (vm.count("avg-time-period"))
      avgtime = vm["avg-time-period"].as<int>();
    if (vm.count("temp-low"))
      templow = vm["temp-low"].as<int>();
    if (vm.count("temp-high"))
      temphigh = vm["temp-high"].as<int>();
	
    
    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
} 


int main(int argc, char* argv[]) 
{
	long timestart,timeend=0,avg=0;
	int nrecord=0;
	avgtime=1;
	templow=88;
	temphigh=92;
	if (!parse_args(argc, argv))
    	return 1;

	 SimpleDDS *simpledds;
	 TempMonitorTypeSupport_var typesupport;
    	 DataReader_ptr reader;
    	 TempMonitorDataReader_var bpReader;
    	 ReturnCode_t status;
	 int i=0;
         DDS::TopicQos tQos;
         tQos.durability.kind=VOLATILE_DURABILITY_QOS;
         tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
         tQos.history.depth=10;
         tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
         tQos.durability_service.history_depth= 1024;
         simpledds = new SimpleDDS(tQos);
	 typesupport = new TempMonitorTypeSupport();
    	 reader = simpledds->subscribe(typesupport);
    	 bpReader = TempMonitorDataReader::_narrow(reader);
   	 TempMonitorSeq  bpList;
     	 SampleInfoSeq     infoSeq;
	 while (1) 
	 {
         	status = bpReader->take(
            	bpList,
            	infoSeq,
            	LENGTH_UNLIMITED,
            	ANY_SAMPLE_STATE,
           	ANY_VIEW_STATE,
            	ANY_INSTANCE_STATE);
         	checkStatus(status, "take");
          	if (status == RETCODE_NO_DATA) 
		{
          		continue;
          	}
          	for (i = 0; i < bpList.length(); i++) 
	  	{
			temp << bpList[i].deviceID;
			if(strcmp(temp.str().c_str() , deviceid.c_str() ) == 0 )
			{
	
				avg+=(long)bpList[i].temp;
				if(nrecord == 0)
	                        {
	                                timestart=(long)bpList[i].timeOfMeasurement;
	                        }
	                        else
	                        {
					timeend =(long) bpList[i].timeOfMeasurement;
	                                if((timeend - timestart) > (60*avgtime))
	                                {
						avg=avg/nrecord;
						if(avg < templow || avg > temphigh)
						{
		                                       	cout<<"\nStart Time : "<<timestart<<" End Time : "<<timeend;
							cout<<"\nTemperature : "<<avg;
						}
						nrecord = -1;
	                               }
	                         }
				status = bpReader->return_loan(bpList, infoSeq);
        			checkStatus(status, "return_loan");
			}
			sleep(1);
		}
		nrecord++;
		
    	}
        /* We're done.  Delete everything */
        simpledds->deleteReader(reader);
        delete simpledds;
        return 0;
}
