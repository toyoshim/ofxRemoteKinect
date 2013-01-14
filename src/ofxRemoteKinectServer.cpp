#include "ofxRemoteKinectServer.h"

#include <sstream>

#include "ofxRemoteKinectProtocol.h"

//--------------------------------------------------------------
ofxRemoteKinectServer::ofxRemoteKinectServer(): publisherPort(8989),
						responderPort(8990),
						nearClip(500.0),
						farClip(4000.0),
						tiltAngle(0.0),
						quality(2),
						subscribed(false),
						activeTimer(0) {
}

ofxRemoteKinectServer::~ofxRemoteKinectServer() {
	// Reset kinect device configurations.
	kinect.setDepthClipping();
	kinect.setCameraTiltAngle(0.0);
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::setup() {
	// Initialize kinect device.
	kinect.setRegistration(true);
	kinect.init();
	kinect.open();
	
	// TODO(toyoshim): Revive movie fallback mode.
	
	// Start ZMQ servers.
	stringstream ss;
	ss << "tcp://*:" << publisherPort;
	publisher.bind(ss.str());
	ofLog(OF_LOG_NOTICE) << "Start remote kinect publisher at tcp://*: " << publisherPort;
	
	ss.str("");
	ss << "tcp://*:" << responderPort;
	responder.bind(ss.str());
	ofLog(OF_LOG_NOTICE) << "Start remote kinect responder at tcp://*: " << responderPort;
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::update() {
	if (subscribed) {
		activeTimer--;
		if (activeTimer == 0)
			subscribed = false;
	}
	try {
		while (responder.hasWaitingMessage()) {
			string data;
			responder.getNextMessage(data);

			ofxRemoteKinectProtocol::type type;
			float f1;
			float f2;
			int i1;
			ofxRemoteKinectProtocol::parse(data, &type, &f1, &f2, &i1);
			string response;
			switch (type) {
				case ofxRemoteKinectProtocol::TYPE_PING:
					subscribed = true;
					activeTimer = 30;
					ofxRemoteKinectProtocol::buildPong(&response);
					break;
				case ofxRemoteKinectProtocol::TYPE_CLIP:
					setClip(f1, f2);
					ofxRemoteKinectProtocol::buildAck(&response);
					break;
				case ofxRemoteKinectProtocol::TYPE_TILT_ANGLE:
					setTiltAngle(f1);
					ofxRemoteKinectProtocol::buildAck(&response);
					break;
				case ofxRemoteKinectProtocol::TYPE_QUALITY:
					setQuality(i1);
					ofxRemoteKinectProtocol::buildAck(&response);
					break;
				default:
					ofxRemoteKinectProtocol::buildNack(&response);
					break;
			}
			responder.send(response);
		}
	} catch (zmq::error_t e) {
		ofLog(OF_LOG_ERROR) << "unexpected ZMQ exception: " << e.what();
	}
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::draw() {
	// Draw actual kinect images.
	kinect.update();
	kinect.getTextureReference().draw(0.0, 0.0);
	kinect.getDepthTextureReference().draw(640.0, 0.0);
	
	// TODO(toyoshim): Revive recording feature.
	
	// Skip following process if there is no subscriber.
	if (!subscribed)
		return;

	// Compress images by JPEG with the specified quality.
	ofImageQualityType qualityType = static_cast<ofImageQualityType>(quality);
	ofBuffer videoBuffer;
	ofSaveImage(kinect.getPixelsRef(), videoBuffer, OF_IMAGE_FORMAT_JPEG, qualityType);
	string data;
	ofxRemoteKinectProtocol::buildVideoFrame(&data, videoBuffer);
	publisher.send(data, true);

	ofBuffer depthBuffer;
	ofSaveImage(kinect.getDepthPixelsRef(), depthBuffer, OF_IMAGE_FORMAT_JPEG, qualityType);
	ofxRemoteKinectProtocol::buildDepthFrame(&data, depthBuffer);
	publisher.send(data, true);

	// Show compressed images.
	ofImage videoImage;
	videoImage.loadImage(videoBuffer);
	videoImage.draw(0.0, 480.0);
	
	ofImage depthImage;
	depthImage.loadImage(depthBuffer);
	depthImage.draw(640.0, 480.0);
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::setPorts(int publisher, int responder) {
	publisherPort = publisher;
	responderPort = responder;
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::setClip(float newNearClip, float newFarClip) {
	nearClip = newNearClip;
	farClip  = newFarClip;
	
	string data;
	ofxRemoteKinectProtocol::buildClip(&data, nearClip, farClip);
	publisher.send(data, true);
	
	kinect.setDepthClipping(nearClip, farClip);
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::setTiltAngle(float newTiltAngle) {
	tiltAngle = newTiltAngle;
	
	string data;
	ofxRemoteKinectProtocol::buildTiltAngle(&data, tiltAngle);
	publisher.send(data, true);

	kinect.setCameraTiltAngle(tiltAngle);
}

//--------------------------------------------------------------
void ofxRemoteKinectServer::setQuality(int newQuality) {
	quality = newQuality;
	
	string data;
	ofxRemoteKinectProtocol::buildQuality(&data, quality);
	publisher.send(data, true);
}
