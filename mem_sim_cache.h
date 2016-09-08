#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <list>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>  
#include <stdint.h>

#ifndef MEM_SIM_CACHE_H
#define MEM_SIM_CACHE_H

struct block{
	std::map<uint32_t,uint32_t> words;
	uint32_t tag;
	uint32_t address;
	bool valid=false;	// used initially to load data
	bool dirty=false;
};

class LRU_Cache{	// cache - sets - blocks - words - bytes
public:
	LRU_Cache(uint32_t total_size);		// Cache constructor, set to zero at start
	uint32_t get(const uint32_t &address, const uint32_t &set_index, const uint32_t &word_index,const uint32_t &tag_index, const uint32_t &blocks_per_set);		// address here is block index
	void set(const uint32_t &data, const uint32_t &address, const uint32_t &set_index, const uint32_t &word_index, const uint32_t &tag_index, const uint32_t &blocks_per_set,const bool &dirty_flag);
	bool cache_hit_check(const uint32_t &address,const uint32_t &tag_index, const uint32_t &set_index,const uint32_t blocks_per_set);	// helper function
	void maintain_order_of_addresses(const uint32_t &address,const uint32_t &set_index,const uint32_t &tag_index, const uint32_t &blocks_per_set);
	bool block_dirty_check(const uint32_t &address, const uint32_t &set_index, const uint32_t &tag_index, const uint32_t & blocks_per_set);
	void print_cache_contents(const uint32_t &words_per_block);
	void flush_response(const uint32_t &address, const uint32_t &set_index, const uint32_t &blocks_per_set, std::map<uint32_t,block> &memory);
	
private:
	std::map<uint32_t,std::list<uint32_t> > cache_LRU;	// key : set_index, value : list of blocks ( limit of blocks = blocks/set)
	std::map<uint32_t,std::map<uint32_t,block> > sets;	// key : block index address , value : block of data
	uint32_t cache_size;			// size of one line ( set / total number of blocks in a line)
};

#endif 
