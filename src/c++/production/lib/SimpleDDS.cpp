#include <stdio.h>
#include <iostream>
#include <string>
#include "SimpleDDS.h"
#include <sstream>
using namespace DDS;
using namespace std;

void checkHandle(void* handle, const char *errormsg) {
    if (handle == NULL) {
        fprintf(stderr, "Error. Invalid handle. %s\n", errormsg);
        exit(0);
    }
}

void checkStatus(ReturnCode_t status, const char* errormsg) {
    if (status != DDS::RETCODE_OK && status != DDS::RETCODE_NO_DATA) {
        fprintf(stderr, "Error, bad status %d, %s\n", status, errormsg);
        exit(0);
    }
}


TopicData::TopicData() {
    topic = NULL;
    publisher = NULL;
    writer = NULL;
    typeName = NULL;
}

TopicData::~TopicData() {
}



SimpleDDS::SimpleDDS() {
    DomainId_t domain = NULL;
    topics = new vector<TopicData*>(10);
    dpf = DomainParticipantFactory::get_instance();
    checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT, 
                                          NULL, STATUS_MASK_NONE);
    checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    InitQos();
}


SimpleDDS::SimpleDDS(TopicQos reliable_topicqos) {
    DomainId_t domain = NULL;
    topics = new vector<TopicData*>(10);
    dpf = DomainParticipantFactory::get_instance();
    checkHandle(dpf.in(), "DomainParticipantFactory::get_instance");
    participant = dpf->create_participant(domain, PARTICIPANT_QOS_DEFAULT,
                                          NULL, STATUS_MASK_NONE);
    checkHandle(participant.in(), "You need to run 'ospl start' to start OpenSpliceDDS");
    //InitQos();

	ReturnCode_t status;
    status = participant->get_default_topic_qos(reliable_topicqos);
	memcpy(&reliable_topic_qos,&reliable_topicqos, sizeof(reliable_topicqos));

}

void getQos(TopicQos &tQos)
{
	tQos.durability.kind=VOLATILE_DURABILITY_QOS;
        tQos.reliability.kind=BEST_EFFORT_RELIABILITY_QOS;
        tQos.history.depth=10;
        tQos.durability_service.history_kind = KEEP_LAST_HISTORY_QOS;
        tQos.durability_service.history_depth= 1024;
}

/* Create the reliable_qos and transient_qos */
void SimpleDDS::InitQos() {
    ReturnCode_t status;
    status = participant->get_default_topic_qos(reliable_topic_qos);
    //checkStatus(status, "DDS::DomainParticipant::get_default_topic_qos");
    //reliable_topic_qos.reliability.kind = RELIABLE_RELIABILITY_QOS;
    //status = participant->set_default_topic_qos(reliable_topic_qos);
    //checkStatus(status, "DDS::DomainParticipant::set_default_topic_qos");

    //status = participant->get_default_topic_qos(transient_topic_qos);
    //checkStatus(status, "DDS::DomainParticipant::get_default_topic_qos");
    //transient_topic_qos.durability.kind = TRANSIENT_DURABILITY_QOS;

}

/* Convert a typeName like "com::netspective::medigy::MyType" to 
 * a topic name like "MyType"
 */
char* SimpleDDS::TypeNameToTopic(const char *typeName) {
    const char *topicName = strrchr(typeName, ':');
    if (topicName == NULL) {
        topicName = typeName;
    }
    else {
        topicName++;
    }
    return strdup(topicName);
}

DataWriter_ptr SimpleDDS::publish(TypeSupport *typesupport) {
    ReturnCode_t status;
    checkHandle(typesupport, "publish");
    TopicData *topic = new TopicData();
    topics->push_back(topic);
    topic->typeName = typesupport->get_type_name();
    status = typesupport->register_type(participant.in(), topic->typeName);
    checkStatus(status, "register_type");
    topic->topicName = TypeNameToTopic(topic->typeName);

    topic->topic = participant->create_topic(
        topic->topicName,
        topic->typeName, 
        reliable_topic_qos, 
        NULL,                  // No TopicListener is needed
        STATUS_MASK_NONE);

    checkHandle(topic->topic.in(), "create_topic");

    status = participant->get_default_publisher_qos(topic->pubQos);
    checkStatus(status, "get_default_publisher_qos");
    topic->publisher = participant->create_publisher(topic->pubQos, NULL, STATUS_MASK_NONE);
    checkHandle(topic->publisher.in(), "create_publisher");

    topic->writer = topic->publisher->create_datawriter(
        topic->topic.in(), 
        DATAWRITER_QOS_USE_TOPIC_QOS,  
        NULL, 
        STATUS_MASK_NONE);

    checkHandle(topic->writer, "create_datawriter");
    return topic->writer;
}


void SimpleDDS::deleteWriter(DataWriter_ptr writer) {
    ReturnCode_t status;
    int i;
    TopicData *topic;

    /* Find the TopicData for this writer */
    for (i = 0; i < topics->size(); i++) {
        topic = topics->at(i);
        if (topic->writer == writer) {
            break;
        }
    }

    if (topic->writer != writer) {
        fprintf(stderr, "Error: deleteWriter: bad writer\n");
        exit(0);
    }

    /* Remove the TopicData from the topics vector */
    topics->erase(topics->begin() + i);

    status = topic->publisher->delete_datawriter(writer);
    checkStatus(status, "delete_datawriter");

    status = participant->delete_publisher(topic->publisher.in() );
    checkStatus(status, "delete_publisher");

    status = participant->delete_topic(topic->topic.in() );
    checkStatus(status, "delete_topic");

    delete topic; 
}


DataReader_ptr SimpleDDS::subscribe(TypeSupport *typesupport) {
    ReturnCode_t status;
    checkHandle(typesupport, "publish");
    TopicData *topic = new TopicData();
    topics->push_back(topic);
    topic->typeName = typesupport->get_type_name();
    status = typesupport->register_type(participant.in(), topic->typeName);
    checkStatus(status, "register_type");
    topic->topicName = TypeNameToTopic(topic->typeName);

    topic->topic = participant->create_topic(
        topic->topicName,
        topic->typeName, 
        reliable_topic_qos, 
        NULL,                  // No TopicListener is needed
        STATUS_MASK_NONE);

    checkHandle(topic->topic.in(), "create_topic");

    status = participant->get_default_subscriber_qos(topic->subQos);
    checkStatus(status, "get_default_subscriber_qos");
    topic->subscriber = participant->create_subscriber(topic->subQos, NULL, STATUS_MASK_NONE);
    checkHandle(topic->subscriber.in(), "create_subscriber");

    topic->reader = topic->subscriber->create_datareader(
        topic->topic.in(), 
        DATAREADER_QOS_USE_TOPIC_QOS,  
        (DataReaderListener*) NULL, 
        STATUS_MASK_NONE);

    checkHandle(topic->reader, "create_datareader");
    return topic->reader;
}
DataReader_ptr SimpleDDS::filteredSubscribe(TypeSupport *typesupport, string fTopic, string fieldName, string filterContent,const StringSeq &expr)
{
    ReturnCode_t status;
    checkHandle(typesupport, "publish");
    TopicData *topic = new TopicData();
    topics->push_back(topic);
    topic->typeName = typesupport->get_type_name();
    status = typesupport->register_type(participant.in(), topic->typeName);
    checkStatus(status, "register_type");
    topic->topicName = TypeNameToTopic(topic->typeName);

    topic->topic = participant->create_topic(
        topic->topicName,
        topic->typeName,
        reliable_topic_qos,
        NULL,                  // No TopicListener is needed
        STATUS_MASK_NONE);

    checkHandle(topic->topic.in(), "create_topic");

    status = participant->get_default_subscriber_qos(topic->subQos);
    checkStatus(status, "get_default_subscriber_qos");


    topic->subscriber = participant->create_subscriber(topic->subQos, NULL, STATUS_MASK_NONE);
    checkHandle(topic->subscriber.in(), "create_subscriber");

    ostringstream buf;
    buf << fieldName <<" = '" << filterContent << "'";
    CORBA::String_var sFilter = CORBA::string_dup(buf.str().c_str());
    filteredTopic = participant->create_contentfilteredtopic(fTopic.c_str(),topic->topic, sFilter.in(), expr);
    topic->reader = topic->subscriber->create_datareader(filteredTopic.in(),DATAREADER_QOS_USE_TOPIC_QOS, NULL, STATUS_MASK_NONE);

    checkHandle(topic->reader, "create_datareader");
    return topic->reader;
}


void SimpleDDS::deleteReader(DataReader_ptr reader) {
    ReturnCode_t status;
    int i;
    TopicData *topic;

    /* Find the TopicData for this reader */
    for (i = 0; i < topics->size(); i++) {
        topic = topics->at(i);
        if (topic->reader == reader) {
            break;
        }
    }

    if (topic->reader != reader) {
        fprintf(stderr, "Error: deleteReader: bad reader\n");
        exit(0);
    }

    /* Remove the TopicData from the topics vector */
    topics->erase(topics->begin() + i);

    status = topic->subscriber->delete_datareader(reader);
    checkStatus(status, "delete_datareader");

    status = participant->delete_subscriber(topic->subscriber.in() );
    checkStatus(status, "delete_subscriber");

    status = participant->delete_topic(topic->topic.in() );
    checkStatus(status, "delete_topic");

    delete topic; 
}


SimpleDDS::~SimpleDDS() {
    ReturnCode_t status;

    status = dpf->delete_participant( participant.in() );
    checkStatus(status, "delete_participant");
    delete topics;
}


