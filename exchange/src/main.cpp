#include <iostream>

#include "../include/json.hpp"
#include "serial_wrapper.hpp"

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
	  std::cout << "LINE: " << line << std::endl;

	  //json shit

	}
      }
    }
  }
  
  serial_wrapper::closeSerialPort(fd);
  return 0;
}
