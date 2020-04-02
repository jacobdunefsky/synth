#pragma once
#include "instrument.hpp"
class Channel{
private:
	const double bitrate = 44100;
public:
	Instrument* curInst;
	double curVol;
	double curFreq;
	double x;
	int frames;
	int time;
	bool sustain;

	Channel(double myFreq, double myVol);

	double sample();
	double advance();
	
	void play(Instrument* myInst, double myVol, double myFreq);
	void play(double myVol, double myFreq);
};