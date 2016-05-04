//
//  hexManager.hpp
//  brdg_RUT
//
//  Created by Ovis aries on 2015/10/05.
//
//

#ifndef hexManager_hpp
#define hexManager_hpp

#include "ofMain.h"
#include "ofxKsmrEaseKeyFrame.hpp"
#include "ofxKsmrRibbonAnimator_Sheep.hpp"
#include "nGonRailGenerator.hpp"
#include "ofxMidiOut.h"
#include "midiOscSpliter.h"

#define N_C  0
#define N_CS 1
#define N_D	 2
#define N_DS 3
#define N_E  4
#define N_F  5
#define N_FS 6
#define N_G  7
#define N_GS 8
#define N_A  9
#define N_AS 10
#define N_B  11

typedef ofxKsmrRibbonAnimator_Sheep ribbon;

class hexManager{
public:
	
	void setup();
	void update();
	void draw();
	
	void addNewPattern();
	void addGrid();
	
	void pattern_square();
	void pattern_circle();
	void pattern_bigRibbon();
	void pattern_thinNgon();
	void pattern_Slice();
	
	ofxKsmrEaseKeyFrame frame;
	nGonRailGenerator railgen;
	vector< ofPtr<ribbon> > ribbons;
	vector<ofVec3f> rail1,rail2;
	
	ofVec2f getNgon(float deg, float rad, int nGon);
	
	bool toggle_cameraUp;
	
	float value_hexGrid;
	float value_patterns;
	
	float value_widen;
	
	ofPtr<midiOscSpliter> midiOut;
	
	vector<int> scale_Ionian;
};

#endif /* hexManager_hpp */
