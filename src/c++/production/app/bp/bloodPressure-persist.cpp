#include <iostream>
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_bp.h"

//Visolve Added
#include <mongo/mongo/client/dbclient.h>


// -- BOOST Include
#include <boost/program_options.hpp>

// -- DDS Include
#include <dds/runtime.hpp>
#include <dds/topic.hpp>
#include <dds/reader.hpp>
#include <dds/traits.hpp>

using namespace mongo;


std::stringstream temp;

void run(long time,short bp,short lbp,short pr) 
{
  DBClientConnection c;
  c.connect("localhost");
  BSONObj p = BSONObjBuilder().append("Time_Stamp",0).append("Systolic_Pressure",bp).append("Diastolic_Pressure",lbp).append("Pulse Rate",pr).obj();
  c.insert("EMR.patients", p);
 
}


REGISTER_TOPIC_TRAITS(com::netspective::medigy::BloodPressure)

int main(int argc, char* argv[]) {
  

  std::string partition = "blood";

  dds::Runtime runtime(partition);
  dds::Duration cleanup_delay = {3600, 0};

  dds::TopicQos tQos;
  tQos.set_persistent();
  tQos.set_reliable();
  tQos.set_keep_last(10);
  tQos.set_durability_service(cleanup_delay,
			      DDS::KEEP_LAST_HISTORY_QOS,
			      1024,
			      8192,
			      4196,
			      8192);

  dds::Topic<com::netspective::medigy::BloodPressure> topic("BloodPressure", tQos);


  dds::DataReaderQos drQos(tQos);
  dds::DataReader<com::netspective::medigy::BloodPressure> dr(topic, drQos);

  com::netspective::medigy::BloodPressureSeq data;
  DDS::SampleInfoSeq info;

  dds::Duration timeout = {60, 0};
  dr.wait_for_historical_data(timeout);

  while (true) {
    dr.take(data, info);
    for (uint32_t i = 0; i < data.length(); ++i) 
	{
	try {
		
    		run(data[i].timeOfMeasurement,data[i].systolicPressure,data[i].diastolicPressure,data[i].pulseRatePerMinute);
		std::cout << data[i].timeOfMeasurement<<"\n";
		
    		std::cout << "connected ok" << endl;
  		} catch( DBException &e ) {
    			std::cout << "caught " << e.what() << endl;}
    }
    dr.return_loan(data, info);
    sleep(1);
  }
  return 0;
}

