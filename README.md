ofxRemoteKinect
===============
Copyright © 2013 Takashi Toyoshima <toyoshim@gmail.com>. See also LICENSE.txt.

Description
-----------
ofxRemoteKinect is an openFrameworks addon for using Xbox Kinect via network.

Usually, Kinect is connected to the PC via USB. As a result, it's too difficult
to extend cable length in a reliable way with reasonable cost. But this addon
make it possible to use Kinect remotely. Kinect is connected to a server PC via
USB. The server runs a service program to pass the Kinect data to a client PC
via TCP/IP. LAN is easy to use in the distance. Of couse, you can use it via
Wifi network. Also you can use it via the internet with acceptable latency.

Dependency
----------
ofxRemoteKinect depends on some other addons.

+ [ofxZmq](https://github.com/satoruhiga/ofxZmq)
    commit affab2a0ff02f660042e08b75e318fa4cdb4bb4d
+ [ofxKinect](https://github.com/ofTheo/ofxKinect)
    commit eb06686cbc5d38a07fcf24817ad9e73031f6a4b7
+ [ofxControlPanel](https://github.com/kylemcdonald/ofxControlPanel)
    commit 98697f40b0dfa423689d9360d04cfb026de5d7c8
+ ofxXmlSettings openFrameworks 0073 includes

Installation
------------
To use ofxRemoteKinect, checkout this repositry in addition to openFrameworks
and dependent addons.

`git clone https://github.com/toyoshim/ofxRemoteKinect.git`

The addon should sit in `openFrameworks/addons/ofxRemoteKinect/`.

Example Projects
----------------
Two example projects exists in the `example` folder.

### remoteKinectServer
This is a reference server implementation. You may want to use it as is. If you
want to listen at different ports, you can change it by editing port numbers
in main.cpp.

### remoteKinectClient
This is an example client. Also you can change the server address by editing
server and port description written in main.cpp.

This example use ofxRemoteKinectClient class to control kinect configurations
and get the latest screen information. This class provide functions to control
near and far clipping parameters for depth information, tilt angle, and JPEG
compression quality which is used to transfer screen information. Also it
provide remote kinect screen information as ofTexture, ofPixels, and direct
pointers to buffer memories as ofxKinect does.
