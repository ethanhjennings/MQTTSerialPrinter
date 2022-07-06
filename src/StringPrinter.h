// Helper class that prints to a string buffer, for emulating Serial's printing methods.

#ifndef _STRING_PRINTER_H_
#define _STRING_PRINTER_H_

#include <Arduino.h>

class StringPrinter : public Print {

private:
  uint8_t* _buff;
  unsigned int _buff_index;
  unsigned int _buff_size;

public:
  StringPrinter(unsigned int buff_size);
  ~StringPrinter();

  const uint8_t* get_buff();
  void reset();
  virtual size_t write(uint8_t c);
  unsigned int length();
};

#endif
