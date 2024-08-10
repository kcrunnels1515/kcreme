#include "ValueInterface.hpp"

inline bool is_int(const std::string & s) // https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

inline bool is_float(const std::string & s) // https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtod(s.c_str(), &p);

   return (*p == 0);
}

ValuePool::ValuePool() :
  size(0),
  mem(nullptr)
  {
    this->chunks = new std::list<ValChunk>();
    this->initialize(2);
  }

ValuePool::~ValuePool() {
  if (this->mem) this->shutdown();
  delete this->chunks;
}

void ValuePool::initialize(uint8_t pages) {
  if (this->mem) this->shutdown();
  this->size = PAGE*pages;
  this->mem = (uint8_t*)mmap(NULL, this->size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, 0 , 0);
  if (this->mem == MAP_FAILED) shutdown();
}

void ValuePool::shutdown(){
  int err = munmap(this->mem,this->size);
  if (err != 0) {
    std::cout << "Unmapping memory failed" << std::endl;
  }
  this->size = 0;
  this->mem = nullptr;
  this->chunks->clear();
  this->vars.clear();
}

ValChunk* ValuePool::allocate(size_t size){
  uint16_t* holes = this->getList();

  uint16_t best = UINT16_MAX;
  size_t offset = -1;
  for (int i = 1; i < (holes[0]*2); i+=2) {
    if (holes[i+1] >= size && holes[i+1] < best) {
      best = holes[i+1];
      offset = holes[i];
    }
  }

  if (offset < 0) {
	  delete[] holes;
	  return nullptr;
  }
  ValChunk temp = ValChunk(offset,this->mem+offset,size);

  auto it = this->chunks->begin();
  if (offset > 0) {
    for (; it != this->chunks->end(); ++it) {
      if ((*it).addr > temp.addr) {
        break;
      }
    }
  }
  this->chunks->insert(it, temp);
  delete[] holes;
  return &*(--it);
}

void ValuePool::free(uint8_t* address){
  auto it = this->chunks->begin();
  for (; it != this->chunks->end(); ++it) {
    if ((*it).addr == address) {
      this->chunks->erase(it);
      break;
    }
  }
}

uint16_t* ValuePool::getList() {
  std::vector<uint16_t> chunk_vec;
  int last_possible = 0;
  auto it = this->chunks->begin();

  if (this->chunks->empty()) {
    chunk_vec.push_back(0);
    chunk_vec.push_back(this->size);
    goto past;
  } else if ((*it).offset > 0) {
    chunk_vec.push_back(0);
    chunk_vec.push_back((*it).offset);
    //++it;
  }

  while (it != this->chunks->end()) {
    int cur_offset = (*it).offset + (*it).len;
    auto it_temp = it;
    it_temp++;
    if (it_temp != this->chunks->end()) {
      int next_beg = (*it_temp).offset;
      if (cur_offset < next_beg) {
        chunk_vec.push_back(cur_offset);
        chunk_vec.push_back(next_beg - cur_offset);
      }
    }
    ++it;
  }
  last_possible = this->chunks->back().offset + this->chunks->back().len;

  if (last_possible < this->size) {
    chunk_vec.push_back(last_possible);
    chunk_vec.push_back(this->size - last_possible);
  }
past:
  size_t ind = chunk_vec.size() + 1;
  uint16_t* chunks = new uint16_t[ind];
  chunks[0] = chunk_vec.size() / 2;
  if (ind>1) std::copy(chunk_vec.begin(), chunk_vec.end(),chunks+1);
  return chunks;
}

// value ValuePool::add_value(Type _t, uint16_t _type_num, uint16_t _len, uint8_t* _bytes) {
//   if (_type_num > 16383) return 0;
//   if (this->mem == nullptr) return 0;
//   uint64_t ret_val = 0;
//   ret_val |= ((uint64_t)_t << 62);
//   ret_val |= ((uint64_t)_type_num << 48);
//   uint8_t* ptr = this->allocate(_len);
//   memcpy(ptr, _bytes, _len);
//   ret_val |= (uint64_t) ptr;
//   return ret_val;
// }

bool ValuePool::add_value(std::string& name, std::vector<unsigned> type, void* data, size_t len) {
  if (this->mem == nullptr) return false;
  // debug_str is assumed not to be empty
  if (this->vars.find(name)!=this->vars.end()) return false;
  ValChunk* vc = this->allocate(len);
  if (vc) {
    this->vars[name] = vc;
    // need to change this to meaningful
    // type later, now it's all going to be
    // basic int, float, str, etc
    vc->tt = type;
    return true;
  }
  return false;
}

ValChunk* ValuePool::get_var(std::string& name) {
  std::map<std::string,ValChunk*>::iterator it = this->vars.find(name);
  if (it == this->vars.end()) return nullptr;
  return it->second;
}

bool ValuePool::is_var(std::string name) {
  if (this->vars.find(name) != this->vars.end()) return true;
  return false;
}
