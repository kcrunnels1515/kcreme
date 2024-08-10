#include "MemoryManager.hpp"

Chunk::Chunk(unsigned offset, uint8_t* addr, unsigned len) :
  byte_offset(offset),
  addr(addr),
  len(len) {}

MemoryManager::MemoryManager(){

  this->_chunks = new std::list<Chunk>();
  this->_fns = new std::unordered_map<std::string,uint8_t*>();
}

MemoryManager::~MemoryManager(){
  if (this->_mem_start != nullptr) this->shutdown();
  delete this->_chunks;
  delete this->_fns;
}

void MemoryManager::initialize(){
  if (this->_mem_start != nullptr) this->shutdown();
  this->_size = 0;
  this->_capacity = PAGE*2;
  this->_mem_start = (uint8_t*)mmap(NULL, this->_capacity, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0 , 0);
  this->_last = this->_mem_start;
  if (this->_mem_start == MAP_FAILED) shutdown();
  ks_err err = ks_open(KS_ARCH_X86, KS_MODE_64, &this->ks);
  if (err != KS_ERR_OK) {
     	    printf("ERROR: failed on ks_open(), quit\n");
     	    shutdown();
  }
}

void MemoryManager::shutdown(){
  int err = munmap(this->_mem_start,this->_capacity);
  if (err != 0) {
    std::cout << "Unmapping memory failed" << std::endl;
  }
  this->_size = 0;
  this->_capacity = 0;
  this->_mem_start = nullptr;
  this->_last = nullptr;
  this->_chunks->clear();
  this->_fns->clear();
  ks_close(this->ks);
}

uint8_t *MemoryManager::allocate(std::string& code, std::string name){
  uint8_t* encode;
  size_t size;
  size_t count;

  if (ks_asm(this->ks, code.c_str(), 0, &encode, &size, &count) != KS_ERR_OK) {
      printf("ERROR: ks_asm() failed & count = %lu, error = %u\n",
           count, ks_errno(ks));
      return nullptr;
  } else {
    // INSERT CHUNK IN MEMORY
    uint16_t* holes = (uint16_t*)this->getList();
    uint8_t* ret_addr;
    int wordOffset = bestFit(size, holes);
    if (wordOffset < 0) {
	    delete[] holes;
	    return nullptr;
    }
    Chunk temp = Chunk(wordOffset,this->_mem_start+wordOffset,size);
    auto it = this->_chunks->begin();
    if (wordOffset > 0) {
      for (; it != this->_chunks->end(); ++it) {
        if ((*it).addr > temp.addr) {
          break;
        }
      }
    }
    this->_chunks->insert(it, temp);
    delete[] holes;
    ret_addr = temp.addr;

    // COPY CONTENTS
    memcpy(temp.addr, encode, size);
    ks_free(encode);

    // CHECK DEFINE CALL
    if (!name.empty()) {
      // create vtable entry for defined function/variable
      std::string vt_code = "JMP " + std::to_string((uint64_t)temp.addr) + ";";
      // make sure to overwrite an existing vtable entry
      // so that recompiling functions doesn't require all other functions
      // to be recompiled to have correct addresses
      auto it = this->_fns->find(name);
      if (it == this->_fns->end()) {
        // create new entry
        uint8_t* vt_loc = this->allocate(vt_code);
        if (vt_loc) {
          this->_fns->insert({name,vt_loc});
          ret_addr = vt_loc;
        } else
          return nullptr;
      } else {
        // overwrite existing entry
        uint8_t* vt_encode;
        size_t vt_size;
        size_t vt_count;
        if (ks_asm(this->ks, vt_code.c_str(), 0, &vt_encode, &vt_size, &vt_count) != KS_ERR_OK) {
          return nullptr;
        } else {
          // copy new jmp instr to existing vtable loc
          memcpy(it->second,vt_encode,vt_size);
          ret_addr = it->second;
          ks_free(vt_encode);
        }
      }
    }
    return ret_addr;
  }
}

void MemoryManager::free(void *address){
  auto it = this->_chunks->begin();
  for (; it != this->_chunks->end(); ++it) {
    if ((*it).addr == address) {
      this->_chunks->erase(it);
      break;
    }
  }
}

void *MemoryManager::getList(){
  std::vector<uint16_t> chunk_vec;
  int last_possible = 0;
  auto it = this->_chunks->begin();

  if (this->_chunks->empty()) {
    chunk_vec.push_back(0);
    chunk_vec.push_back(this->_size);
    goto past;
  } else if ((*it).byte_offset > 0) {
    chunk_vec.push_back(0);
    chunk_vec.push_back((*it).byte_offset);
    //++it;
  }

  while (it != this->_chunks->end()) {
    int cur_offset = (*it).byte_offset + (*it).len;
    auto it_temp = it;
    it_temp++;
    if (it_temp != this->_chunks->end()) {
      int next_beg = (*it_temp).byte_offset;
      if (cur_offset < next_beg) {
        chunk_vec.push_back(cur_offset);
        chunk_vec.push_back(next_beg - cur_offset);
      }
    }
    ++it;
  }
  last_possible = this->_chunks->back().byte_offset + this->_chunks->back().len;

  if ((uint32_t)last_possible < this->_size) {
    chunk_vec.push_back(last_possible);
    chunk_vec.push_back(this->_size - last_possible);
  }
past:
  size_t ind = chunk_vec.size() + 1;
  uint16_t* chunks = new uint16_t[ind];
  chunks[0] = chunk_vec.size() / 2;
  if (ind>1) std::copy(chunk_vec.begin(), chunk_vec.end(),chunks+1);
  return (void*)chunks;
}

uint8_t *MemoryManager::getMemoryStart(){
  return this->_mem_start;
}

uint8_t *MemoryManager::getCurPtr() {
  return this->_last;
}

unsigned MemoryManager::getMemoryLimit(){
  return this->_capacity;
}

int bestFit(int sizeInWords, void *list){
  uint16_t* ptr = (uint16_t*)list;
  uint16_t best = UINT16_MAX;
  size_t best_ind = -1;
  for (int i = 1; i < (ptr[0]*2); i+=2) {
    if (ptr[i+1] >= sizeInWords && ptr[i+1] < best) {
      best = ptr[i+1];
      best_ind = ptr[i];
    }
  }
  return best_ind;
}
