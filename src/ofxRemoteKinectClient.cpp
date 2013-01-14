#include "ofxRemoteKinectClient.h"

#include <sstream>

#include "ofxRemoteKinectProtocol.h"

//--------------------------------------------------------------
ofxRemoteKinectClient::ofxRemoteKinectClient(): publisher("tcp://localhost:8989"),
						responder("tcp://localhost:8990"),
						nearClip(500.0),
						farClip(4000.0),
						tiltAngle(0.0),
						quality(2),
						pixelsIsDirty(true),
						depthPixelsIsDirty(true) {
	texture.allocate(640, 480, GL_RGB);
	depthTexture.allocate(640, 480, GL_LUMINANCE);
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::setup() {
	// Start ZMQ clients.
	subscriber.connect(publisher);
	requester.connect(responder);
	ping.connect(responder);
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::update() {
	try {
		string data;
		while (ping.hasWaitingMessage()) {
			ping.getNextMessage(data);
			// TODO(toyoshim): Check if the response is ping.
		}
		while (requester.hasWaitingMessage()) {
			requester.getNextMessage(data);
			// TODO(toyoshim): Check if the response is ack.
		}
		while (subscriber.hasWaitingMessage()) {
			subscriber.getNextMessage(data);

			ofxRemoteKinectProtocol::type type;
			float f1;
			float f2;
			int i1;
			ofxRemoteKinectProtocol::parse(data, &type, &f1, &f2, &i1);
			switch (type) {
				case ofxRemoteKinectProtocol::TYPE_CLIP:
					nearClip = f1;
					farClip = f2;
					break;
				case ofxRemoteKinectProtocol::TYPE_TILT_ANGLE:
					tiltAngle = f1;
					break;
				case ofxRemoteKinectProtocol::TYPE_QUALITY:
					quality = i1;
					break;
				case ofxRemoteKinectProtocol::TYPE_VIDEO_FRAME:
					ofxRemoteKinectProtocol::parseFrame(data, &texture);
					pixelsIsDirty = true;
					break;
				case ofxRemoteKinectProtocol::TYPE_DEPTH_FRAME:
					ofxRemoteKinectProtocol::parseFrame(data, &depthTexture);
					depthPixelsIsDirty = true;
					break;
				default:
					// TODO(toyoshim): Handle unexpected data.
					break;
			}
		}
	} catch (zmq::error_t e) {
		ofLog(OF_LOG_ERROR) << "ZMQ catch exception: " << e.what();
	}
	// TODO(toyoshim): Implement an internal send queue to wait for reply for previous request.
	// Currently, requests and pings may drop.
	string data;
	ofxRemoteKinectProtocol::buildPing(&data);
	ping.send(data);
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::draw() {
	texture.draw(0.0, 0.0);
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::setServers(const string& newPublisher, const string& newResponder) {
	publisher = newPublisher;
	responder = newResponder;
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::setClip(float newNearClip, float newFarClip) {
	nearClip = newNearClip;
	farClip  = newFarClip;
	string data;
	ofxRemoteKinectProtocol::buildClip(&data, nearClip, farClip);
	requester.send(data, true);
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::setTiltAngle(float newTiltAngle) {
	tiltAngle = newTiltAngle;
	string data;
	ofxRemoteKinectProtocol::buildTiltAngle(&data, tiltAngle);
	requester.send(data, true);
}

//--------------------------------------------------------------
void ofxRemoteKinectClient::setQuality(int newQuality) {
	quality = newQuality;
	string data;
	ofxRemoteKinectProtocol::buildQuality(&data, quality);
	requester.send(data, true);
}

//--------------------------------------------------------------
ofPixels& ofxRemoteKinectClient::getPixelsRef() {
	if (pixelsIsDirty) {
		texture.readToPixels(pixels);
		pixelsIsDirty = false;
	}
	return pixels;
}

//--------------------------------------------------------------
ofPixels& ofxRemoteKinectClient::getDepthPixelsRef() {
	if (depthPixelsIsDirty) {
		depthTexture.readToPixels(depthPixels);
		depthPixelsIsDirty = false;
	}
	return depthPixels;
}