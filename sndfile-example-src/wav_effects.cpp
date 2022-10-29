#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"
#include "algorithm"
using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {
	if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output_file> ( reverse | left | right | single_echo | double_echo  | triple_echo | amplitude_loop)\n";
		return 1;
	}
    
    string effect = argv[argc-1];
	SndfileHandle sfhIn { argv[argc-3] };

	if(sfhIn.error()) {
		cerr << "Error: invalid input file\n";  
		return 1;
    }

	if((sfhIn.format() & SF_FORMAT_TYPEMASK) != SF_FORMAT_WAV) {
		cerr << "Error: file is not in WAV format\n";
		return 1;
	}

	if((sfhIn.format() & SF_FORMAT_SUBMASK) != SF_FORMAT_PCM_16) {
		cerr << "Error: file is not in PCM_16 format\n";
		return 1;
	}
    SndfileHandle sfhOut { argv[argc-2], SFM_WRITE, sfhIn.format(),
    sfhIn.channels(), sfhIn.samplerate() };
	if(sfhOut.error()) {
		cerr << "Error: invalid output file\n";
		return 1;
    }

    size_t nFrames;
    bool flag = true;
    vector<short> samples(FRAMES_BUFFER_SIZE * sfhIn.channels());
    vector<short> samples_out_echo(FRAMES_BUFFER_SIZE * sfhIn.channels());
    vector<short> samples_out;
    int value = 0;
    int counter = 0;
    int i = 0;
    while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        i++;
        if (effect == "reverse") {
            cout << "Reversing...\n";
            short aux = 0;
            for (size_t i = 0; i < samples.size(); i++ ) {
                samples_out.push_back(samples[i]);
            }
        } else if (effect == "left") { //LEFT
            for (size_t i = 0; i < samples.size(); i++) {
                if (value % 2 == 0) {
                    samples_out.push_back(samples[i]);
                } else {
                    samples_out.push_back(-32768);
                }
                value++;
            }
        } else if (effect == "right") {
            for (size_t i = 0; i < samples.size(); i++) {
                if (value % 2 == 0) {
                    samples_out.push_back(-32768);
                } else {
                    samples_out.push_back(samples[i]);
                }
                value++;
            }
        } else if(effect == "single_echo") {
            for (size_t i = 0; i < samples.size(); i++) {
                samples_out_echo[i] = (samples[i] + samples[i-44000])*0.5;
            }
        } else if(effect == "double_echo") {
            for (size_t i = 0; i < samples.size(); i++) {
                samples_out_echo[i] = (samples[i] + samples[i-44100] + samples[i-88200])*0.5;
            }
        
        } else if(effect == "triple_echo") {
            for (size_t i = 0; i < samples.size(); i++) {
                samples_out_echo[i] = (samples[i] + samples[i-44100] + samples[i-88200] + samples[i-samples.size()])*0.4;
            }
        }else if (effect == "amplitude_loop"){
            cout << "Amplitude Loop...\n";
            for (size_t i = 0; i <= samples.size(); i++) {
                samples_out.push_back(samples[i] * (double(value)/double(samples.size())));
                if (flag == 1){
                    value++;
                }else{
                    value--;
                }
            }
            flag = !flag;

        }else{
            cerr << "Error: invalid effect\n";
            return 1;
        }
        if (effect == "single_echo" || effect == "double_echo" || effect == "triple_echo"){
            sfhOut.writef(samples_out_echo.data(), nFrames);
        }
    }

    if (effect == "single_echo" || effect == "double_echo" || effect == "triple_echo"){
        cout << "Done!\n";
        return 1;
    }
    // reverse samples_out
    else if (effect == "reverse"){
        // reverse samples_out in group of 2
        std::reverse(samples_out.begin(), samples_out.end());
        for (size_t i = 0; i < samples_out.size(); i+=2 ) {
            short aux = samples_out[i];
            samples_out[i] = samples_out[i+1];
            samples_out[i+1] = aux;
        }
        cout << samples_out.size() << endl;
        sfhOut.writef(samples_out.data(),FRAMES_BUFFER_SIZE*i);

    }else if (effect != "reverse"){
        sfhOut.writef(samples_out.data(), FRAMES_BUFFER_SIZE*i);
    }

    cout << "Done!\n";
}