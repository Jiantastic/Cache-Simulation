#include "mem_sim_cache.h"
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

LRU_Cache::LRU_Cache(uint32_t number_of_blocks){
	cache_size = number_of_blocks;
}

bool LRU_Cache::cache_hit_check(const uint32_t &address,const uint32_t &tag_index, const uint32_t &set_index,const uint32_t blocks_per_set){	// helper function
	// check if block is in set, tag match and valid match
	std::list<uint32_t> list1;
	std::list<uint32_t>::iterator it;
	uint32_t block_index = address % blocks_per_set;

	list1 = cache_LRU[set_index];

	for(it = list1.begin(); it != list1.end(); ++it){
		// if valid and tag match
		if( (*it) == block_index){
			// now we know that the block is in the set, does tag and valid match?
			std::map<uint32_t,block> m;
			m = sets[set_index];
			if( (m[block_index].valid == true) && (m[block_index].tag == tag_index) ){
				return true;
			}
		}
	}
	return false;
}

// write
void LRU_Cache::set(const uint32_t &data, const uint32_t &address, const uint32_t &set_index, const uint32_t &word_index, const uint32_t &tag_index, const uint32_t &blocks_per_set,const bool &dirty_flag){
	// only after cache hit or post dirty function handling
	maintain_order_of_addresses(address,set_index,tag_index,blocks_per_set);
	std::map<uint32_t,block> m;
	uint32_t block_index = address % blocks_per_set;

	m = sets[set_index];
	m[block_index].tag = tag_index;
	m[block_index].address = address;
	m[block_index].valid = true;
	m[block_index].dirty = dirty_flag;
	m[block_index].words[word_index] = data;
	sets[set_index] = m;
}

uint32_t LRU_Cache::get(const uint32_t &address, const uint32_t &set_index, const uint32_t &word_index,const uint32_t &tag_index, const uint32_t &blocks_per_set){			// address here is block index
	maintain_order_of_addresses(address,set_index,tag_index,blocks_per_set);
	std::map<uint32_t,block> m;
	uint32_t block_index = address % blocks_per_set;
	
	m = sets[set_index];

	return (m[block_index].words[word_index]);
}

void LRU_Cache::maintain_order_of_addresses(const uint32_t &address, const uint32_t &set_index,const uint32_t &tag_index, const uint32_t &blocks_per_set){
	// executes after every get / set
	std::list<uint32_t> list1;
	std::list<uint32_t>::iterator it;
	uint32_t block_index = address % blocks_per_set;
	list1 = cache_LRU[set_index];		// get list stored at set_index

	bool found = false;
	for(it = list1.begin(); it != list1.end(); ++it){
		if( (*it) == block_index){
			found = true;
			// bring it forward
			list1.remove(*it);
			list1.push_back(*it);
		}
	}

	if(!found){	// not found
		if(list1.size() > blocks_per_set){
			// delete LRU to make some space
			it = list1.begin();
			list1.erase(it);
		}
		//memory_write();
		list1.push_back(block_index);
		cache_LRU[set_index] = list1;
		// have to get data from memory to put uint32_to the cache after this
	}
}

bool LRU_Cache::block_dirty_check(const uint32_t &address, const uint32_t &set_index, const uint32_t &tag_index, const uint32_t &blocks_per_set){
	uint32_t block_index = address % blocks_per_set;
	std::map<uint32_t,block> m;
	m = sets[set_index];

	if(m[block_index].dirty){
		return true;
	}
	return false;
}

void LRU_Cache::flush_response(const uint32_t &address, const uint32_t &set_index, const uint32_t &blocks_per_set, std::map<uint32_t,block> &memory){
	std::map<uint32_t,std::map<uint32_t,block> > s;
	std::map<uint32_t,std::map<uint32_t,block> >::iterator it1;
	std::map<uint32_t,block> m;
	std::map<uint32_t,block>::iterator it2;
	for(it1 = sets.begin(); it1 != sets.end(); ++it1){
		m = it1->second;
		for(it2 = m.begin(); it2 != m.end(); ++it2){
			// it2->second = block data
			(it2->second).dirty = false;
			memory[(it2->second).address] = (it2->second);
		}
	}
}


void LRU_Cache::print_cache_contents(const uint32_t &words_per_block){
	std::map<uint32_t,std::map<uint32_t,block> >::iterator it1;
	std::map<uint32_t,block> m;
	std::map<uint32_t,block>::iterator it2;

	for(it1 = sets.begin();it1 != sets.end(); ++it1){
		m = it1->second;
		for(it2 = m.begin();it2 != m.end(); ++it2 ){
			std::cout << "set-index " << it1->first << ", block-index " << it2->first << ", data = ";
			for(int i=0;i<words_per_block;++i){
				std::cout << (it2->second).words[i] << " ";
			}
			std::cout << "" << std::endl;
		}
	}
}
