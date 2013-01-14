#include "ofxRemoteKinectProtocol.h"

#include <sstream>

#include "ofMain.h"

namespace {
const char kPing[]       = "ping";
const char kPong[]       = "pong";
const char kClip[]       = "clip";
const char kTiltAngle[]  = "tilt_angle";
const char kQuality[]    = "quality";

const char kAck[]        = "ack";
const char kNack[]       = "nack";

const char kVideoFrame[] = "video";
const char kDepthFrame[] = "depth";

const char kDelimiter[] = " ";
};  // namespace

void ofxRemoteKinectProtocol::buildPing(string* data) {
	*data = kPing;
}

void ofxRemoteKinectProtocol::buildPong(string* data) {
	*data = kPong;
}

void ofxRemoteKinectProtocol::buildClip(string* data, float near, float far) {
	stringstream ss;
	ss << kClip << kDelimiter << near << kDelimiter << far;
	*data = ss.str();
}

void ofxRemoteKinectProtocol::buildTiltAngle(string* data, float angle) {
	stringstream ss;
	ss << kTiltAngle << kDelimiter << angle;
	*data = ss.str();
}

void ofxRemoteKinectProtocol::buildQuality(string* data, int quality) {
	stringstream ss;
	ss << kQuality << kDelimiter << quality;
	*data = ss.str();
}

void ofxRemoteKinectProtocol::buildAck(string* data) {
	*data = kAck;
}

void ofxRemoteKinectProtocol::buildNack(string* data) {
	*data = kNack;
}

void ofxRemoteKinectProtocol::buildVideoFrame(string* data, const ofBuffer& frame) {
	data->clear();
	data->reserve(sizeof(kVideoFrame) - 1 + sizeof(kDelimiter) - 1 + frame.size());
	data->append(kVideoFrame);
	data->append(kDelimiter);
	data->append(frame.getBinaryBuffer(), frame.size());
}

void ofxRemoteKinectProtocol::buildDepthFrame(string* data, const ofBuffer& frame) {
	data->clear();
	data->reserve(sizeof(kDepthFrame) - 1 + sizeof(kDelimiter) - 1 + frame.size());
	data->append(kDepthFrame);
	data->append(kDelimiter);
	data->append(frame.getBinaryBuffer(), frame.size());
}

void ofxRemoteKinectProtocol::parse(const string& data, type* type, float* f1, float* f2, int* i1) {
	// Optimize video frame and depth frame types handling here.
	// TODO(toyoshim): Strict command check should be applied.
	if (data[0] == kVideoFrame[0]) {
		*type = TYPE_VIDEO_FRAME;
		return;
	} else if (data[0] == kDepthFrame[0]) {
		*type = TYPE_DEPTH_FRAME;
		return;
	}

	stringstream ss(data);
	string cmd;
	ss >> cmd;
	if (cmd.compare(kPing) == 0) {
		*type = TYPE_PING;
	} else if (cmd.compare(kPong) == 0) {
		*type = TYPE_PONG;
	} else if (cmd.compare(kClip) == 0) {
		*type = TYPE_CLIP;
		ss.ignore();
		ss >> *f1;
		ss.ignore();
		ss >> *f2;
	} else if (cmd.compare(kTiltAngle) == 0) {
		*type = TYPE_TILT_ANGLE;
		ss.ignore();
		ss >> *f1;
	} else if (cmd.compare(kQuality) == 0) {
		*type = TYPE_QUALITY;
		ss.ignore();
		ss >> *i1;
	} else if (cmd.compare(kAck) == 0) {
		*type = TYPE_ACK;
	} else if (cmd.compare(kNack) == 0) {
		*type = TYPE_NACK;
	} else {
		*type = TYPE_UNKNOWN;
	}
}

void ofxRemoteKinectProtocol::parseFrame(const string& data, ofTexture* texture) {
	assert(sizeof(kVideoFrame) == sizeof(kDepthFrame));
	int headerSize = sizeof(kVideoFrame) - 1 + sizeof(kDelimiter) - 1;
	int dataSize = data.size() - headerSize;
	ofBuffer buffer(&data[headerSize], dataSize);
	ofImage image;
	bool rc = image.loadImage(buffer);
	if (data[0] == kVideoFrame[0])
		texture->loadData(image.getPixels(), 640, 480, GL_RGB);
	else
		texture->loadData(image.getPixels(), 640, 480, GL_LUMINANCE);
}
