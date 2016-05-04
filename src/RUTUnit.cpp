//
//  RUTUnit.cpp
//  brdg_RUT
//
//  Created by Ovis aries on 2015/09/25.
//
//

#include "RUTUnit.hpp"

void RUTUnit::setup(ofxKsmrEaseKeyFrame *easeKeyFrames, vector< ofPtr<ofxPDF> > paths)
{
	for (int i = 0;i < paths.size();i++)
	{
		for (int j = 0;j < paths[i]->getNumPath() / 2;j++)
		{
			ofPath path1 = paths[i]->getPathAt(j*2);
			ofPath path2 = paths[i]->getPathAt(j*2+1);
			path1.translate(ofPoint(-paths[i]->getWidth() / 2.0, -paths[i]->getHeight() / 2.0));
			path2.translate(ofPoint(-paths[i]->getWidth() / 2.0, -paths[i]->getHeight() / 2.0));
			
			Unit[i].push_back(ofPtr<ribbonAnimator>(new ribbonAnimator(easeKeyFrames)));
			Unit[i][j]->setRail(path1, path2);
		}
	}
	
	currentNum = &Unit[0];
}

void RUTUnit::update()
{
	for (auto anim : *currentNum) anim->update();
}

void RUTUnit::draw()
{
	for (auto anim : *currentNum) anim->drawMotion();
}

void RUTUnit::shufflePattern()
{
	for (auto anim : *currentNum)
	{
		
		if (anim->getRailAngle() < 45)
		{
			string choise[] = {"simple", "blink", "slideIn", "slideVert"};
			anim->setPattern(choise[int(ofRandom(100)) % 1]);
		}
		else if (anim->getRailAngle() < 90) //under 90 (Quat Round)
		{
			string choise[] = {"simple", "blink"};
			anim->setPattern(choise[int(ofRandom(100)) % 1]);
		}
		else
		{
			string choise[] = {"simple","simple","simple", "blink"};
			anim->setPattern(choise[int(ofRandom(100)) % 1]);
		}
	}
}

void RUTUnit::play()		{ for (auto anim : *currentNum) anim->play(); }
void RUTUnit::playReverse(){ for (auto anim : *currentNum) anim->playReverse(); }
void RUTUnit::stop()		{ for (auto anim : *currentNum) anim->stop(); }
void RUTUnit::pause(bool b){ for (auto anim : *currentNum) anim->pause(b); }
void RUTUnit::setPosition(float pct) { for (auto anim : *currentNum) anim->setPosition(pct); }