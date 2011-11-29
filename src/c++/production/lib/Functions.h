#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
/*String literals*/

#define DOT "."
#define COMMA ", "
#define CLEAN ""
#define SEMI ":"
#define BUFFERSIZE 1024
#define TIMESTAMP "timeStamp"
#define DEVICEID "deviceID"
#define SYSTOLIC "systolic"
#define DIASTOLIC "diastolic"
#define PULSERATE "pulseRate"
#define SPo2 "SPO2"
#define TEMPERATURE "temperature"





using namespace std;

namespace po = boost::program_options;



bool parse_args_pub(int argc, char* argv[],string &hostip,int &port,string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath);

bool parse_args_sub(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logcatedata);

bool parse_args_bp_alarm(int argc, char* argv[],string &domainid,string &deviceid,int &sysmin,int &sysmax,int &dismin,int &dismax,int &pulsemin,int &pulsemax,string &loginfo,string &logdata,string &logconfpath);

bool parse_args_pulse_alarm(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath,int &splow,int &sphigh,int &templow,int &temphigh);

bool parse_args_temp_alarm(int argc,char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath,int &avgtime,int &templow,int &temphigh);

bool parse_args_sub_persist(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath, string &host, string &database);


string alarmString(int value,int min,int max);
#endif


