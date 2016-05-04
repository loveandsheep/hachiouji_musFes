#pragma once

#include "ofMain.h"
#include "hexManager.hpp"
#include "ofxKsmrFragmentFx.h"
#include "ofxOsc.h"
#include "ofxMidi.h"
#include "ofxGui.h"

#include "ofxSequenceExport.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

public:
	void setup();
	void update();
	void draw();
	
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void newMidiMessage(ofxMidiMessage& eventArgs);
	
	ofxSequenceExport expo;
	
	ofEasyCam camera;
	
	hexManager hex;

	ofxKsmrFragmentFx fx;
	ofFbo mainBuf;
	
	ofxOscReceiver receiver;
	ofxOscReceiver soundRec;
	
	ofxPanel gui;
	ofParameter<float> value_fx;
	ofParameter<bool> toggle_flash;
	ofParameter<float> value_hexGrid;
	ofParameter<float> value_patterns;
	ofParameter<float> value_master;
	ofParameter<float> value_widen;

	ofParameter<ofColor> color;
	ofParameter<float> colordensity;
	
	bool midiReflesh;
	float midiVal_value_fx;
	float midiVal_value_hexGrid;
	float midiVal_value_patterns;
	float midiVal_value_master;
	float midiVal_value_widen;
	bool midiToggle_flash;
	
	ofxMidiIn midiIn;
	
	float paekAmp;
	float snapshot;
};
