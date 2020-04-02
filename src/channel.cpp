#include "channel.hpp"
#include <iostream>

Channel::Channel(double myFreq, double myVol){
	curFreq = myFreq;
	curVol = myVol;
}

double Channel::sample(){
	double retval = curVol*curInst->sample(x, frames, curFreq);
	return retval;
}

double Channel::advance(){
	double retval = sample();
	x++;
	if(!sustain)
		frames++;
	return retval;
}

void Channel::play(Instrument* myInst, double myVol, double myFreq){
	curInst = myInst;
	x = (frames = 0);
	curVol = myVol;
	curFreq = myFreq;
}

void Channel::play(double myVol, double myFreq){
	x = (frames = 0);
	curVol = myVol;
	curFreq = myFreq;
}