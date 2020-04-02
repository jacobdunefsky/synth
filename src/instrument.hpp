#pragma once
class Instrument {
public:
	const double bitrate = 44100;
	//variables to control envelope
	double atime = 0; //attack time in seconds
	double suslevel = 0.2; //sustain volume level
	double rtime = 0.5; //release time in seconds
	//sample() should return the wave's position at position "x", time "frames"
	virtual double sample(double x, int frames, double freq);
	virtual double wavesample(double x) = 0;
};