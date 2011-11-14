#include "SimpleDDS.h"
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_bp.h"

// -- BOOST Include
#include <boost/program_options.hpp>

// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>
using namespace std;
namespace po = boost::program_options;
using namespace com::netspective::medigy;
std::stringstream temp;
int sysmin,sysmax,pulsemin,pulsemax,dismin,dismax;
std::string domainid,deviceid;

REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

bool parse_args(int argc, char* argv[])
{
  po::options_description desc("Available options for <bloodPressure-alarm> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("systolic-low", po::value<int>(), "Systolic Low Pressure Alarm Specification - default <90")
    ("systolic-high", po::value<int>(), "Systolic High Pressure Alarm Specification - default >140")
    ("diatolic-low", po::value<int>(), "Diatolic Low Pressure Alarm Specification - default <60")
    ("diatolic-high", po::value<int>(), "Diatolic High Pressure Alarm Specification - default >90")
    ("pulse-rate-low", po::value<int>(), "Pulse Low Rate Alarm Specification - default <60")
    ("pulse-rate-high", po::value<int>(), "Pulse High Rate Alarm Specification - default >90")
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

  
    if (vm.count("systolic-low"))
      sysmin = vm["systolic-low"].as<int>();
    if (vm.count("systolic-high"))
      sysmax = vm["systolic-high"].as<int>();
	
    if (vm.count("diatolic-low"))
      dismin = vm["diatolic-low"].as<int>();
    if (vm.count("diatolic-high"))
      dismin = vm["diatolic-high"].as<int>();

    if (vm.count("pulse-rate-low"))
      pulsemin = vm["pulse-rate-low"].as<int>();
    if (vm.count("pulse-rate-high"))
      pulsemax = vm["pulse-rate-high"].as<int>();

    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}  

int main(int argc, char* argv[]) 
{
	sysmin = 90;
	sysmax = 140;
	dismin = 60;
	dismax = 90;
	pulsemin = 60;
	pulsemax = 90;
	if (!parse_args(argc, argv))
    	return 1;
	SimpleDDS *simpledds;
	 BloodPressureTypeSupport_var typesupport;
    	 DataReader_ptr reader;
    	 BloodPressureDataReader_var bpReader;
    	 ReturnCode_t status;
	  int i=0;
         //simpledds = new SimpleDDS();
         DDS::TopicQos tQos;
         tQos.durability.kind=VOLATILE_DURABILITY_QOS;
         tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
         tQos.history.depth=10;
         tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
         tQos.durability_service.history_depth= 1024;
         simpledds = new SimpleDDS(tQos);
	 typesupport = new BloodPressureTypeSupport();
    	 reader = simpledds->subscribe(typesupport);
    	 bpReader = BloodPressureDataReader::_narrow(reader);

    	 /* Read blood pressure values */
    	 BloodPressureSeq  bpList;
     	 SampleInfoSeq     infoSeq;
    	 cout<<"Waiting to read bloodPressure data...\n";

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
			if (bpList[i].systolicPressure < sysmin || bpList[i].systolicPressure > sysmax || bpList[i].diastolicPressure < dismin || bpList[i].diastolicPressure > dismax || bpList[i].pulseRatePerMinute < pulsemin || bpList[i].pulseRatePerMinute > pulsemax)
			{
				cout<<"\nTime : "<<bpList[i].timeOfMeasurement;
				cout<<"\nPatient Blood Pressure alarm ";
				cout<<"\nSystolic: "<< bpList[i].systolicPressure;
				cout<<"\nDiastolic: "<<bpList[i].diastolicPressure;
				cout<<"\nPULSE: "<<bpList[i].pulseRatePerMinute;
			}
			status = bpReader->return_loan(bpList, infoSeq);
        		checkStatus(status, "return_loan");
			}

			temp.str("");
			
		}
	 	
    	}

        /* We're done.  Delete everything */
        simpledds->deleteReader(reader);
        delete simpledds;
        return 0;
}
