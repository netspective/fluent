#include <stdio.h>
#include <string>
#include <iostream>
#include "Functions.h"
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
namespace po = boost::program_options;



bool parse_args_pub(int argc, char* argv[],string &hostip,int &port, string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath)
{
  po::options_description desc("Available options for are");
  desc.add_options()
    ("help", "produce help message")
    ("data-gen-ip", po::value<std::string>(), "Data Generator IP ")
    ("data-gen-port", po::value<int>(), "Data Generator Port ")
    ("domain", po::value<std::string>(), "Device Domain ")
    ("device-id",po::value<std::string>(), "Device ID - for device identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log Data Specification ")
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
    if (vm.count("data-gen-port")) 
      port = vm["data-gen-port"].as<int>();
    if (vm.count("domain")) 
      domainid = vm["domain"].as<std::string>();
    if (vm.count("device-id")) 
      deviceid = vm["device-id"].as<std::string>();
    if (vm.count("log-info")) 
      loginfo = vm["log-info"].as<std::string>();
    if (vm.count("log-data")) 
      logdata = vm["log-data"].as<std::string>();
    if (vm.count("log4cpp-conf")) 
      logconfpath = vm["log4cpp-conf"].as<std::string>();
    
  } 
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}

bool parse_args_sub(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath)
{
  po::options_description desc("Available options for are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log data Specification ")
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
    if (vm.count("log-info"))
      loginfo = vm["log-info"].as<std::string>();
    if (vm.count("log-data"))
      logdata = vm["log-data"].as<std::string>();
    if (vm.count("log4cpp-conf"))
      logconfpath = vm["log4cpp-conf"].as<std::string>();
	   

    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}

bool parse_args_sub_persist(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath, string &host, string &database) {

  po::options_description desc("Available options for are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log data Specification ")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
    ("host", po::value<std::string>(), "Database Host")
    ("database", po::value<std::string>(), "Database Name")
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
	if (vm.count("log-info"))
		loginfo = vm["log-info"].as<std::string>();
	if (vm.count("log-data"))
		logdata = vm["log-data"].as<std::string>();
	if (vm.count("log4cpp-conf"))
		logconfpath = vm["log4cpp-conf"].as<std::string>();
	if (vm.count("host"))
		host = vm["host"].as<std::string>();
	if (vm.count("database"))
		database = vm["database"].as<std::string>();


  }
  catch (...) {
    std::cout << desc << "\n";
    return false;
 }
  return true;

}




bool parse_args_bp_alarm(int argc, char* argv[],string &domainid,string &deviceid,int &sysmin,int &sysmax,int &dismin,int &dismax,int &pulsemin,int &pulsemax,string &loginfo,string &logdata,string &logconfpath)
{
  po::options_description desc("Available options for <bloodPressure-alarm> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log data Specification ")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
    ("systolic-low", po::value<int>(), "Systolic Low Pressure Alarm Specification - default <90")
    ("systolic-high", po::value<int>(), "Systolic High Pressure Alarm Specification - default >140")
    ("diastolic-low", po::value<int>(), "Diatolic Low Pressure Alarm Specification - default <60")
    ("diastolic-high", po::value<int>(), "Diatolic High Pressure Alarm Specification - default >90")
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
       deviceid = vm["device-id"].as<std::string>();
    if (vm.count("log-info"))
      loginfo = vm["log-info"].as<std::string>();
    if (vm.count("log-data"))
      logdata = vm["log-data"].as<std::string>();
    if (vm.count("log4cpp-conf"))
      logconfpath = vm["log4cpp-conf"].as<std::string>();
    if (vm.count("systolic-low"))
      sysmin = vm["systolic-low"].as<int>();
    if (vm.count("systolic-high"))
      sysmax = vm["systolic-high"].as<int>();
    if (vm.count("diastolic-low"))
      dismin = vm["diastolic-low"].as<int>();
    if (vm.count("diastolic-high"))
      dismin = vm["diastolic-high"].as<int>();
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

bool parse_args_pulse_alarm(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath,int &splow,int &sphigh,int &pulselow,int &pulsehigh)
{
  po::options_description desc("Available options for <pulseoximeter-alarm> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log data Specification ")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
    ("spo2-low", po::value<int>(), "SPO2 Low level Alarm Alarm Specification - default <88")
    ("spo2-high", po::value<int>(), "SPO2 High Level Alarm Specification - default >92")
    ("pulse-low", po::value<int>(), "Temperature Low level Alarm Alarm Specification - default <90")
    ("pulse-high", po::value<int>(), "Temperature High Level Alarm Specification - default >105")
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
    if (vm.count("log-info")) 
      loginfo = vm["log-info"].as<std::string>();
    if (vm.count("log-data")) 
      logdata = vm["log-data"].as<std::string>();
    if (vm.count("log4cpp-conf")) 
      logconfpath = vm["log4cpp-conf"].as<std::string>();
    if (vm.count("spo2-low"))
      splow = vm["spo2-low"].as<int>();
    if (vm.count("spo2-high"))
      sphigh = vm["spo2-high"].as<int>();
    if (vm.count("pulse-low"))
      pulselow = vm["pulse-low"].as<int>();
    if (vm.count("pulse-high"))
      pulsehigh = vm["pulse-high"].as<int>();
	
    
    }
  
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
} 

bool parse_args_temp_alarm(int argc, char* argv[],string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath,int &avgtime,int &templow,int &temphigh)
{
  po::options_description desc("Available options for <Temperature Monitor> are");
  desc.add_options()
    ("help", "produce help message")
    ("domain", po::value<std::string>(), "Device Domain")
    ("device-id",po::value<std::string>(), "Device ID for identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log data Specification ")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
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
      deviceid = vm["device-id"].as<std::string>();
    if (vm.count("log-info")) 
      loginfo = vm["log-info"].as<std::string>();
    if (vm.count("log-data")) 
      logdata = vm["log-data"].as<std::string>();
    if (vm.count("log4cpp-conf")) 
      logconfpath = vm["log4cpp-conf"].as<std::string>();
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


string alarmString(int value,int min,int max)
{
	stringstream temp;
	temp<<value;
	if(value < min)
	temp<<" (LOW)";
	else if(value > max)
	temp<<" (HIGH)";
	else
	temp<<" (NORMAL)";
	return temp.str();
}



bool parse_args_pub_ecg(int argc, char* argv[],string &hostip,int &port, string &domainid,string &deviceid,string &loginfo,string &logdata,string &logconfpath,int &heartbeats,int &ecgsample,int &internalsample,
float &amplitudenoise,float &heart_rate_mean,float &heart_rate_std,float &lowfreq,float &highfreq,float &lowfreqstd,float &highfreqstd,float &lfhfradio)
{
  po::options_description desc("Available options for are");
  desc.add_options()
    ("help", "produce help message")
    ("data-gen-ip", po::value<std::string>(), "Data Generator IP ")
    ("data-gen-port", po::value<int>(), "Data Generator Port ")
    ("domain", po::value<std::string>(), "Device Domain ")
    ("device-id",po::value<std::string>(), "Device ID - for device identification")
    ("log-info", po::value<std::string>(), "Log Info Specification")
    ("log-data", po::value<std::string>(), "Log Data Specification ")
    ("log4cpp-conf", po::value<std::string>(), "Log Configration and Format specification file")
    ("approx-heart-beats", po::value<int>(), "Approximate number of heart beats default - 256")
    ("ecg-sample-freq", po::value<int>(), "ECG sampling frequency Hertz default - 256")
    ("internal-sample-freq", po::value<int>(), "Internal sampling frequency Hertz default - 256")
    ("amplitude-noise", po::value<float>(), "Amplitude of additive uniformly distributed noise: mV default - 0.0")
    ("heart-beat-mean", po::value<float>(), "Heart rate mean: beats per minute default - 60.0")
    ("heart-rate-std", po::value<float>(), "Heart rate std: beats per minute default - 1.0")
    ("low-freq", po::value<float>(), "Low frequency:  Hertz default - 0.1")
    ("high-freq", po::value<float>(), "High frequency std: Hertz default - 0.25")
    ("low-freq-std", po::value<float>(), "Low frequency std: Hertz default - 0.01")
    ("high-freq-std", po::value<float>(), "High frequency: Hertz default - 0.01")
    ("lf-hf-radio", po::value<float>(), "LF/HF ratio default - 0.5")
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
    if (vm.count("data-gen-port")) 
      port = vm["data-gen-port"].as<int>();
    if (vm.count("domain")) 
      domainid = vm["domain"].as<std::string>();
    if (vm.count("device-id")) 
      deviceid = vm["device-id"].as<std::string>();
    if (vm.count("log-info")) 
      loginfo = vm["log-info"].as<std::string>();
    if (vm.count("log-data")) 
      logdata = vm["log-data"].as<std::string>();
    if (vm.count("log4cpp-conf")) 
      logconfpath = vm["log4cpp-conf"].as<std::string>();
    if (vm.count("approx-heart-beats")) 
      heartbeats = vm["approx-heart-beats"].as<int>();
    if (vm.count("ecg-sample-freq")) 
      ecgsample = vm["ecg-sample-freq"].as<int>();
    if (vm.count("internal-sample-freq")) 
      internalsample = vm["internal-sample-freq"].as<int>();
    if (vm.count("amplitude-noise")) 
      amplitudenoise = vm["amplitude-noise"].as<float>();
    if (vm.count("heart-beat-mean")) 
      heart_rate_mean = vm["heart-beat-mean"].as<float>();
    if (vm.count("heart-rate-std")) 
      heart_rate_std = vm["heart-rate-std"].as<float>();
    if (vm.count("low-freq")) 
      lowfreq = vm["low-freq"].as<float>();
    if (vm.count("high-freq")) 
      highfreq = vm["high-freq"].as<float>();
    if (vm.count("low-freq-std")) 
      lowfreqstd = vm["low-freq-std"].as<float>();
    if (vm.count("high-freq-std")) 
      highfreqstd = vm["high-freq-std"].as<float>();
    if (vm.count("lf-hf-radio")) 
      lfhfradio = vm["lf-hf-radio"].as<float>();


    
    
  } 
  catch (...) {
    std::cout << desc << "\n";
    return false;
  }
  return true;
}






