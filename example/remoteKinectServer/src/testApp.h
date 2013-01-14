#pragma once

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "ofxRemoteKinect.h"

class testApp : public ofBaseApp {
public:
	testApp(int publisherPort, int responderPort);
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
private:
	ofxControlPanel gui;
	ofxRemoteKinectServer server;
	float nearClip;
	float farClip;
	float tiltAngle;
	int quality;
};
