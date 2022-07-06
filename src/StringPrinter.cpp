// Helper class that prints to a string buffer, for emulating Serial's printing methods.

#include "StringPrinter.h"

StringPrinter::StringPrinter(unsigned int buff_size)
: _buff_size(buff_size), _buff_index(0) {
  _buff = new uint8_t[buff_size];
}

StringPrinter::~StringPrinter() {
  delete[] _buff;
}

size_t StringPrinter::write(uint8_t c) {
  if (_buff_index < _buff_size) {
    _buff[_buff_index] = c;
    _buff_index++;
  } else {
    // Just drop data that's too long, sorry!
    // TODO: force early send somehow, or implement a dynamic array.
    return 0;
  }
  return 1;
}

const uint8_t* StringPrinter::get_buff() {
  return _buff;
}

void StringPrinter::reset() {
  _buff_index = 0;
}

unsigned int StringPrinter::length() {
  return _buff_index;
}
