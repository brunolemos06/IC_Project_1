#include <iostream>
#include <vector>
#include <sndfile.hh>
#include "wav_quant.h"

using namespace std;

constexpr size_t FRAMES_BUFFER_SIZE = 65536; // Buffer for reading frames

int main(int argc, char *argv[]) {
	if(argc < 4) {
		cerr << "Usage: " << argv[0] << " <input file> <output_file> ( reverse | left | right | single_echo | double_echo | triple_echo )\n";
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
    vector<short> samples(FRAMES_BUFFER_SIZE * sfhIn.channels());
    vector<short> samples_out(FRAMES_BUFFER_SIZE * sfhIn.channels()*2);
    int value = 0;
    while((nFrames = sfhIn.readf(samples.data(), FRAMES_BUFFER_SIZE))) {
        samples.resize(nFrames * sfhIn.channels());

        if (effect == "reverse") {
            short aux = 0;
            for (int i = 0; i < samples.size(); i+=1) {
                samples_out[i] = samples[samples.size()-i-1];
            }
            for (int i = 0; i < samples.size(); i+=2) {
                aux = samples_out[i];
                samples_out[i] = samples_out[i+1];
                samples_out[i+1] = aux;
            }
        } else if (effect == "left") { //LEFT
            for (int i = 0; i < samples.size(); i++) {
                if (value % 2 == 0) {
                    samples_out[i] = samples[i];
                } else {
                    samples_out[i] = -32768;
                }
                value++;
            }
        } else if (effect == "right") {
            for (int i = 0; i < samples.size(); i++) {
                if (value % 2 == 0) {
                    samples_out[i] = -32768;
                } else {
                    samples_out[i] = samples[i];
                }
                value++;
            }
        } else if(effect == "single_echo") {
            for (int i = 0; i < samples.size(); i++) {
                if (i < 44100) {
                    samples_out[i] = samples[i];
                } else {
                    samples_out[i] = samples[i] + samples[i-44100];
                }
            }
        } else if(effect == "double_echo") {
            for (int i = 0; i < samples.size(); i++) {
                if (i < 44100) {
                    samples_out[i] = samples[i];
                } else if (i < 88200) {
                    samples_out[i] = samples[i] + samples[i-44100];
                } else {
                    samples_out[i] = samples[i] + samples[i-44100] + samples[i-88200];
                }
            }
        } else if(effect == "triple_echo") {
            for (int i = 0; i < samples.size(); i++) {
                if (i < 44100) {
                    samples_out[i] = samples[i];
                } else if (i < 88200) {
                    samples_out[i] = samples[i] + samples[i-44100];
                } else if (i < 132300) {
                    samples_out[i] = samples[i] + samples[i-44100] + samples[i-88200];
                } else {
                    samples_out[i] = samples[i] + samples[i-44100] + samples[i-88200] + samples[i-132300];
                }
            }
        } else if(effect == "single_echo_2") {
            for (int i = 0; i < samples.size(); i++) {
                if (i < 88200) {
                    samples_out[i] = samples[i];
                } else {
                    samples_out[i] = samples[i] + samples[i-88200];
                }
            }
        } else if(effect == "double_echo_2") {
            for (int i = 0; i < samples.size(); i++) {
                if (i < 88200) {
                    samples_out[i] = samples[i];
                } else if (i < 176400) {
                    samples_out[i] = samples[i] + samples[i-88200];
                } else {
                    samples_out[i] = samples[i] + samples[i-88200] + samples[i-176400];
                }
            }
        } else{
            cerr << "Error: invalid effect\n";
            return 1;
        }

        sfhOut.writef(samples_out.data(), nFrames);
    }
    cout << "Done!\n";
}