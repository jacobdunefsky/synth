#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include <portaudio/portaudio.h>
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

int playAll(const void* input, void* out_, unsigned long count, 
	const PaStreamCallbackTimeInfo* time, 
	PaStreamCallbackFlags status, void* data){

	float* out = (float*) out_;
	std::vector<Channel*>* my_c = (std::vector<Channel*>*)data;
	for(int i = 0; i < count; i++){
		out[i] = 0;
		for(int j = 0; j < my_c->size(); j++)
			out[i] += my_c->at(j)->advance();
		addNShift(recentArr,out[i],myWidth);
	}
	return 0;
}

int main(int argc, char** argv){
	int globalFrame = 0;

	PaError err;

	Square* square = new Square();

	Instrument* insts[] = 
		{new SawChorus(), new Sine(), square, new Carillon(), new Gamelan(), new SawFM()};
	int curInst = 0;
	int curOct = 0;
	double divisions = 12;
	if(argc > 1) divisions = std::stod(argv[1]);

	int chnum = 5;
	int curch = 0;

	std::vector<Channel*>* chs = new std::vector<Channel*>();
	for(int i = 0; i < chnum; i++){
		chs->push_back(new Channel(0,0));
		chs->at(i)->curInst = insts[0];
	}

	//I couldn't find a way around this.
	using sfk = sf::Keyboard;
	sfk::Key keys[] = {
		sfk::Q,sfk::W,sfk::E,sfk::R,sfk::T,sfk::Y, 
		sfk::U,sfk::I,sfk::O,sfk::P,sfk::LBracket,sfk::RBracket,
		sfk::A,sfk::S,sfk::D, sfk::F,sfk::G,sfk::H,
		sfk::J,sfk::K,sfk::L,sfk::SemiColon,sfk::Quote,sfk::Return,
		sfk::Z,sfk::X,sfk::C,sfk::V,sfk::B,sfk::N,
		sfk::M,sfk::Comma,sfk::Period,sfk::Slash,sfk::RShift,sfk::LControl
	};

	sfk::Key controlKeys[] = {sfk::Up, sfk::Down, sfk::Subtract, sfk::Add};

	bool keysDown[arrlen(keys)] = {false};
	bool controlDown[arrlen(controlKeys)] = {false};

	std::map<sfk::Key, int> keytoch;
	for (int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) 
		keytoch.insert(std::pair<sfk::Key,int>(keys[i],-1));

	bool pressedThisFrame = false;
	
	sf::RenderWindow mainwin (sf::VideoMode(200,200), "Synth!");
	err = Pa_Initialize();
	PaStream *stream;

	//Get rid of lag
	PaStreamParameters *params = new PaStreamParameters();
	params->sampleFormat = paFloat32;
	int defout = Pa_GetDefaultOutputDevice();
	params->device = defout;
	params->channelCount = 1;
	params->suggestedLatency = Pa_GetDeviceInfo(defout)->defaultLowOutputLatency;

	Pa_OpenStream(&stream, NULL, params, 44100, paFramesPerBufferUnspecified, paNoFlag, playAll, chs);
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

		//Handle control keys
		//Change instrument

		if(sfk::isKeyPressed(sfk::Up)){
			if(!controlDown[0]){
				curInst++;
				curInst %= sizeof(insts)/sizeof(insts[0]);
				for(int i = 0; i < chnum; i++){
					chs->at(i)->curInst = insts[curInst];
				}
			}
			controlDown[0] = true;
		}
		else{
			controlDown[0] = false;
		}
		if(sfk::isKeyPressed(sfk::Down)){
			if(!controlDown[1]){
				curInst = (curInst)?(curInst-1):(sizeof(insts)/sizeof(insts[0])-1);
				for(int i = 0; i < chnum; i++){
					chs->at(i)->curInst = insts[curInst];
				}
			}
			controlDown[1] = true;
		}
		else{
			controlDown[1] = false;
		}
		const double dutyStep = 0.00001;
		if(sfk::isKeyPressed(sfk::Left)){
			double curDuty = square->duty;
			square->duty = (curDuty>0)?(curDuty-dutyStep):(1-dutyStep);
		}
		if(sfk::isKeyPressed(sfk::Right)){
			double curDuty = square->duty;
			square->duty = fmod(curDuty+dutyStep,1);
		}

		if(sfk::isKeyPressed(sfk::Subtract)){
			if(!controlDown[2])
				curOct--;
			controlDown[2] = true;
		}
		else{
			controlDown[2] = false;
		}
		if(sfk::isKeyPressed(sfk::Add)){
			if(!controlDown[3])
				curOct++;
			controlDown[3] = true;
		}
		else{
			controlDown[3] = false;
		}

		pressedThisFrame = false;
		for(int i = 0; i < sizeof(keys)/sizeof(keys[0]); i++){
			int ipressed = sfk::isKeyPressed(keys[i]);
			if(ipressed){
				if(!keysDown[i]){
					chs->at(curch)->play(0.2,pow(2.0,curOct+((i+3)/divisions))*220);
					keytoch[keys[i]] = curch;
					curch = (curch+1)%chnum;
					keysDown[i] = true;
					pressedThisFrame = true;
				}
				else{
					chs->at(keytoch[keys[i]])->sustain = true;
					//curch = (curch+1)%chnum;

					//TODO: Write sustain code
				}
			}
			else if(!ipressed){
				//if(keysDown[i]){
				//	ch->curVol = 0;
				//}
				keysDown[i] = false;
				if(keytoch[keys[i]] != -1)
					chs->at(keytoch[keys[i]])->sustain = false;
				keytoch[keys[i]] = -1;
				//curch = (curch==0)?(chnum-1):(curch-1);
			}
		}
		//Render oscilliscope
		mainwin.clear(sf::Color::Black);
		sf::VertexArray p (sf::Points, myWidth);
		/*double outval = 0;
		for(int i = 0; i < chnum; i++){
			outval += chs->at(i)->sample();
		}
		if(globalFrame == 30){
			addNShift(recentArr,outval,100);
			globalFrame = 0;
		}
		else
			globalFrame++;*/
		for(int i = 0; i < myWidth; i++){
			double ycoord = -100*(recentArr[i]-1);
			p[i].position = sf::Vector2f (i, ycoord);
			p[i].color = sf::Color::White;
		}
		mainwin.draw(p);
		mainwin.display();
	}
	Pa_StopStream(stream);
	Pa_CloseStream(stream);
	Pa_Terminate();

	for(int i = 0; i < sizeof(insts)/sizeof(insts[0]); i++){
		delete insts[i];
	}
	
	for(int i = 0; i < chnum; i++){
		delete chs->at(i);
	}
	delete chs;
	delete params;

	delete[] recentArr;

	return 0;
}