//
//  midiOscSpliter.cpp
//  hachiouji_video
//
//  Created by Ovis aries on 2016/05/05.
//
//

#include "midiOscSpliter.h"

void midiOscSpliter::setupOsc(string addr, int port)
{
	sender.setup(addr, port);
}

void midiOscSpliter::sendNoteOnOsc(int channel, int pitch, int velocity)
{
	ofxOscMessage m;
	m.setAddress("/note/on");
	m.addIntArg(channel);
	m.addIntArg(pitch);
	m.addIntArg(velocity);
	sender.sendMessage(m);
	ofxMidiOut::sendNoteOn(channel, pitch, velocity);
}

void midiOscSpliter::sendNoteOffOsc(int channel, int pitch, int velocity)
{
	ofxOscMessage m;
	m.setAddress("/note/off");
	m.addIntArg(channel);
	m.addIntArg(pitch);
	m.addIntArg(velocity);
	sender.sendMessage(m);ofxMidiOut::sendNoteOff(channel, pitch, velocity);
}