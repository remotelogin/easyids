#include <iostream>

#include "../include/json.hpp"
#include "../include/definitions.hpp"
#include "serial_wrapper.hpp"

void from_json(const nlohmann::json& j, presence_sensor_data& d) {
    j.at("id").get_to(d.id);
    j.at("x").get_to(d.x);
    j.at("y").get_to(d.y);
    j.at("speed").get_to(d.speed);
    j.at("distance").get_to(d.distance);
    j.at("valid").get_to(d.valid);
}

int main() {

  std::cout << "initializing...\n";

  const char *portname = "/dev/ttyACM0";
  int fd = serial_wrapper::openSerialPort(portname);
  if (fd < 0)
    return 1;

  if (!serial_wrapper::configureSerialPort(fd, B9600)) {
    serial_wrapper::closeSerialPort(fd);
    return 1;
  }

  const char *message = "Hello, Serial Port!";
  if (serial_wrapper::writeToSerialPort(fd, message, strlen(message)) < 0) {
    std::cerr << "Error writing to serial port: " << strerror(errno)
              << std::endl;
  }

  std::string buffer;
  char temp[64];
  
  while (true) {
    int n = serial_wrapper::readFromSerialPort(fd, temp, sizeof(temp));
    
    if (n > 0) {
      buffer.append(temp, n);
      
      size_t pos;
      while ((pos = buffer.find('\n')) != std::string::npos) {
	
	std::string line = buffer.substr(0, pos);
	buffer.erase(0, pos + 1);
	
	if (!line.empty()) {
	  if(line == "\\TJ") {
	    //std::cout<< "delimiter detected!\n";
	  } else {
	    //LINE: {"id":1,"x":0,"y":0,"speed":0,"distance":0,"valid":0}
	    //std::cout << "chunk: " << line << std::endl;
	    nlohmann::json jdata = nlohmann::json::parse(line);
	    presence_sensor_data pd = jdata.get<presence_sensor_data>();

	    if(pd.valid == 1)
	      std::cout << "PERSON DETECTED!!!\n";
	    
	  }
	}
      }
    }
  }
  
  serial_wrapper::closeSerialPort(fd);
  return 0;
}
