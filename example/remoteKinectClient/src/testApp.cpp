#include "testApp.h"

#include <sstream>

static const char kNearClip[]  = "rk_nc";
static const char kFarClip[]   = "rk_fc";
static const char kTiltAngle[] = "rk_ta";
static const char kQuality[]   = "rk_qu";

//--------------------------------------------------------------
testApp::testApp(const string& publisher, const string& responder) : publisher(publisher), responder(responder) {
	nearClip  = client.getNearClip();
	farClip   = client.getFarClip();
	tiltAngle = client.getTiltAngle();
	quality   = client.getQuality();
	
	mode = 0;
}

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofBackground(0, 0, 0);
	
	gui.setup("Client", ofGetWidth() - 260, 5, 255, 400);
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
	gui.loadSettings("settings.xml");
	
	client.setServers(publisher, responder);
	client.setup();

	client.setClip(nearClip, farClip);
	client.setTiltAngle(tiltAngle);
	client.setQuality(quality);
}

//--------------------------------------------------------------
void testApp::update() {
	gui.update();
	
	// Reflect settings from GUI to ofxRemoteKinect.
	float newNearClip  = gui.getValueF(kNearClip);
	float newFarClip   = gui.getValueF(kFarClip);
	float newTiltAngle = gui.getValueF(kTiltAngle);
	int newQuality     = gui.getValueI(kQuality);
	if (nearClip != newNearClip || farClip != newFarClip) {
		nearClip = newNearClip;
		farClip  = newFarClip;
		client.setClip(nearClip, farClip);
	}
	if (tiltAngle != newTiltAngle) {
		tiltAngle = newTiltAngle;
		client.setTiltAngle(tiltAngle);
	}
	if (quality != newQuality) {
		quality = newQuality;
		client.setQuality(quality);
	}
	
	client.update();
	
	// Reflect settings from ofxRemoteKinect to GUI.
	gui.setValueF(kNearClip, client.getNearClip());
	gui.setValueF(kFarClip, client.getFarClip());
	gui.setValueF(kTiltAngle, client.getTiltAngle());
	gui.setValueF(kQuality, client.getQuality());
}

//--------------------------------------------------------------
void testApp::draw() {
	switch (mode) {
		case 0:
			draw3d();
			break;
		case 1:
			client.getTextureReference().draw(0.0, 0.0);
			break;
		default:
			client.getDepthTextureReference().draw(0.0, 0.0);
			break;
	}
	
	gui.draw();

	// Show framerate.
	stringstream ss;
	ss << "FPS: " << ofGetFrameRate();
	ofDrawBitmapString(ss.str(), 520, 460);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN)
		mode = (mode + 1) % 3;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
	gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
	gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) {
}

//--------------------------------------------------------------
void testApp::draw3d() {
	ofPushMatrix();

	rotate += ofVec3f(1.0, 2.0, 3.0);
	ofTranslate(320, 240);
	ofRotateX(rotate.x);
	ofRotateY(rotate.y);
	ofRotateZ(rotate.z);
	ofTranslate(-320, -240);
	
	ofPixels pixels = client.getPixelsRef();
	ofPixels depthPixels = client.getDepthPixelsRef();
	
	for (int x = 0; x < 640; x += 4) {
		for (int y = 0; y < 480; y += 4) {
			float z = (depthPixels.getColor(x, y).getBrightness() - 127) / 2.0;
			ofSetColor(pixels.getColor(x, y));
			ofCircle(x, y, z, 2);
		}
	}
	
	ofSetColor(255, 255, 255, 255);
	ofPopMatrix();
}