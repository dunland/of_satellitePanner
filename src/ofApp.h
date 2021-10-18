#pragma once

#include "ofMain.h"
#include <vector>
#include <list>
#include "Globals.h"
#include "ofxOsc.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxGui.h"

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

	void audioSetup(int deviceNum);

	ofSoundStream soundStream;

	ofxOscReceiver receiver;

	bool use_video = true;
	bool show_video = false; // toggles video on/off

	Circle *circles_list[1920][1080]; // TODO: work with circles matrix instead of vector

	bool spacebar_lock = false;

	float vidWidth;
	float vidHeight;

    // edge and line detection:
	ofxCvColorImage colorImg;
    ofxCvGrayscaleImage grayImg;

    ofImage img, edge_img, sobel_img;
};
