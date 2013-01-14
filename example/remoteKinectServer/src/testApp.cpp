#include "testApp.h"

#include <sstream>

static const char kNearClip[]  = "rk_nc";
static const char kFarClip[]   = "rk_fc";
static const char kTiltAngle[] = "rk_ta";
static const char kQuality[]   = "rk_qu";

//--------------------------------------------------------------
testApp::testApp(int publisherPort, int responderPort) {
	nearClip  = server.getNearClip();
	farClip   = server.getFarClip();
	tiltAngle = server.getTiltAngle();
	quality   = server.getQuality();

	server.setPorts(publisherPort, responderPort);
}

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	ofEnableSmoothing();
	
	gui.setup("Server", ofGetWidth() - 260, 5, 255, 400);
	gui.addPanel("Settings", 0);
	gui.addSlider("Near Clip [mm]",
		      kNearClip,
		      nearClip,
		      0.0,
		      10000.0,
		      false);
	gui.addSlider("Far clip [mm]",
		      kFarClip,
		      farClip,
		      0.0,
		      10000.0,
		      false);
	gui.addSlider("Tilt Angle [deg]",
		      kTiltAngle,
		      tiltAngle,
		      -30.0,
		      30.0,
		      false);
	gui.addSlider("Quality",
		      kQuality,
		      quality,
		      0,
		      4,
		      true);

	server.setup();
}

//--------------------------------------------------------------
void testApp::update(){
	gui.update();
	
	// Reflect settings from GUI to ofxRemoteKinect.
	float newNearClip  = gui.getValueF(kNearClip);
	float newFarClip   = gui.getValueF(kFarClip);
	float newTiltAngle = gui.getValueF(kTiltAngle);
	int newQuality     = gui.getValueI(kQuality);
	if (nearClip != newNearClip || farClip != newFarClip) {
		nearClip = newNearClip;
		farClip  = newFarClip;
		server.setClip(nearClip, farClip);
	}
	if (tiltAngle != newTiltAngle) {
		tiltAngle = newTiltAngle;
		server.setTiltAngle(tiltAngle);
	}
	if (quality != newQuality) {
		quality = newQuality;
		server.setQuality(quality);
	}

	// Client requests may change server settings.
	server.update();
	
	// Reflect settings from ofxRemoteKinect to GUI.
	gui.setValueF(kNearClip, server.getNearClip());
	gui.setValueF(kFarClip, server.getFarClip());
	gui.setValueF(kTiltAngle, server.getTiltAngle());
	gui.setValueF(kQuality, server.getQuality());
}

//--------------------------------------------------------------
void testApp::draw(){
	server.draw();
	gui.draw();
	
	// Show framerate.
	stringstream ss;
	ss << "FPS: " << ofGetFrameRate();
	ofDrawBitmapString(ss.str(), 900, 20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
}