# Overview

This document gives an overview into the process of developing a LRU cache.

# Conceptual workflow

The development of the cache revolves around a few key questions and developing a function ( or more ) in a class structure to solve each problem

1. Least-recently-used cache replacement algorithm
2. Reading data from the cache and outputting the data to stdout
3. Writing data to the cache
4. What happens when a block is not in the cache?
5. What happens when a block is dirty and not in the cache?
6. Data structure for storing data in the cache and memory
7. How to get the set index, tag index, byte offsets etc

# File / data structure
A few key points behind the thought process of developing the cache:

* A map key-value store is used to store the data in the cache with a search time of O(log N), this contrasts with an array / vector data structure with a search time of O(N) - thus using a map is more optimal
* C++ classes were used to express the LRU_cache to help intuition and code readability
* Structs are used to organize blocks of data as an atomic unit of storage, as such:

<pre>struct block{
        /*
            words in a block
            tag
            valid
            dirty
        */
    }</pre>

# Compilation

This project consists of 3 files.  
* mem_sim.cpp ( main file )
* mem_sim.h ( cache functionality declarations for LRU cache )
* mem_sim_cache.cpp ( code implementation for LRU cache functionality - as declared in mem_sim.h )
 
To compile, simply execute    

    g++ -std=c++11 mem_sim.cpp mem_sim_cache.cpp

To run it against some test cases, check out the Testing and Debugging section.

# Testing and Debugging
Testing and debugging is done via a 2 step process

1. Creating test cases for the cache ( direct-mapped, fully-associative etc )
2. Comparing the output of the code with the test cases

As repeated tests were required to develop a working cache, the process of compiling and testing is automated via a shell script. Before executing the script, ensure that the command "chmod u+x testing_script.sh" has been applied to make the .sh file an executable


To check if chmod u+x has been applied, enter the directory which contains testing_script.sh and enter the following

    ls -l | grep testing_script.sh
    
Ensure that it looks something like -rwxrwxr--+ 

Finally, to run the script ( does the compilation and testing ),

    ./testing_script.sh

