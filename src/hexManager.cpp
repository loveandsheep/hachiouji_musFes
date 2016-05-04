//
//  hexManager.cpp
//  brdg_RUT
//
//  Created by Ovis aries on 2015/10/05.
//
//

#include "hexManager.hpp"

void hexManager::setup()
{
	value_hexGrid = 0;
	value_patterns = 0;
	
	midiOut = ofPtr<midiOscSpliter>(new midiOscSpliter);
	((midiOscSpliter*)(midiOut.get()))->setupOsc("192.168.10.8", 12400);
//	midiOut->openVirtualPort("ribbonMidi");
	midiOut->openPort(0);
	
	scale_Ionian.push_back(N_C);
	scale_Ionian.push_back(N_D);
	scale_Ionian.push_back(N_E);
	scale_Ionian.push_back(N_F);
	scale_Ionian.push_back(N_G);
	scale_Ionian.push_back(N_A);
	scale_Ionian.push_back(N_B);
}

void hexManager::update()
{
	
	float moriagari = (ofGetMouseX() / float(ofGetWidth()));
	
	if ((ofGetFrameNum() % 3 == 0) && (ofRandomuf() < value_hexGrid * 2)) addGrid();
	
	if (ofRandomuf() < value_patterns)
	{
		if (ofGetFrameNum() % 3 == 0) addNewPattern();
	}
	
	for (auto it : ribbons) it->update();
	
	vector<ofPtr<ribbon> >::iterator ite = ribbons.begin();
	while (ite != ribbons.end()) {
		if ((*ite)->getPosition() == 1.0)
		{
			ite = ribbons.erase(ite);
		}else ++ite;
	}
}

void hexManager::draw()
{
	toggle_cameraUp = ofGetKeyPressed('c');
	
	ofPushMatrix();
	ofTranslate(ofNoise(ofGetFrameNum() / 70.0) * 30,
				ofNoise(ofGetFrameNum() / 94.1) * 40);
	
	for (auto it : ribbons) {
		ofPushMatrix();
		it->drawMotion();
		
		ofPopMatrix();
	}
	
	ofPushMatrix();
	ofSetCircleResolution(3);
	ofNoFill();
	ofCircle(0, 0, 30);
	ofFill();
	ofLine(-5, 0, 5, 0);
	ofLine( 0, -5, 0, 5);
	ofPopMatrix();
	
	ofPopMatrix();

	ofSetColor(255);
}

void hexManager::addGrid()
{
	railgen.clear();
	float base = int(ofRandom(6)) * 60;
	for (int i = 0;i < 3;i++)
	{
		railgen.addVertex(ofRandom(60,120) + base, ofRandom(30,800));
	}
	railgen.genRail(&rail1, &rail2, 1.0 + powf(ofRandomuf(), 8.0) * 5.0);
	
	ribbons.push_back(ofPtr<ribbon>(new ribbon(&frame, midiOut)));
	ribbons.back()->setRail(rail1, rail2);
	ribbons.back()->setColor(ofFloatColor(1.0,0.0,0.0));
	ribbons.back()->setPattern("chase");
	ribbons.back()->ch = 6;
	ribbons.back()->offset = 60;
	ribbons.back()->setDuration(0.9);
	ribbons.back()->play();
}

void hexManager::addNewPattern()
{
//	for (int i = 0;i < 128;i++)
//	{
//		midiOut.sendNoteOff(0, i);
//	}
	if (ofRandomuf() < 0.5){
	
		pattern_thinNgon();
	
	}else if (ofRandomuf() < 0.5){
		
		pattern_square();
		
	}else if (ofRandomuf() < 0.5){
		
		pattern_circle();
		
	}else if (ofRandomuf() < 0.5){
		
		pattern_bigRibbon();
		
	}else{
		pattern_Slice();
	}
	
}

void hexManager::pattern_Slice()
{
	rail1.clear();
	rail2.clear();
	float wd = 1000;
	bool dir_x = ofRandomuf() < 0.5;
	bool dir_y = ofRandomuf() < 0.5;
	
	for (int i = -1000; i <= 1000; i+=10)
	{
		rail1.push_back(ofVec2f(i * (dir_x ? -1.0 : 1.0) + wd,
								(i * (dir_y ? -1.0 : 1.0) - wd) * -1.0));
		
		rail2.push_back(ofVec2f(i * (dir_x ? -1.0 : 1.0) - wd ,
								(i * (dir_y ? -1.0 : 1.0) + wd) * -1.0));
	}
	
	ribbons.push_back(ofPtr<ribbon>(new ribbon(&frame, midiOut)));
	ribbons.back()->setRail(rail1, rail2);
	ribbons.back()->setColor(ofFloatColor(1.0,0.0,0.0));
	ribbons.back()->setPattern("chase");
	ribbons.back()->ch = 5;
	ribbons.back()->offset = 24;
	ribbons.back()->setDuration(0.5);
	ribbons.back()->play();
}

void hexManager::pattern_thinNgon()
{
	rail1.clear();
	rail2.clear();
	float scl = ofRandom(100,500);
	int n = 4;
	if (ofRandomuf() < 0.2) n = 3;
	if (ofRandomuf() < 0.2) n = 6;
	bool dir = ofRandomuf() < 0.5;
	ofVec2f wid = ofVec2f(ofRandomf() * value_widen, 0);
	
	for (int i = 0;i <= ofRandom(360,1080);i++)
	{
		rail1.push_back(wid + ofVec2f(getNgon(i * (dir ? -1.0 : 1.0), scl, n)));
		rail2.push_back(wid + ofVec2f(getNgon(i * (dir ? -1.0 : 1.0), scl - 5, n)));
	}
	ribbons.push_back(ofPtr<ribbon>(new ribbon(&frame, midiOut)));
	ribbons.back()->setRail(rail1, rail2);
	ribbons.back()->setPattern("chase");
	if (ofRandomuf() < 0.2) ribbons.back()->setPattern("blink");
	ribbons.back()->ch = 4;
	ribbons.back()->offset = 48;
	ribbons.back()->setDuration(0.7);
	ribbons.back()->play();
}

void hexManager::pattern_bigRibbon()
{
	rail1.clear();
	rail2.clear();
	float wd = 10;
	ofVec2f offs = ofVec2f(ofRandomf() * 600, ofRandomf() * 600);
	for (int i = -600; i <= 600; i+=10)
	{
		rail1.push_back(ofVec2f(i + wd, (i - wd) * -1.0) + offs);
		rail2.push_back(ofVec2f(i - wd, (i + wd) * -1.0) + offs);
	}
	
	ribbons.push_back(ofPtr<ribbon>(new ribbon(&frame, midiOut)));
	ribbons.back()->setRail(rail1, rail2);
	ribbons.back()->setColor(ofFloatColor(1.0,0.0,0.0));
	ribbons.back()->setPattern("chase");
	if (ofRandomuf() < 0.5) ribbons.back()->setPattern("simple");
	if (ofRandomuf() < 0.2) ribbons.back()->setPattern("blink");
	ribbons.back()->ch = 3;
	ribbons.back()->offset = 32;
	ribbons.back()->setDuration(0.7);
	ribbons.back()->play();
}

void hexManager::pattern_square()
{
	for (int i = -2;i <= 2;i++)
	{
		rail1.clear();
		rail2.clear();
		
		int length = 50;
		int dir = (ofRandomuf() < 0.5 ? 1.0 : -1.0);
		ofVec2f wid = ofVec2f(ofRandomf() * value_widen, 0);
		
		for (int j = -length ;j <= length;j+=5)
		{
			rail1.push_back(wid + ofVec2f(i * 20 - 5, j * dir));
			rail2.push_back(wid + ofVec2f(i * 20 + 5, j * dir));
		}
		ribbons.push_back(ofPtr<ribbon>(new ribbon(&frame, midiOut)));
		ribbons.back()->setRail(rail1, rail2);
		ribbons.back()->setPattern("Stripe");
		if (ofRandomuf() < 0.5) ribbons.back()->setPattern("simple");
		if (ofRandomuf() < 0.2) ribbons.back()->setPattern("blink");
		ribbons.back()->ch = 2;
		ribbons.back()->offset = 48;
		ribbons.back()->setDuration(ofRandom(0.3,0.9));
		ribbons.back()->play();
		
	}

}

void hexManager::pattern_circle()
{
	rail1.clear();
	rail2.clear();
	
	float rd = ofRandom(10, 600);
	float dg = ofRandomuf() < 0.5 ? 1.0 : -1.0;
	
	int gon = 3;
	
	ofVec2f wid = ofVec2f(ofRandomf() * value_widen, 0);
	
	for (int i = 0;i <= ofRandom(360,720);i+=5)
	{
		rail1.push_back(ofVec2f(sin(dg * ofDegToRad(i + rd)) * rd,
								cos(dg * ofDegToRad(i + rd)) * rd) + wid);
		rail2.push_back(ofVec2f(sin(dg * ofDegToRad(i + rd)) * rd * 0.92,
								cos(dg * ofDegToRad(i + rd)) * rd * 0.92) + wid);
//		rail1.push_back(getNgon(dg * (i + rd), rd, gon));
//		rail2.push_back(getNgon(dg * (i + rd), rd * 0.9, gon));
		
	}
	ribbons.push_back(ofPtr<ribbon>(new ribbon(&frame, midiOut)));
	ribbons.back()->setRail(rail1, rail2);
	ribbons.back()->setPattern("chase");
	if (ofRandomuf() < 0.2) ribbons.back()->setPattern("blink");
	ribbons.back()->ch = 1;
	ribbons.back()->offset = 32;
	ribbons.back()->setDuration(0.7);
	ribbons.back()->play();
}


ofVec2f hexManager::getNgon(float deg, float rad, int nGon)
{
	float th = ofDegToRad(deg + 90);
	float thp = ofDegToRad(deg);
	float r = cos(PI / nGon) / cos(th - TWO_PI / nGon * floor((nGon * th + PI) / TWO_PI));

	return ofVec2f(cos(thp) * r * rad,
				   sin(thp) * r * rad);
}