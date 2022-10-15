#ifndef BITSTREAM_H
#define BITSTREAM_H

#include <iostream>
#include <vector>
#include <fstream>

class BitStream {
    private:
        
        std::vector<int> quant;
        int bit;

    public:
        //constructor to initialize quant and initialize bit
        BitStream( int bit){
            this->bit = bit; 
            quant.resize(0);
        }
        //function to write in a file only 1 bit
        void write_bit(std::ofstream& file) {
                file << bit;
        }
        //funtion to read 
        void read_bit(std::ifstream& file) {
            file >> bit;
        }
        //funtion to write in file N bits
        void write_bits(std::ofstream& file, std::vector<int>& bits) {
            for (auto& b : bits) {
                file << b;
            }
        }
        //function to read N bits from file and put them in vector quant
        void read_bits(std::ifstream& file, size_t n) {
            int bit;
            for (size_t i = 0; i < n; i++) {
                file >> bit;
                quant[i] = bit;
            }
        }

        // function encoder
        //should be able to convert a text file containing only 0s and 1s into the binary equivalent (each byte
        //of the binary file should represent eight of the bits in the text file)
        void encode(std::ifstream& file, std::ofstream& file2) {
            int bit;
            std::vector<int> bits;
            while (file >> bit) {
                bits.push_back(bit);
                if (bits.size() == 8) {
                    write_bits(file2, bits);
                    file2 << ' ';
                    bits.clear();
                }
                //if the total number of bits in the text file is not a multiple of 8, then fill the last byte with 0s
                if (file.eof()) {
                    //if the last byte is not full then fill it with 0s
                    if (bits.size() != 8) {
                        while (bits.size() != 8) {
                            bits.push_back(0);
                        }
                        write_bits(file2, bits);
                        file2 << ' ';
                        bits.clear();
                    }
                }
            }
            
        }
        //function decoder
        //should be able to convert a binary file into a text file containing only 0s and 1s
        void decode(std::ifstream& file, std::ofstream& file2) {
            int bit;
            std::vector<int> bits;
            while (file >> bit) {
                bits.push_back(bit);
                if (bits.size() == 8) {
                    write_bits(file2, bits);
                    bits.clear();
                }
            }
        }
};

#endif 