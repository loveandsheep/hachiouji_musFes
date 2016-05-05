//
//  FragFXUnits_mosaic.h
//  XYMode
//
//  Created by Ovis aries on 2015/08/20.
//
//

#ifndef XYMode_FragFXUnits_mosaic_h
#define XYMode_FragFXUnits_mosaic_h

#include "ofxKsmrFragFXUnit.h"

class FragFXUnits_Mosaic : public ofxKsmrFragFXUnit{
	
public:
	virtual void setup(){
		Name = "Mosaic";
		u_Volume = 1.0;
		
		source = STRINGIFY(
						   uniform float Volume;
						   uniform float Phase;
						   uniform sampler2DRect tex0;
						   
						   void main(void){
							   vec2 st = gl_TexCoord[0].st;
							   vec4 col;
							   
							   float sz = max(1.0,Volume);
							   
							   col = texture2DRect(tex0, vec2(int(st.x / sz) * int(sz),
															  int(st.y / sz) * int(sz)));
							   
							   gl_FragColor = col;
						   }
						   
						   );
	}
};


#endif
