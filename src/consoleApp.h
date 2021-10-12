#pragma once

#include "ofMain.h"
#include <vector>
#include <list>
#include "ofxGui.h"


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

	// GUI:
	ofxPanel gui;
	ofxIntSlider gui_circleRadius;
	ofxFloatSlider gui_circleShrinkFactor;
	ofxFloatSlider gui_circleGrowFactor;
	ofxIntSlider gui_spawn_threshold;
	ofxFloatSlider gui_circleSpawnProbability;
	ofxToggle guiChangeSpawnMode;
};
