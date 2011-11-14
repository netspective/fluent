#ifndef __SIMPLE_DDS_H_
#define __SIMPLE_DDS_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>

#include "ccpp_dds_dcps.h"

using namespace DDS;
using namespace std;


/* Check that the given handle is not null.
 * If it is, print an error and exit.
 */
void checkHandle(void* handle, const char *errormsg);

/* Check that the status is ok.
 * If not, print the given error message and exit.
 */
void checkStatus(ReturnCode_t status, const char *errormsg);


/** TopicData is used to store all the objects needed to read/write a topic.
 *  For publishers this includes:
 *  - The Topic
 *  - The Publisher and PublisherQos
 *  - The DataWriter and DataWriterQos
 * 
 *  For subscribers this includes:
 *   ???
 * 
 */  
class TopicData {
  public:
    Topic_var      topic;
    const char*    typeName;
    char*          topicName;

    Publisher_var   publisher;
    PublisherQos    pubQos;
    DataWriter_ptr  writer;
    DataWriterQos   writerQos;

    Subscriber_var  subscriber;
    SubscriberQos   subQos;
    DataReader_ptr  reader;
    DataReaderQos   readerQos;

    TopicData();
    ~TopicData();
};



/** Simple DDS is used to create publishers and subscribers quickly,
 *  using default settings.  The typical usage will be:
 *
 *  SimpleDDS *simpledds = new SimpleDDS();
 *
 *  DataWriter_ptr writer = simpledds->publish(ChatMessageTypeSupport_var);
 *  ChatMessageDataWriter_var chatWriter = ChatMessageDataWriter::_narrow(writer);
 *  chatWriter->write(chat, NULL);
 *
 *  DataReader_ptr reader = simpledds->subscribe(ChatMessageTypeSupport_var);
 *  ChatMessageDataReader_var chatReader = ChatMessageDataReader::_narrow(reader);
 *  chatReader->take(...);
 */

class SimpleDDS {
  public:
    DomainParticipantFactory_var  dpf;
    DomainParticipant_var participant;
    vector<TopicData*> *topics;
    TopicQos reliable_topic_qos;
    TopicQos transient_topic_qos;

    void InitQos();
    char* TypeNameToTopic(const char *typeName);

    /* Create a new DDS domain */
    SimpleDDS(TopicQos);
    SimpleDDS();

    /* Delete the DDS domain */
    ~SimpleDDS();

    /* Create a 'publisher' for a Type (like BloodPressureType) */
    DataWriter_ptr publish(TypeSupport *support);

    /* Create a 'subscriber' for a Type (like BloodPressureType) */
    DataReader_ptr subscribe(TypeSupport *support);

    /* Delete the writer used for publishing */
    void deleteWriter(DataWriter_ptr writer);

    /* Delete the reader used for subscribing */
    void deleteReader(DataReader_ptr reader);

};

#endif


