// Helper class that prints to a string buffer, for emulating Serial's printing methods.

#include "StringPrinter.h"

StringPrinter::StringPrinter(int buff_size) 
: _buff_size(buff_size), _buff_index(0) {
  _buff = new uint8_t[buff_size];
}

StringPrinter::~StringPrinter() {
  delete[] _buff;
}

size_t StringPrinter::write(uint8_t c) {
  if (_buff_index < _buff_size - 1) {
    _buff[_buff_index] = c;
    _buff_index++;
  } else {
    // No-op, just drop data that's too long, sorry!
	// TODO: force early send somehow, or implement a dynamic array.
  }
  return 1;
}

char* StringPrinter::get_buff() {
  char* buff_copy = new char[_buff_index+1];
  memcpy(buff_copy, (char*)_buff, _buff_index);
  buff_copy[_buff_index] = (uint8_t)'\0';
  
  // Rely on caller for freeing this
  return buff_copy;
}

void  StringPrinter::reset() {
  _buff_index = 0;
}