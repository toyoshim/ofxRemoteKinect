#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxZmq.h"

class ofxRemoteKinectServer {
public:
	ofxRemoteKinectServer();
	~ofxRemoteKinectServer();
	
	void setup();
	void update();
	void draw();
	
	void setPorts(int publisher, int responder);  // Should be called before calling setup().
	void setClip(float nearClip, float farClip);
	void setTiltAngle(float tiltAngle);
	void setQuality(int quality);
	
	float getNearClip() { return nearClip; }
	float getFarClip() { return farClip; }
	float getTiltAngle() { return tiltAngle; }
	int getQuality() { return quality; }
	
private:
	int publisherPort;
	int responderPort;
	float nearClip;
	float farClip;
	float tiltAngle;
	int quality;
	
	ofxKinect kinect;
	ofxZmqPublisher publisher;
	ofxZmqReply responder;
	
	bool subscribed;
	int activeTimer;
};
