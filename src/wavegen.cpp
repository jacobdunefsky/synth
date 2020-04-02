#include <cmath>
#include <iostream>
#include <vector>
#include <portaudio.h>
#include "wavegen.hpp"

int sign(double x){
	return (x<0)?-1:((x==0)?0:1);
}

Wave::Wave(Wavetype my_type, double my_frequency, double my_volume, double my_x){
	wavetype = my_type;
	frequency = my_frequency;
	x = my_x;
	volume = my_volume;
}
Wave::Wave(Wavetype my_type, double my_frequency, double my_volume){
	wavetype = my_type;
	frequency = my_frequency;
	volume = my_volume;
	x = 0;
}
Wave::Wave(Wavetype my_type, double my_frequency){
	wavetype = my_type;
	frequency = my_frequency;
	volume = 1;
	x = 0;
}

std::vector<Wave*> Waves::findByFreq(double frequency){
	std::vector<Wave*> retval;
	for(Wave* curWave : waves){
		if(curWave->frequency == frequency) retval.push_back(curWave);
	}
	return retval;
}

Wave* Waves::findById(int id){
	for(Wave* curWave : waves){
		if(curWave->id == id) return curWave;
	}
	return NULL;
}

void Waves::addWave(Wave* wave){
	wave->parent = this;
	wave->id = curId;
	curId++;
	waves.push_back(wave);
}

int wavegen(const void *input, void* out_, unsigned long count, 
	const PaStreamCallbackTimeInfo* time, 
	PaStreamCallbackFlags status, void *data){

	const double pi = 3.14159265358979;
	const unsigned int bitrate = 44100;

	float* out = (float*) out_;
	Waves* myData = (Waves*) data;

	double x = 0;

	for(int i = 0; i < count; i++){
		out[i] = 0;
		for(Wave* wave : myData->waves){
			switch(wave->wavetype){
				case Wavetype::Sine:
					out[i] += wave->volume*sin(wave->x*2*pi);
					break;
				case Wavetype::Saw:				
					out[i] += wave->volume*(fmod(wave->x,1)-0.5);
					break;
				case Wavetype::Square:
					out[i] += wave->volume*sign(fmod(wave->x,1)-0.5);
					break;
			}
			wave->x+=((double)wave->frequency)/bitrate;
		}
	}
	return 0;

}
