#pragma once

#include "ofMain.h"
#include <vector>
#include <list>
#include "Globals.h"
#include "ofxOsc.h"
#include "ofxFFMPEGVideoPlayer.h"

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

	ofSoundStream soundStream;

	ofxOscReceiver receiver;

	ofImage image;
	ofPixels imgPixels;
	ofTexture texture;

	ofVideoPlayer video;
	bool use_video = true;
	bool show_video = false; // toggles video on/off

	Circle *circles_list[1920][1080]; // TODO: work with circles matrix instead of vector

	bool spacebar_lock = false;

	// -------- open cv: ----------
	// ofxCvColorImage colorImg;

	// ofxCvGrayscaleImage grayImage;
	// ofxCvGrayscaleImage grayBg;
	// ofxCvGrayscaleImage grayDiff;

	// ofxCvContourFinder contourFinder;

	// int threshold;
	// bool bLearnBakground;
};
