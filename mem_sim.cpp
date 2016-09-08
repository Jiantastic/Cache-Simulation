// g++ mem_sim.cpp mem_sim_cache.cpp
// cat direct-mapped.input | ./a 8 2 2 1 2 1 2 2
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <list>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <iomanip>  
#include <stdint.h>
#include "mem_sim_cache.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc < 8){
		cerr << "incorrect number of command line arguments" << endl;
		return 1;
	}

	uint32_t address_bits = atoi(argv[1]);
	uint32_t bytes_per_word = atoi(argv[2]);
	uint32_t words_per_block = atoi(argv[3]);
	uint32_t blocks_per_set = atoi(argv[4]);
	uint32_t sets_per_cache = atoi(argv[5]);
	uint32_t hit_time = atoi(argv[6]);
	uint32_t memory_read_time = atoi(argv[7]);
	uint32_t memory_write_time = atoi(argv[8]);

	LRU_Cache main_cache(blocks_per_set * sets_per_cache);
	map<uint32_t,block> main_memory;	// key : index address , value : a block of memory

	// cache rows = blocks
	string str1;
	uint32_t address;
	uint32_t hex_data;

	while(cin >> str1){
		// Max memory size of 8MB
		if(str1[0] == '#'){
			getline(cin,str1);
			continue;
		}
		
		cin >> dec >> address;	

		// for access address -> tag------- set index -------- block index ------- word index --------- byte offset ( depends on number of bytes per word )
		// *true* block address is block index
		// IMPORTANT: number of bits for each offset
		uint32_t byte_offset = log2(bytes_per_word);
		uint32_t word_offset = log2(words_per_block);
		//uint32_t block_offset = log2(blocks_per_set);	// what you will be using to index cache blocks - set_index
		uint32_t set_offset = log2(sets_per_cache);

		uint32_t block_index = 0;
		uint32_t word_index = 0;
		uint32_t tag_index = 0;
		uint32_t set_index = 0;

		// make it into a function
		string a="";
		stringstream ss;
		int mask=0;

		for(int i=0;i<word_offset;++i){
			a += "1";
		}
		ss << a;
		ss >> mask;

		word_index = (address >> byte_offset) & mask;

		a = "";
		ss.clear();

		for(int i=0;i<set_offset;++i){
			a += "1";
		}
		ss << a;
		ss >> mask;

		set_index = (address >> (byte_offset + word_offset)) & mask;

		a = "";
		ss.clear();

		tag_index = address >> (byte_offset + word_offset + set_offset);	// remainder is tag_index

		uint32_t miss_time = 0;

		if(str1 == "write-req"){
			cin >> hex >> hex_data;
			// if cache miss
			if(!(main_cache.cache_hit_check(address,tag_index,set_index,blocks_per_set))){	// cache hit miss and not in LRU_buffer
				// get data from memory
				if(main_cache.block_dirty_check(address,set_index,tag_index,blocks_per_set)){
					miss_time = memory_write_time;
					main_cache.flush_response(address,set_index,blocks_per_set,main_memory);
				}
				// get data from memory if miss
				block b;
				b = main_memory[address];
				b.words[word_index] = hex_data;
				main_memory[address] = b;

				// write data to cache
				main_cache.set(hex_data,address,set_index,word_index,tag_index,blocks_per_set,true);	// set dirty = true
				//main_cache.set(hex_data,address);

				if( (words_per_block == 1)){
					cout << "write-ack " << set_index << " miss " << (hit_time + miss_time) << endl;
				}
				else{
					cout << "write-ack " << set_index << " miss " << (memory_read_time + hit_time + miss_time) << endl;
				}
			}
			else{	// if cache hit
				main_cache.set(hex_data,address,set_index,word_index,tag_index,blocks_per_set,true);	// set dirty flag = true
				cout << "write-ack " << set_index << " hit " << hit_time << endl;
			}
		}
		else if(str1 == "read-req"){
			if(main_cache.cache_hit_check(address,tag_index,set_index,blocks_per_set)){	// if cache hit
				cout << "read-ack " << set_index << " hit " << hit_time << " " << setfill('0') << setw(bytes_per_word*2) << uppercase << hex << main_cache.get(address,set_index,word_index,tag_index,blocks_per_set) << endl;
			}	
			else{
				// cache miss
				// else if main_cache is not dirty
				// read data from memory into cache
				if(main_cache.block_dirty_check(address,set_index,tag_index,blocks_per_set)){
					miss_time = memory_write_time;
					main_cache.flush_response(address,set_index,blocks_per_set,main_memory);
				}
					// need to write back old data to memory
				uint32_t temp_data;
				block temp_block;
				temp_block = main_memory[address];
				temp_data = temp_block.words[word_index];

				main_cache.set(temp_data,address,set_index,word_index,tag_index,blocks_per_set,false);	// set dirty bit = false
				cout << "read-ack " << set_index << " miss " << (memory_read_time + hit_time + miss_time) << " " << setfill('0') << setw(bytes_per_word*2) << uppercase << hex << main_cache.get(address,set_index,word_index,tag_index,blocks_per_set) << endl;
			}
		}
		else if(str1 == "flush-req"){
			// check all dirty blocks in cache and write them to memory, cache-memory consistency
			cout << "start of flush response" << endl;
			main_cache.flush_response(address,set_index,blocks_per_set,main_memory);
			cout << "end of flush response" << endl;
		}
		else if(str1 == "debug-req"){
			cout << "start of debug response" << endl;
			main_cache.print_cache_contents(words_per_block);
			cout << "end of debug response" << endl;
		}
	}
	return 0;
}
