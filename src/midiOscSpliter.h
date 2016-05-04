//
//  midiOscSpliter.h
//  hachiouji_video
//
//  Created by Ovis aries on 2016/05/05.
//
//

#ifndef midiOscSpliter_hpp
#define midiOscSpliter_hpp

#include "ofxOsc.h"
#include "ofxMidi.h"

class midiOscSpliter : public ofxMidiOut{
public:
	
	ofxOscSender sender;
	
	void setupOsc(string addr, int port);
	void sendNoteOnOsc(int channel, int pitch, int velocity=64);
	void sendNoteOffOsc(int channel, int pitch, int velocity=0);
};

#endif /* midiOscSpliter_hpp */
