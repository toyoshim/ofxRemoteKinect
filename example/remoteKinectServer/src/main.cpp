#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 1280, 960, OF_WINDOW);
	
	int publisherPort = 8989;
	int responderPort = 8990;
	ofRunApp(new testApp(publisherPort, responderPort));
}
