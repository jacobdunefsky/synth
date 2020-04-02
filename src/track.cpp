#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>

#include <portaudio.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "wavegen.hpp"
#include "channel.hpp"
#include "waves.cpp"

#define arrlen(arr) (sizeof(arr)/sizeof(arr[0]))

void addNShift(double* a, double v, int alen){
	for(int i = 0; i < alen-1; i++){
		a[i] = a[i+1];
	}
	a[alen-1] = v;
}

double* recentArr = new double[200];
int myWidth = 200;

double secsPerRow = 0.15;
int framesPerRow = secsPerRow*44100;
int playFrames = -1;

int playAll(const void* input, void* out_, unsigned long count, 
	const PaStreamCallbackTimeInfo* time, 
	PaStreamCallbackFlags status, void* data){

	float* out = (float*) out_;
	Channel* ch = (Channel*)data;
	for(int i = 0; i < count; i++){
		out[i] = 0;
		out[i] += ch->advance();
		playFrames++;
		addNShift(recentArr,out[i],myWidth);
	}
	return 0;
}

int main(int argc, char** argv){

	secsPerRow = std::stod(argv[2]);
	framesPerRow = secsPerRow*44100;

	PaError err;
	std::cout << framesPerRow;

	SawChorus* square = new SawChorus();

	int curOct = 0;
	//std::vector<Channel*>chs = new std::vector<>();
	
	Channel* ch = new Channel(0,0);
	ch->curInst = square;

	std::ifstream fp (argv[1]);

	sf::RenderWindow mainwin (sf::VideoMode(200,200), "Synth!");
	err = Pa_Initialize();
	PaStream *stream;

	//Get rid of lag
	PaStreamParameters *params = new PaStreamParameters();
	params->sampleFormat = paFloat32;
	int defout = Pa_GetDefaultOutputDevice();
	params->device = defout;
	params->channelCount = 1;
	//params->suggestedLatency = 0.02;
	params->suggestedLatency = Pa_GetDeviceInfo(defout)->defaultLowOutputLatency;

	Pa_OpenStream(&stream, NULL, params, 44100, paFramesPerBufferUnspecified, paNoFlag, playAll, ch);
	Pa_StartStream(stream);

	while(mainwin.isOpen()){

		sf::Event event;
		while(mainwin.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				mainwin.close();
			}
			if(event.type == sf::Event::Resized){
				myWidth = event.size.width;
				sf::FloatRect visibleArea(0, 0, myWidth, 200);
        		mainwin.setView(sf::View(visibleArea));
				delete[] recentArr;
				recentArr = new double[myWidth];
			}
		}
		if(playFrames > framesPerRow || playFrames == -1){
			std::string curNote;
			if(!std::getline(fp, curNote)){
				mainwin.close();
				break;
			}
			std::cout << curNote << ", ";
			playFrames = 0;
			if(curNote == "="){
				ch->sustain = false;
			}
			else if(curNote != ""){
				std::string pc ("");
				std::string oct ("");

				int pcpos = 0;
				for(;curNote[pcpos] != '-'; pcpos++){
					pc += curNote[pcpos];
				}
				for(pcpos++; pcpos < curNote.length() && curNote[pcpos] != '#'; pcpos++)
					oct += curNote[pcpos];
				ch->play(0.2,pow(2.0,std::stod(oct)+(std::stod(pc)+3)/12.0)*220);
				ch->sustain = true;
			}
			else{
				ch->sustain == true;
			}
		}

		//Render oscilliscope
		mainwin.clear(sf::Color::Black);
		sf::VertexArray p (sf::Points, myWidth);

		for(int i = 0; i < myWidth; i++){
			double ycoord = -100*(recentArr[i]-1);
			p[i].position = sf::Vector2f (i, ycoord);
			p[i].color = sf::Color::White;
		}
		mainwin.draw(p);
		mainwin.display();
	}

	//cleanup

	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	delete square;
	delete ch;
	delete params;

	delete[] recentArr;

	return 0;
}