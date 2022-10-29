#include <iostream>
#include <sndfile.hh>
#include <vector>
#include <cmath>
#include "BitStream.h"
using namespace std;


int main() {
    //test the class BitStream
    BitStream bs("test.txt", 'w');
    bs.write_bits("decoder.txt",8);
    // bs.decoder();
}