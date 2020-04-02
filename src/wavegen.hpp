#pragma once
#include <map>
#include <vector>
#include <portaudio.h>

int sign(double x);
enum class Wavetype {Sine, Saw, Square};

class Waves;

class Wave {
public:
	Wavetype wavetype;
	double x;
	double volume;
	double frequency;
	Wave(Wavetype my_type, double my_frequency, double my_volume, double my_x);
	Wave(Wavetype my_type, double my_frequency, double my_volume);
	Wave(Wavetype my_type, double my_frequency);
	unsigned int id;
	Waves* parent;
};

class Waves {
public:
	std::vector<Wave*> waves;
	std::vector<Wave*> findByFreq(double frequency);
	Wave* findById(int id);
	void addWave(Wave* wave);
private:
	unsigned int curId = 0;
};

int wavegen(const void *input, void* out, unsigned long count, 
	const PaStreamCallbackTimeInfo* time, 
	PaStreamCallbackFlags status, void *data);