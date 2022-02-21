#pragma once

#include "ofMain.h"
#include <vector>
#include <list>
#include "Globals.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"
#include "ofxFFmpegRecorder.h"

using namespace cv;
using namespace ofxCv;

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

	// video:
	float vidWidth;
	float vidHeight;

	// edge and line detection:
	ofxCvColorImage colorImg;
	ofxCvGrayscaleImage grayImg;

	ofImage img, edge_img, sobel_img;

	// communication:
	int midiParams[128];

	ofSerial serial;

	bool bSendSerialMessage; // a flag for sending serial
	char bytesRead[10];		 // data from serial, we will be trying to read 3
	char bytesReadString[11]; // a string needs a null terminator, so we need 3 + 1 bytes
	int nBytesRead;			 // how much did we read?
	int nTimesRead;			 // how many times did we read?
	float readTime;			 // when did we last read?

	ofTrueTypeFont font;
};
