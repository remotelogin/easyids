#pragma once

#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

namespace serial_wrapper {
  
  int openSerialPort(const char* portname);
  
  bool configureSerialPort(int fd, int speed);
  
  int readFromSerialPort(int fd, char* buffer, size_t size);
  
  int writeToSerialPort(int fd, const char* buffer, size_t size);
  
  void closeSerialPort(int fd);
  
};

