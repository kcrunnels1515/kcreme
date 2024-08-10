#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_
#include <cstdint>
#include <unistd.h>
#include <list>
#include <iostream>
#include <sys/mman.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include <unordered_map>
#include <keystone/keystone.h>

#define PAGE 4096

struct Chunk {
  unsigned byte_offset;
  uint8_t* addr;
  uint16_t len;
  Chunk(unsigned offset, uint8_t* addr, unsigned len);
};

class MemoryManager {
  private:
  uint32_t _size;
  uint32_t _capacity;
  uint8_t* _mem_start;
  uint8_t* _last;
  std::list<Chunk>* _chunks;
  std::unordered_map<std::string, uint8_t*>* _fns;
  ks_engine *ks;

  public:
  MemoryManager();
  ~MemoryManager();
  void initialize();
  void resize();
  void shutdown();
  // returrns ptr to location of a piece of code to execute
  // for easiest time, i think no args
  // return value ---> ???
  uint8_t *allocate(std::string& code, std::string name = "");
  void free(void *address);
  void *getList();
  uint8_t *getMemoryStart();
  uint8_t *getCurPtr();
  unsigned getMemoryLimit();
};

int bestFit(int sizeInWords, void *list);

#endif // MEMORYMANAGER_H_
