#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofSetFrameRate(30);
	hex.setup();
	
	ofFbo::Settings settings;
	settings.width = 1920;
	settings.height = 1080;
	settings.internalformat	= GL_RGB;
	settings.useDepth		= true;
	settings.useStencil		= false;
	settings.textureTarget	= ofGetUsingArbTex() ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
	settings.depthStencilInternalFormat = GL_DEPTH_COMPONENT16;
	settings.depthStencilAsTexture = false;
	settings.minFilter = GL_LINEAR;
	settings.maxFilter = GL_LINEAR;
	settings.numSamples = 8;
	
	camera.setDistance(800);
	
	mainBuf.allocate(settings);
	fx.setup(&mainBuf, settings);
	
	receiver.setup(12400);
	soundRec.setup(11000);
	
	
	midiIn.listPorts();
	midiIn.openPort("Launch Control");
	midiIn.addListener(this);
	
	gui.setup();
	gui.add(value_hexGrid.set("hexGrid", 0.0, 0.0, 1.0));
	gui.add(value_patterns.set("pattern", 0.0, 0.0, 1.0));
	gui.add(value_fx.set("effects", 0.0, 0.0, 1.0));
	gui.add(value_widen.set("widen", 0.0, 0.0, 1.0));
	gui.add(value_master.set("master", 1.0, 0.0, 1.0));
	gui.add(toggle_flash.set(false));
	gui.add(color.set("color", ofColor(255,255,255), ofColor(0,0,0),ofColor(255,255,255)));
	gui.add(colordensity.set("density", 1.0, 0.0, 1.0));
	
	value_fx = 0.0;
	
	camera.disableMouseInput();
	
	expo.setup("rendering/render", "png");
}

//--------------------------------------------------------------
void ofApp::update(){
	
	while (soundRec.hasWaitingMessages())
	{
		ofxOscMessage m;
		soundRec.getNextMessage(&m);
//		cout << ofGetFrameNum() << ":" << m.getAddress() << endl;
	}
	
	while (receiver.hasWaitingMessages()) {
		
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		
		if (m.getAddress() == "/fx/sw")
		{
			ofxKsmrFragFXUnit* fxUn;
			fxUn = fx.getfxUnit(KsmrFragFx_Type(m.getArgAsInt32(0)));
			fxUn->bEnable = m.getArgAsInt32(1);
		}
		if (m.getAddress() == "/fx/val")
		{
			ofxKsmrFragFXUnit* fxUn;
			fxUn = fx.getfxUnit(KsmrFragFx_Type(m.getArgAsInt32(0)));
			fxUn->u_Volume = m.getArgAsFloat(1);
		}
		
	}
	
	/*なんかイベントで落ちる？ので退避*/
	if (midiReflesh)
	{
		value_fx = midiVal_value_fx;
		value_hexGrid = midiVal_value_hexGrid;
		value_patterns = midiVal_value_patterns;
		value_master = midiVal_value_master;
		value_widen = midiVal_value_widen;
		toggle_flash = midiToggle_flash;
		midiReflesh = true;
	}else{
		midiVal_value_fx = value_fx;
		midiVal_value_hexGrid = value_hexGrid;
		midiVal_value_patterns = value_patterns;
		midiToggle_flash = toggle_flash;
		midiVal_value_widen = value_widen;
		midiVal_value_master = value_master;
	}
	
	hex.value_widen = value_widen * 600.0;
	hex.value_hexGrid = value_hexGrid;
	hex.value_patterns = value_patterns;
	hex.update();
	
	if (value_fx > 0.0)
	{
		if (ofRandomuf() < value_fx * 0.3)
		{
			fx.getfxUnit(KsmrFragFx_Type(int(ofRandom(10))))->bEnable = true;
			fx.getfxUnit(KsmrFragFx_Type(int(ofRandom(10))))->bEnable = true;
			fx.getfxUnit(KsmrFragFx_Type(int(ofRandom(10))))->bEnable = true;
			
			fx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable = fx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable && toggle_flash;
		}

		if (ofRandomuf() < 0.1)
		{
			for (int i = 0;i < 10;i++)
				fx.getfxUnit(KsmrFragFx_Type(i))->bEnable = false;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0, 0, 0);

	ofSetColor(255);
	
	ofPushStyle();
	mainBuf.begin();
	ofClear(0, 0, 0, 255);
	camera.begin();
	hex.draw();
	camera.end();
	mainBuf.end();

	ofSetColor(255);
	mainBuf.draw(300, 720, 640, 360);
	
	fx.applyFx();

	mainBuf.draw(300, 0, 1280, 720);
	ofNoFill();
	ofSetColor(255, value_master * 255.0, value_master * 255);
	ofRect(300, 0, 1280, 720);
	ofFill();
	
	ofSetColor(255, 255 * value_master);
	mainBuf.draw(1920, 0, 1920, 1080);
	
	/*=== 画像エクスポート ===*/
	if (expo.running())
	{
		expo.addQue(&mainBuf);
		if (ofGetFrameNum() % 30 < 15)
		{
			ofSetColor(255, 0, 0);
			ofDrawBitmapString("Rec", ofGetWidth()-50, 30);
			ofSetColor(255);
		}
	}
	
	ofSetColor(255);
	gui.draw();
	
	
	static float elapsed = ofGetElapsedTimef();
	static float stopwtch = ofGetElapsedTimef();
	elapsed = ofGetElapsedTimef();
	if (ofGetKeyPressed(' ')) stopwtch = ofGetElapsedTimef();
	
	string info;
	info += "fps      :" + ofToString(ofGetFrameRate()) + "\n";
	info += "elapsed  :" + ofToString(ofGetElapsedTimef()) + "\n";
	info += "stopWtch :" + ofToString(elapsed - stopwtch) + "\n\n ";
	info += "Time     :" + ofToString(ofGetHours(), 2) + ":" + ofToString(ofGetMinutes(), 2) + ":" + ofToString(ofGetSeconds(), 2) + "\n";
	
	ofPushMatrix();
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofTranslate(990, 1000);
	glScaled(3.0, 3.0, 3.0);
	ofDrawBitmapString(info, 0, 0);
	ofPopMatrix();
	
	ofPushMatrix();
	ofTranslate(ofGetMouseX(), ofGetMouseY());
	ofLine(-5, 0, 5, 0);
	ofLine(0, -5, 0, 5);
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key == 'f') ofToggleFullscreen();
	
	if (key == 'r') {
		if (expo.running())
		{
			expo.stop();
		}
		else expo.start();
	}
}

void ofApp::newMidiMessage(ofxMidiMessage &eventArgs)
{
	midiReflesh = true;
//	cout << "=== MidiIn ===" << endl;
//	for (int i = 0;i < eventArgs.bytes.size();i++)
//	{
//		cout << int(eventArgs.bytes[i]) << endl;
//	}
	
	if (eventArgs.bytes[1] == 41) midiVal_value_hexGrid = eventArgs.bytes[2] / 127.0;
	if (eventArgs.bytes[1] == 42) midiVal_value_patterns= eventArgs.bytes[2] / 127.0;
	if (eventArgs.bytes[1] == 43) midiVal_value_fx = eventArgs.bytes[2] / 127.0;
	if (eventArgs.bytes[1] == 44) midiVal_value_widen = eventArgs.bytes[2] / 127.0;
	if (eventArgs.bytes[1] == 48) midiVal_value_master = eventArgs.bytes[2] / 127.0;
	
	if (eventArgs.bytes[1] ==  9)
		fx.getfxUnit(KSMR_FRAGFX_FRINGE)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  10)
		fx.getfxUnit(KSMR_FRAGFX_NOISE)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  11)
		fx.getfxUnit(KSMR_FRAGFX_VERTGLITCH)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  12)
		fx.getfxUnit(KSMR_FRAGFX_VERTNOISE)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  25)
		fx.getfxUnit(KSMR_FRAGFX_SLANTSHIFT)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  26)
		fx.getfxUnit(KSMR_FRAGFX_WATER)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  27)
		fx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->bEnable = eventArgs.bytes[2] == 127;
	if (eventArgs.bytes[1] ==  28)
		fx.getfxUnit(KSMR_FRAGFX_INVERT)->bEnable = eventArgs.bytes[2] == 127;
	
	if (eventArgs.bytes[1] == 21) midiToggle_flash = eventArgs.bytes[2] > 0;
	
	if (eventArgs.bytes[1] == 22)
	{
		fx.getfxUnit(KSMR_FRAGFX_WATER)->u_Volume = eventArgs.bytes[2] / 12.0;
		fx.getfxUnit(KSMR_FRAGFX_VERTSLIDE)->u_Volume = eventArgs.bytes[2] / 127.0;
		fx.getfxUnit(KSMR_FRAGFX_MOSAIC)->u_Volume = eventArgs.bytes[2] / 127.0;
		fx.getfxUnit(KSMR_FRAGFX_FRINGE)->u_Volume = eventArgs.bytes[2] / 6400.0;
		fx.getfxUnit(KSMR_FRAGFX_NOISE)->u_Volume = eventArgs.bytes[2] / 64.0;
	}
	
}

void ofApp::exit()
{
	expo.close();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
