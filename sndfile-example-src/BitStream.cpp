#include <iostream>
#include <sndfile.hh>
#include <vector>
#include <cmath>
#include "BitStream.h"
using namespace std;


int main() {
    //test the class BitStream
    BitStream bs("test.txt", 'w');
    //read the bits from the file and store them in a vector
    //print the bits
    // vector<char> bits = {'0', '0', '1', '0', '0', '0', '1', '1'};
    // bs.write_bits(bs.read_to_charvector());
    bs.write_bits(bs.read_to_charvector(8),8);
    // bs.read_to_charvector();
    // bs.write_bits(bits);
    // bs.write_bit('1');
    
    //print the output of read_bit
    // bs.decoder();
    bs.flushl();

}