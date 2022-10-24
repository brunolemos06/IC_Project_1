  #ifndef BITSTREAM_H
  #define BITSTREAM_H

  #include <iostream>
  #include <vector>
  #include <fstream>
  using namespace std;
  
  class BitStream {
      private:
        fstream file;
        char mode;
        char buffer;
        int count;
  
      public:
        //constructor to initialize file and mode
        BitStream(const char* filename, char mode){
            if (mode == 'z'){
                this->mode = mode;
                file.open(filename, ios::in | ios::out | ios::binary);
            }else if (mode == 'w'){
                this->mode = mode;
                file.open(filename, ios::out | ios::binary);
            }else if (mode == 'r'){
                this->mode = mode;
                file.open(filename, ios::in | ios::binary);
            }else{
                cout << "Invalid mode" << endl;
              }
            buffer = 0;
            count = 0;
          }
        //Function to read the bits from 'decoder.txt' to a vector of chars
        vector<char> read_to_charvector(){
            ifstream file("decoder.txt");
            vector<char> bits;
            //read the file and store the bits in a vector
            char bit;
            while (file >> bit){
                bits.push_back(bit);
              }
            //print the bits
            for (int i = 0; i < bits.size(); i++){
                cout << bits[i];
            }
            file.close();
            return bits;
          }
        //Function Encoder
        void write_bit(char bit) {
            buffer <<= 1;
                if (bit == '1') {
                    buffer |= 1;
                  }
                count++;
                if(count == 8) {
                    file.write(&buffer, 1);
                    buffer = 0;
                    count = 0;
                }
        }
        //Function Encoder to N bits
        void write_bits(const vector<char> bits) {
            //get the & of the bits and the buffer
            for (int i = 0; i < bits.size(); i++){
                buffer <<= 1;
                if (bits[i] == '1') {
                    buffer |= 1;
                }
                count++;
                if(count == 8) {
                    file.write(&buffer, 1);
                    buffer = 0;
                    count = 0;
                  }
              }
        }
            
            // for (auto& b : bits) { 
            //     write_bit(b);
            // }
            //   }

        //Function Decoder
        void decoder() {
            //read the content of the file byte by byte
            char c;
            file.read(&c, 1);
            //convert the byte in a string of bits
            string bits = "";
            for (int i = 0; i < 8; i++) {
                bits = (c & 1 ? "1" : "0") + bits;
                c >>= 1;
            }
            //print the string of bits
            cout << bits << endl;
            ofstream file("decoder.txt");
            file << bits;
            file.close();
        }

        //function to flush the buffer
        void flush() {
            if (mode == 'w'){
                while(count != 0) {
                   write_bit('0');
                  }
              }
          }
    };
    #endif