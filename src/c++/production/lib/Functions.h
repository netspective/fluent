#ifndef __FUNCTIONS_H_
#define __FUNCTIONS_H_

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>


using namespace std;

namespace po = boost::program_options;



bool parse_args_pub(int argc, char* argv[],string &hostip,string &domainid,string &deviceid,string &logfile,string &logcate,string &logcatedata,string &logconfpath);

bool parse_args_sub(int argc, char* argv[],string &domainid,string &deviceid,string &logfile,string &logcatedata);

string replaceconfstring(string logfile,string filename,string infostring,string datastring);
#endif


