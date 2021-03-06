#pragma once

#include "ofMain.h"
#include <vector>
#include <list>
#include "Globals.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include "ofxFFmpegRecorder.h"
#include "ofxSimpleSerial.h"

#define PORT 9001

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void onNewMessage(string & message);
	void radiusChanged();

	// Sound:
	ofSoundStream soundStream;
	void audioSetup(int deviceNum);

	// Video recording:
	ofxFFmpegRecorder m_Recorder;
	ofVideoGrabber m_Grabber;

	ofFbo mCapFbo;
	ofPixels mPix;

	ofxOscReceiver receiver;

	bool spacebar_lock = false;

	// communication:
	int midiParams[128];

	ofxSimpleSerial	serial;
	string incomingMessage = "";
};
