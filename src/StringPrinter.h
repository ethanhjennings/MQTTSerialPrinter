// Helper class that prints to a string buffer, for emulating Serial's printing methods.

#ifndef _STRING_PRINTER_H_
#define _STRING_PRINTER_H_

#include <Arduino.h>

class StringPrinter : public Print {

private:
  uint8_t* _buff;
  int _buff_index;
  int _buff_size;
	  
public:
  StringPrinter(int buff_size);
  ~StringPrinter();
  char * get_buff(); // Returns a copy that needs to be freed
  void reset();
  virtual size_t write(uint8_t c);
  bool has_data() {return _buff_index > 0;}
};

#endif