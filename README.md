# synth
A synthesizer written in C++

## Installation
`synth` depends on PortAudio and SFML (don't ask why I used such a bulky library when SDL would've done just fine). This means that you'll need to install these two libraries before making them. All PortAudio headers should go in `Libraries/Include/portaudio`, and all SFML headers should go in `Libraries/Include/SFML`. The library files themselves should go in `Libraries/lib`.

To make `synth`, simply go into `src` and run `make`. If you want to make `track`, run `make track`. Please note that for some dumb reason, I developed `synth` on a Windows box, so the `Makefile` in `src` uses Windows path syntax (with backslashes and whatnot). Additionally, when building on Windows, MinGW must be installed, `gcc.exe` must be located in your `PATH`, and you must link with `lmingw32`. If you're on a Mac or a Linux system, you should remove `-lmingw32` from the `Makefile` and change the paths to use Unix syntax.

For your convenience, I've included a Windows binary in the `obj` directory, along with a bunch of dlls that I think I have permission to redistribute.

## Usage
`synth` takes no arguments. It presents the user with a GUI-based synthesizer with an oscilloscope view. To play notes, simply press keys on the keyboard. You can cycle through a selection of instruments with the up and down arrow keys, you can change octaves using - and Shift+=, and you can adjust the duty cycle on the square wave instrument by using the left and right arrow keys.

`track` takes two arguments: an input file and a decimal number. `track` reads the input file and plays the notes specified by the file. The second argument determines the number of seconds per line. Each line in a `track` file can contain at most one note. A note is specified by two integers separated by a dash. The first integer, between 0 and 11, determines which pitch class the note belongs to. For example, 0 would represent C, 1 would represent C# (or Db), and so on. The second integer determines the octave that the note belongs to. Negative octaves are permissible. Alternatively, a line may consist solely of the symbol `=`, denoting a "note off" command that silences the current instrument. Finally, any line can end in a comment, which begins with a `#`. Example files for `track` are given in `obj/Examples`. I've found that they play best when the number of seconds per line is around 0.78.

## Licensing
Please note that both `synth` and `track` use SFML. Shoutouts to those people.

Additionally, `synth` and `track` both use PortAudio. Here's the license:

PortAudio Portable Real-Time Audio Library
Copyright (c) 1999-2011 Ross Bencina and Phil Burk

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

