#ifndef VALUEINTERFACE_H_
#define VALUEINTERFACE_H_
#include <stdint.h>
#include <list>
#include <unistd.h>
#include <string.h>
#include <map>
#include <vector>
#include <sys/mman.h>
#include <iostream>
#include "../Defs/Defs.hpp"

#define PAGE 4096


// first 2 bits: Type value, next 14 bits: type identifier,
// last 48 bits: pointer to data
//typedef uint64_t value;

// struct TypeDefn {
//   // make fund constructors for basic types
//   // concat-able so that new types can be built
//   // up from them
//   value* (*cons)(uint16_t, uint64_t); // arg1: len in bytes, arg2: data location
//   void (*read)(value);
//   void (*write)(value, uint64_t); // arg1: type object, arg2: data
// };

struct ValChunk {
  uint32_t offset;
  uint8_t* addr;
  uint32_t len;
  std::vector<unsigned> tt;
  ValChunk(uint32_t _offset, uint8_t* _addr, uint32_t _len)
    : offset(_offset), addr(_addr), len(_len) {}
};

class ValuePool {
private:
  // data fields
  uint16_t size;
  uint8_t* mem;
  std::list<ValChunk>* chunks;
  //std::map<uint16_t,TypeDefn> types;
  std::map<std::string, ValChunk*> vars;
  // methods
  void initialize(uint8_t pages);
  void shutdown();
  ValChunk* allocate(size_t size);
  void free(uint8_t* address);
  uint16_t* getList();

public:
  ValuePool();
  ~ValuePool();
  //void* add_value(Type _t, uint16_t _type_num, uint16_t _len, uint8_t* _bytes);
  bool add_value(std::string& name, std::vector<unsigned> type, void* data, size_t len);
  ValChunk* get_var(std::string& name);
  bool is_var(std::string name);
};

inline bool is_int(const std::string & s);
inline bool is_float(const std::string & s);

#endif // VALUEINTERFACE_H_
