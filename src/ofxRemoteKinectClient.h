#pragma once

#include "ofMain.h"
#include "ofxZmq.h"

class ofxRemoteKinectClient {
public:
	ofxRemoteKinectClient();
	
	void setup();
	void update();
	void draw();

	// setServer() should be called before calling setup().
	// |server| string should follow the format 'tcp://<host>:<port>'.
	void setServers(const string& publisher, const string& responder);
	// Others should be called after calling setup().
	void setClip(float nearClip, float farClip);
	void setTiltAngle(float tiltAngle);
	void setQuality(int quality);
	
	float getNearClip() { return nearClip; }
	float getFarClip() { return farClip; }
	float getTiltAngle() { return tiltAngle; }
	int getQuality() { return quality; }
	
	unsigned char* getPixels() { return getPixelsRef().getPixels(); }
	unsigned char* getDepthPixels() { return getDepthPixelsRef().getPixels(); }

	ofPixels& getPixelsRef();
	ofPixels& getDepthPixelsRef();
	
	ofTexture& getTextureReference() { return texture; }
	ofTexture& getDepthTextureReference() { return depthTexture; }
	
private:
	string publisher;
	string responder;
	float nearClip;
	float farClip;
	float tiltAngle;
	int quality;

	ofxZmqSubscriber subscriber;
	ofxZmqRequest requester;
	ofxZmqRequest ping;
	
	ofTexture texture;
	ofTexture depthTexture;
	
	ofPixels pixels;
	ofPixels depthPixels;
	
	bool pixelsIsDirty;
	bool depthPixelsIsDirty;
};
