OBJS = wavegen.cpp main.cpp channel.cpp instrument.cpp
OBJS_TRACK = wavegen.cpp track.cpp channel.cpp instrument.cpp
OBJ_NAME = synth
OBJ_TRACK_NAME = track

LIBRARIES = -lmingw32 -lportaudio -lsfml-main -lsfml-window -lsfml-system -lsfml-graphics 
OPTIONS = -w -Wall -std=c++11 -g

INCLUDE_PATHS = -I..\Libraries\include
LIBRARY_PATHS = -L..\Libraries\lib

all : $(OBJS)
	g++ $(OBJS) $(OPTIONS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBRARIES) -o $(OBJ_NAME)
track : 
	g++ $(OBJS_TRACK) $(OPTIONS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LIBRARIES) -o $(OBJ_TRACK_NAME)