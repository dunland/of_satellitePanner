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

	// ------------------------------ GUI -----------------------------
	// main GUI:
	ofxPanel gui_main;
	ofxToggle gui_main_drawCircles;
	ofxToggle gui_main_drawLines;

	// GUI for circles:
	ofxPanel gui_circles;
	ofxIntSlider gui_circleRadius;
	ofxFloatSlider gui_circleShrinkFactor;
	ofxFloatSlider gui_circleGrowFactor;
	ofxIntSlider gui_spawn_threshold;
	ofxFloatSlider gui_circleSpawnProbability;
	ofxToggle guiChangeSpawnMode;

	// GUI for line detection:
	ofxPanel gui_lines;

	//    Canny Edge Detection
	ofxIntSlider gui2_edgeThreshold;
	//    Hough Transform Lines
	ofxIntSlider gui2_lineThreshold;
	ofxIntSlider gui2_minLineLength;
	ofxIntSlider gui2_maxLineGap;

};
