#include "serial_wrapper.hpp"

#include <iostream>
#include <cerrno>
#include <cstring> 

int serial_wrapper::openSerialPort(const char *portname) {
  int fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    std::cerr << "Error opening " << portname << ": " << strerror(errno)
              << std::endl;
    return -1;
  }
  return fd;
}

bool serial_wrapper::configureSerialPort(int fd, int speed) {
  struct termios tty;
  if (tcgetattr(fd, &tty) != 0) {
    std::cerr << "Error from tcgetattr: " << strerror(errno) << std::endl;
    return false;
  }

  cfsetospeed(&tty, speed);
  cfsetispeed(&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8; // 8-bit characters
  tty.c_iflag &= ~IGNBRK;                     // disable break processing
  tty.c_lflag = 0;                            // no signaling chars, no echo, no
                                              // canonical processing
  tty.c_oflag = 0;                            // no remapping, no delays
  tty.c_cc[VMIN] = 0;                         // read doesn't block
  tty.c_cc[VTIME] = 5;                        // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);   // ignore modem controls,
                                     // enable reading
  tty.c_cflag &= ~(PARENB | PARODD); // shut off parity
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr(fd, TCSANOW, &tty) != 0) {
    std::cerr << "Error from tcsetattr: " << strerror(errno) << std::endl;
    return false;
  }
  return true;
}

int serial_wrapper::readFromSerialPort(int fd, char *buffer, size_t size) {
  return read(fd, buffer, size);
}

int serial_wrapper::writeToSerialPort(int fd, const char *buffer, size_t size) {
  return write(fd, buffer, size);
}

void serial_wrapper::closeSerialPort(int fd) { close(fd); }
