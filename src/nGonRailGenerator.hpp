//
//  nGonRailGenerator.hpp
//  brdg_RUT
//
//  Created by Ovis aries on 2015/10/05.
//
//

#ifndef nGonRailGenerator_hpp
#define nGonRailGenerator_hpp

#include "ofMain.h"

class nGonRailGenerator{
public:
	nGonRailGenerator()
	{
		nGon = 6;
	}
	void clear();
	void addVertex(float deg, float rad);
	void genRail(vector<ofVec3f> *ptDst1, vector<ofVec3f> *ptDst2, float width);
	
	int nGon;
	vector<ofVec3f> verts;
	vector<float> degs,rads;
};

#endif /* nGonRailGenerator_hpp */
