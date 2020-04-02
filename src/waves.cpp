#include "instrument.hpp"
#include <vector>

class Sine : public Instrument {
private:
	const double pi = 3.14159265358979;
public:
	double wavesample(double x){ 
		//(0.5*44100 - (double)frames)/(0.5*44100);
		//envelope = (envelope<0)?0:envelope;
		//normalize the volume by dividing by 2/pi
		return sin(x*2*pi)/0.636619; 
	}
};

class Saw : public Instrument {
public:
	double wavesample(double x){
		//normalize the volume by dividing by 0.25
		return 4*(fmod(x,1)-0.5); 
	}
};

class SawChorus : public Instrument{
public:
	double wavesample(double x){
		Saw s;
		return (s.wavesample(x)+s.wavesample((x-0.1)*(47.0/48)))/2;
	}
};

class Square : public Instrument {
private:
	int sign(double x){ return (x<0)?-1:((x==0)?0:1); }
public:
	Square(){
		rtime = 0.1;
	}
	double duty = 0.5;
	double wavesample(double x){ 
		//double envelope = (0.5*44100 - (double)frames)/(0.5*44100);
		//envelope = (envelope<0)?0:envelope;
		//volume is already normalized
		return sign(fmod(x,1)-duty); 
	}
};

class Additive : public Instrument {
private:
	int sign(double x){ return (x<0)?-1:((x==0)?0:1); }
	int min(int x, int y){ return (x>y)?y:x; }
public:
	double* coeffs;
	double* freqs;
	int ccount;
	int fcount;
	Additive(double* coeffs_, double* freqs_, int ccount_, int fcount_){
		coeffs = coeffs_;
		freqs = freqs_;
		ccount = ccount_;
		fcount = fcount_;
	}
	double wavesample(double x){
		Sine s;
		double retval = 0;
		for(int i = 0; i < min(fcount, ccount); i++){
			retval += coeffs[i]*(s.wavesample(freqs[i]*x));
		}
		return (abs(retval)>1)?sign(retval):retval;
	}
};

class Gamelan : public Instrument{
public:
	double wavesample(double x){
		double car_coeffs[] = {1, 0.3, 0.2, 0.5};
		double car_freqs[] = {1,2.8,5.5,9,16.7,17.8,20.5,22.9,24.9};

		Additive a (car_coeffs, car_freqs, sizeof(car_coeffs)/sizeof(car_coeffs[0]),
			sizeof(car_freqs)/sizeof(car_freqs[0]));
		return a.wavesample(x)+a.wavesample(x*(47.0/48));
	}
};

class Carillon : public Instrument{
public:
	double wavesample(double x){
		double coeffs[] = {1, 0.4, 0.3, 0.4};
		double freqs[] = {1,2,12.0/5.0,3,4};
		
		Additive a (coeffs, freqs, sizeof(coeffs)/sizeof(coeffs[0]),
			sizeof(freqs)/sizeof(freqs[0]));
		return a.wavesample(x)+a.wavesample(x*(47.0/48));
	}
};

class SawFM : public Instrument {
public:
	double sample(double x, int frames, double freq){
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

		Sine modulator;
		Saw carrier;
		double modfq = modulator.wavesample(0.5*freq*(x/(double)bitrate));
		double index = 0.3;
		return carrier.wavesample(x*(freq/bitrate)+index*modfq)*envelope;
	}
	double wavesample(double x){
		Saw s;
		return s.wavesample(x);
	}
};
