#pragma once

#include "ofMain.h"

class ofxRemoteKinectProtocol {
public:
	enum type {
		TYPE_PING,
		TYPE_PONG,
		TYPE_CLIP,
		TYPE_TILT_ANGLE,
		TYPE_QUALITY,
		TYPE_ACK,
		TYPE_NACK,
		TYPE_VIDEO_FRAME,
		TYPE_DEPTH_FRAME,

		TYPE_UNKNOWN
	};
	static void buildPing(string* data);
	static void buildPong(string* data);
	static void buildClip(string* data, float near, float far);
	static void buildTiltAngle(string* data, float tiltAngle);
	static void buildQuality(string* data, int quality);
	
	static void buildAck(string* data);
	static void buildNack(string* data);
	
	static void buildVideoFrame(string* data, const ofBuffer& frame);
	static void buildDepthFrame(string* data, const ofBuffer& frame);
	
	static void parse(const string& data, type* type, float* f1, float* f2, int* i1);
	static void parseFrame(const string& data, ofTexture* texture);
};