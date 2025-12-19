#ifndef REGISTER_H
#define REGISTER_H
#include <cstdint>
#include <iostream>
#include <math.h>
#include <stdint.h>

class xreg {
  /*
   * xreg can be initialize with a uint16_t value, an hexadecimal
   * by using 0x notation (i.e., reg = 0x00d2) or by using
   * binary notation (i.e., reg = 0b10110110.
   * and and or operator are supported as well (&& and ||).
   * bit at k position can be read using bracket notation
   * (i.e., reg[2]).
   *
   * xreg can be initialize with a special uint16_t value which
   * will represent an address. This can be use with asicio function
   * to clarify the code.
   *
   * Usage
   * -----
   * xreg register1(0x6004); //register(0x6004) // register(24580)
   * xreg register2(0x0fff);
   * register1[0] //return bit 0
   * register1 && (||) register2 // boolean operator
   * register1++(--) //increment register value
   * register1.to_string() // 24580
   * register1.to_stringb() // 110000000000100
   * register1.to_stringx() // 0x6004
   * register1.to_stringh() // h6004
   * xreg reg2 = register1.sub_chunks(0,8) // 00000100
   * register1.write_chunks(0b0110,2,4) // 110000000011000
   * register1.write_chunks(6,2,0) // 110000000010110
   */
public:
  xreg();
  xreg(uint16_t addr); // to be used as a ASIC register
  uint16_t xvalue;
  void fromHexStr(std::string hexStr); // convert Hex string to this.xvalue
  void addressFromHexStr(std::string hexStr);
  uint16_t address;
  uint16_t operator[](uint16_t k);
  void write_chunks(uint16_t bitarray, uint8_t position, uint16_t length);
  void write(uint16_t value, uint16_t pos, uint len); // deprecated
  uint16_t read_chunks(uint8_t position, uint16_t length);
  xreg sub_chunks(uint8_t position, uint16_t length);
  xreg read(uint16_t pos, uint len);
  void operator=(uint16_t k);
  void operator=(long int k);
  void operator=(int k);
  void operator=(std::string k);
  xreg &operator++();
  xreg &operator--();
  xreg &operator+=(uint16_t k);
  xreg operator&&(xreg k);
  xreg operator||(xreg k);
  void set(uint16_t k);
  void unset(uint16_t k);
  std::string to_string();
  std::string to_stringx();
  std::string to_stringh();
  std::string to_stringb();
  std::string addr_to_string();
  std::string addr_to_stringx();
  std::string addr_to_stringh();
  std::string addr_to_stringb();

  friend std::ostream &operator<<(std::ostream &os, const xreg &r);

private:
  uint16_t makemask(uint16_t pos, uint16_t length);
  uint16_t readBit(uint16_t number, uint16_t k);
  bool isRegister(std::string &str);
  bool isHex(std::string str);
  uint16_t hexStringToUint16(std::string &hexStr);
};

#endif // REGISTER_H
