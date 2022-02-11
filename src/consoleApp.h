#pragma once

#include "ofMain.h"
#include <vector>
#include <list>
#include "ofxGui.h"
#include "Globals.h"

class consoleApp : public ofBaseApp
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

	// ------------------------------ GUI -----------------------------
	// main GUI:
	ofxPanel gui_main;
	ofxToggle gui_main_automaticParams;

	// GUI for circles:
	ofxPanel gui_circles;
	ofxToggle guiChangeSpawnMode;

	// GUI for line detection:
	ofxPanel gui_lines;
};
