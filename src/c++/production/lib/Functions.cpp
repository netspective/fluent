#include <stdio.h>
#include <string>
#include <iostream>
#include "Functions.h"
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
namespace po = boost::program_options;



bool parse_args_pub(int argc, char* argv[],string &hostip, string &domainid,string &deviceid,string &logfile,string &logcate,string &logcatedata,string &logconfpath)
{
  po::options_description desc("Available options for are");
  desc.add_options()
    ("help", "produce help message")
    ("data-gen-ip", po::value<std::string>(), "Data Generator IP ")
    ("domain", po::value<std::string>(), "Device Domain ")
    ("device-id",po::value<std::string>(), "Device ID - for device identification")
    ("log-path", po::value<std::string>(), "Log File Location")
    ("log-category", po::value<std::string>(), "Log category Specification")
    ("log-category-data", po::value<std::string>(), "Log category data Specification ")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
    ;
 
  try {
    po::variables_map vm;        
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    
   
    if (vm.count("help") || argc == 1) {
      std::cout << desc << "\n";
      return false;
    }
    
    if (vm.count("data-gen-ip")) 
      hostip = vm["data-gen-ip"].as<std::string>();
    if (vm.count("domain")) 
      domainid = vm["domain"].as<std::string>();
    if (vm.count("device-id")) 
      deviceid = vm["device-id"].as<std::string>();
    if (vm.count("log-path")) 
      logfile = vm["log-path"].as<std::string>();
    if (vm.count("log-category")) 
      logcate = vm["log-category"].as<std::string>();
    if (vm.count("log-category-data")) 
      logcatedata = vm["log-category-data"].as<std::string>();
    if (vm.count("log4cpp-conf")) 
      logconfpath = vm["log4cpp-conf"].as<std::string>();
    
  } 
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}

bool parse_args_sub(int argc, char* argv[],string &domainid,string &deviceid,string &logfile,string &logconfpath)
{
  po::options_description desc("Available options for are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("log-file", po::value<std::string>(), "Log File Location")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
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
      deviceid = vm["device-id"].as<std::string>();
    if (vm.count("log-file"))
       logfile = vm["log-file"].as<std::string>();
    if (vm.count("log4cpp-conf"))
      logconfpath = vm["log4cpp-conf"].as<std::string>();
	   

    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}

string replaceconfstring(string logfile,string filename,string infostring,string datastring)
 {
   string search_string = "INFOFILENAME";
   string search_stringdata = "DATAFILENAME";
   string inbuf;
   fstream stream(filename.c_str(),ios::in);
   ofstream fout(infostring.c_str());
   while(!stream.eof())
   {
       getline(stream, inbuf);
       int spot = inbuf.find(search_string);
       if(spot >= 0)
       {
          string tmpstring = inbuf.substr(0,spot);
          //tmpstring += infostring;
          tmpstring = tmpstring+logfile+"/"+infostring;
          tmpstring += inbuf.substr(spot+search_string.length(), inbuf.length());
          inbuf = tmpstring;
       }

	spot = inbuf.find(search_stringdata);
       if(spot >= 0)
       {
          string tmpstring = inbuf.substr(0,spot);
          //tmpstring += datastring;
          tmpstring = tmpstring+logfile+"/"+datastring;
          tmpstring += inbuf.substr(spot+search_string.length(), inbuf.length());
          inbuf = tmpstring;
       }
        fout<<inbuf<<endl;
   }
	  fout.close();
	return infostring;
 }
 
