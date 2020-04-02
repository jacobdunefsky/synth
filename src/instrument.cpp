#include "instrument.hpp"
#include <iostream>

double Instrument::sample(double x, int frames, double freq){
	double envelope = suslevel;
	//std::cout << atime*bitrate;
	//std::cout << frames-(atime*bitrates) << " ";
	if(frames < atime*bitrate){
		envelope = frames*suslevel/(atime*bitrate);
	}
	else{
		envelope = suslevel + ((-suslevel/(rtime*bitrate)) * (frames-(atime*bitrate)));
		envelope = (envelope > 0)?envelope:0;
	}
	return wavesample(x*(freq/bitrate))*envelope;
}