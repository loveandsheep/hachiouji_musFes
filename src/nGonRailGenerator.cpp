//
//  nGonRailGenerator.cpp
//  brdg_RUT
//
//  Created by Ovis aries on 2015/10/05.
//
//

#include "nGonRailGenerator.hpp"

void nGonRailGenerator::addVertex(float deg, float rad)
{
	if (degs.size() > 0 && rads.size() > 0)
	{
		if (deg != degs.back() && rad != rads.back())
		{
			addVertex(deg, rads.back());
		}
	}
	
	float th = ofDegToRad(deg + 90);
	float thp = ofDegToRad(deg);
	float r = cos(PI / nGon) / cos(th - TWO_PI / nGon * floor((nGon * th + PI) / TWO_PI));
	
	degs.push_back(deg);
	rads.push_back(rad);
	verts.push_back(ofVec2f(cos(thp) * r * rad,
							sin(thp) * r * rad));
	
}

void nGonRailGenerator::clear()
{
	verts.clear();
	degs.clear();
	rads.clear();
}

void nGonRailGenerator::genRail(vector<ofVec3f> *ptDst1, vector<ofVec3f> *ptDst2, float width)
{
	if (verts.size() == 0) return;
	
	ptDst1->clear();
	ptDst2->clear();
	
	ofVec2f no = ofVec2f(0,0); //始点
	ofVec2f dir = verts[0].normalized(); //始点から終点への方向
	ofVec2f wg = dir.rotated(90); //羽の方向
	float dst = verts[0].distance(no); //始点から終点までの距離
	ofVec2f nwg = (verts[1] - verts[0]).rotated(90, ofVec3f(0,0,1)).normalized() * width;//(verts[1] - verts[0]).normalized() * width + (no - verts[0]).normalized() * width;//終点の羽方向
	
	ptDst1->push_back(verts[0] + nwg);
	ptDst2->push_back(verts[0] - nwg);
	
	for (int i = 0;i < verts.size() - 1;i++)
	{
		no	= verts[i];
		dir = (verts[i+1] - verts[i]).normalized();
		dst = verts[i+1].distance(verts[i]);
		wg	= nwg;
		if (i == verts.size() - 2)
		{
			nwg = (verts[i+1] - verts[i]).normalized().rotated(90.0, ofVec3f(0,0,1)) * width;
		}else{
			nwg = (verts[i+2] - verts[i+1]).normalized() * width + (verts[i] - verts[i+1]).normalized() * width;
		}

		ofVec3f tg_a = verts[i+1] + nwg;
		ofVec3f tg_b = verts[i+1] - nwg;
		ofVec3f me_a = ptDst1->back();
		ofVec3f me_b = ptDst2->back();
		
		float dot_a = (tg_a - me_a).normalized().dot((tg_b - me_b).normalized());
		float dot_b = (tg_a - me_b).normalized().dot((tg_b - me_a).normalized());
		
		if (dot_a < dot_b) swap(tg_a, tg_b);
		
		for (float j = 0;j <= dst;j+=3)
		{
			ptDst1->push_back(me_a.interpolated(tg_a, j / dst));
			ptDst2->push_back(me_b.interpolated(tg_b, j / dst));
		}
		ptDst1->push_back(tg_a);
		ptDst2->push_back(tg_b);
	}
}