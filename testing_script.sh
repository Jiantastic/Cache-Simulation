#!/bin/bash

g++ -std=c++11 mem_sim.cpp mem_sim_cache.cpp

cp a.exe testing
cd testing/

echo "compilation successful"

INPUT=direct-mapped.input
OUTPUT=direct-mapped.output
GOT=direct-mapped.got

cat ${INPUT} | ./a 8 2 2 1 2 1 2 2 > ${GOT}

diff ${OUTPUT} ${GOT}

echo "done direct-mapped"

INPUT=fully_associative.input
OUTPUT=fully_associative.output
GOT=fully_associative.got

cat ${INPUT} | ./a 16 1 4 1 4 1 1 1 > ${GOT}

diff ${OUTPUT} ${GOT}

echo "done fully_associative"

INPUT=single_byte_reads.input
OUTPUT=single_byte_reads.output
GOT=single_byte_reads.got

cat ${INPUT} | ./a 8 1 1 1 1 1 1 1 > ${GOT}

diff ${OUTPUT} ${GOT}

echo "done single-byte-reads"

INPUT=two_byte_reads.input
OUTPUT=two_byte_reads.output
GOT=two_byte_reads.got

cat ${INPUT} | ./a 8 2 1 1 1 1 1 1 > ${GOT}

diff ${OUTPUT} ${GOT}

echo "done two-byte-reads"

INPUT=four_byte_writes.input
OUTPUT=four_byte_writes.output
GOT=four_byte_writes.got

cat ${INPUT} | ./a 8 4 1 1 1 1 1 1 > ${GOT}

diff ${OUTPUT} ${GOT}

echo "done four-byte-reads"

