#include "ofMain.h"
#include "ofApp.h"
#include "consoleApp.h"
#include "Globals.h"

//========================================================================
int main()
{

	// ofSetupOpenGL(1024, 768, OF_WINDOW); // <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	// ofRunApp(new ofApp());

	ofGLWindowSettings settings;
	settings.setSize(1024, 768);
	settings.setPosition(ofVec2f(300, 0));
	auto mainWindow = ofCreateWindow(settings);

	settings.setSize(300, 300);
	settings.setPosition(ofVec2f(0, 0));
	auto consoleWindow = ofCreateWindow(settings);

	auto mainApp = make_shared<ofApp>();
	auto consoleApp_ = make_shared<consoleApp>();

	ofRunApp(consoleWindow, consoleApp_);
	ofRunApp(mainWindow, mainApp);
	ofRunMainLoop();
}
