#include "consoleApp.h"
#include <iostream>
#include "Globals.h"

//--------------------------------------------------------------
void consoleApp::setup()
{
    gui.setup();
    gui.add(gui_spawn_threshold.setup("spawn threshold", 180, 0, 255));
    gui.add(gui_circleSpawnProbability.setup("spawn probability", 0.1, 0, 1));
    gui.add(gui_circleRadius.setup("circle radius", 180, 4, 255));
    gui.add(gui_circleShrinkFactor.setup("circle shrink factor", 0.1, 0, 10));
    gui.add(gui_circleGrowFactor.setup("circle grow factor", 1, 0, 10));
    gui.add(guiChangeSpawnMode.setup("toggle spawn mode", false));
    gui.setPosition(0, 100);
}

//--------------------------------------------------------------
void consoleApp::update()
{
    // update global values with gui:
    CircleControls::circles_radius = gui_circleRadius;
    CircleControls::spawn_threshold = gui_spawn_threshold;
    CircleControls::circles_grow_factor = gui_circleGrowFactor;
    CircleControls::circles_shrink_factor = gui_circleShrinkFactor;
    CircleControls::circles_probability = gui_circleSpawnProbability;

    if (guiChangeSpawnMode)
    {
        CircleControls::spawn_index = (CircleControls::spawn_index + 1) % CircleControls::spawn_mode.size();
        guiChangeSpawnMode = false;
    }
}

//--------------------------------------------------------------
void consoleApp::draw()
{
    ofBackground(0);

    // display samplerate:
    string fpsStr = "frame rate: " + ofToString(ofGetFrameRate(), 2);
    ofDrawBitmapString(fpsStr, 20, 40);

    // display num of circles:
    string numOfCircles = "num of circles: " + ofToString(CircleControls::circles.size(), 2);
    ofDrawBitmapString(numOfCircles, 20, 60);

    string strSpawnMode = "spawn mode: " + CircleControls::spawn_mode[CircleControls::spawn_index];
    ofDrawBitmapString(strSpawnMode, 20, 80);

    gui.draw();
}

//--------------------------------------------------------------
void consoleApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void consoleApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void consoleApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void consoleApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void consoleApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void consoleApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void consoleApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void consoleApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void consoleApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void consoleApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void consoleApp::dragEvent(ofDragInfo dragInfo)
{
}