#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <vector>
#include <map>
#include <sndfile.hh>

class WAVHist {
  private:
	std::vector<std::map<short, size_t>> counts;

  public:
	WAVHist(const SndfileHandle& sfh) {
		counts.resize(sfh.channels());
	}

	void update(const std::vector<short>& samples) {
		size_t n { };
		for(auto s : samples)
			counts[n++ % counts.size()][s]++;
	}

	void dump() const {
		std::map<short, size_t>::const_iterator index1;
		std::map<short, size_t>::const_iterator index2 = counts[1].begin();
		for(index1 = counts[0].begin(); index1 != counts[0].end(); index1++) {
			std::cout << index2->first << " " << (index2->second + index1->second)/2<< std::endl;
			index2++;
		}

		// for(auto p : counts[0])
		// 	std::cout << p.first << " " << p.second << std::endl;
		// for(auto p : counts[1])
		// 	std::cout << p.first << " " << p.second << std::endl;
		// for(auto [value, counter] : counts[1])
		// 	std::cout << value << '\t' << counter << '\n';
		// std::cout << "//////////////";
		// for(auto [value, counter] : counts[0])
		// 	std::cout << value << '\t' << counter << '\n';
		

	}
};

#endif

