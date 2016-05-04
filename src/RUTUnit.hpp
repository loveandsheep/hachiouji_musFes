//
//  RUTUnit.hpp
//  brdg_RUT
//
//  Created by Ovis aries on 2015/09/25.
//
//

#ifndef RUTUnit_hpp
#define RUTUnit_hpp

#include "ofMain.h"
#include "ofxPDF.h"
#include "ofxKsmrRibbonAnimator_Sheep.hpp"

typedef ofxKsmrRibbonAnimator_Sheep ribbonAnimator;

class RUTUnit{
public:
	void setup(ofxKsmrEaseKeyFrame *easeKeyFrames, vector< ofPtr<ofxPDF> > paths);
	void update();
	void draw();
	
	void shufflePattern();
	
	void play();
	void playReverse();
	void stop();
	void pause(bool b);
	void setPosition(float pct);
	
	ofxKsmrEaseKeyFrame* ease;
	vector< ofPtr<ribbonAnimator> >  Unit[64];
	vector< ofPtr<ribbonAnimator> > *currentNum;
};

#endif /* RUTUnit_hpp */
