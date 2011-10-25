#include <iostream>
#include <iostream>
#include <dds/dds.hpp>
#include "ccpp_tempmonitor.h"
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
void run(long time,short utemp) 
{
	DBClientConnection c;
	c.connect("localhost");
	BSONObj p = BSONObjBuilder().append("Time_Stamp",(int)time).append("Temperature",utemp).obj();
	c.insert("EMR.patients", p);
}
	REGISTER_TOPIC_TRAITS(com::netspective::medigy::TempMonitor)
int main(int argc, char* argv[]) 
{
	std::string partition = "temp";
	dds::Runtime runtime(partition);
	dds::Duration cleanup_delay = {3600, 0};
	dds::TopicQos tQos;
	tQos.set_persistent();
	tQos.set_reliable();
	tQos.set_keep_last(10);
	tQos.set_durability_service(cleanup_delay,DDS::KEEP_LAST_HISTORY_QOS,1024,8192,4196,8192);
	dds::Topic<com::netspective::medigy::TempMonitor> topic("TempMonitor", tQos);
	dds::DataReaderQos drQos(tQos);
	dds::DataReader<com::netspective::medigy::TempMonitor> dr(topic, drQos);
	com::netspective::medigy::TempMonitorSeq data;
	DDS::SampleInfoSeq info;
	dds::Duration timeout = {60, 0};
	dr.wait_for_historical_data(timeout);
	while (true) 
	{
		dr.take(data, info);
		for (uint32_t i = 0; i < data.length(); ++i) 
		{
			try 
			{
				run(data[i].timeOfMeasurement,data[i].temp);
				std::cout << "connected ok" << endl;
			} 
			catch( DBException &e ) 
			{
				std::cout << "caught " << e.what() << endl;}
		}
		dr.return_loan(data, info);
		sleep(1);
	}
	return 0;
}

