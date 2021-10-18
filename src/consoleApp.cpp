#include "consoleApp.h"
#include <iostream>
#include "Globals.h"

//--------------------------------------------------------------
void consoleApp::setup()
{

    ofSetWindowTitle("consoleApp");

    // main gui:
    gui_main.setup();
    gui_main.add(gui_main_drawCircles.setup("draw circles?", true));
    gui_main.add(gui_main_drawLines.setup("draw lines?", true));
    gui_main.setPosition(0,100);

    // circles:
    gui_circles.setup();
    gui_circles.add(gui_spawn_threshold.setup("spawn threshold", 180, 0, 255));
    gui_circles.add(gui_circleSpawnProbability.setup("spawn probability", 0.1, 0, 1));
    gui_circles.add(gui_circleRadius.setup("circle radius", 7, 2, 100));
    gui_circles.add(gui_circleShrinkFactor.setup("circle shrink factor", 0.1, 0, 10));
    gui_circles.add(gui_circleGrowFactor.setup("circle grow factor", 1, 0, 10));
    gui_circles.add(guiChangeSpawnMode.setup("toggle spawn mode", false));
    gui_circles.setPosition(0, 200);

    // lines:
    gui_lines.setup();
    gui_lines.add(gui2_edgeThreshold.setup("Edge Threshold", 50, 0, 100));
    gui_lines.add(gui2_lineThreshold.setup("Line Threshold", 150, 0, 200));
    gui_lines.add(gui2_minLineLength.setup("min line length", 10, 0, 200));
    gui_lines.add(gui2_maxLineGap.setup("max line gap", 10, 0, 200));
    gui_lines.setPosition(0, 350);
}

///////////////////////////////////////////////////////////////////////
void consoleApp::update()
{
    // ------------ update gui/global variables -----------------------
    CircleControls::circles_radius = gui_circleRadius;
    CircleControls::spawn_threshold = gui_spawn_threshold;
    CircleControls::circles_grow_factor = gui_circleGrowFactor;
    CircleControls::circles_shrink_factor = gui_circleShrinkFactor;
    gui_circleSpawnProbability.setMax(ofGetFrameRate() / 60);

    LineDetection::edgeThreshold = gui2_edgeThreshold;
    LineDetection::lineThreshold = gui2_lineThreshold;
    LineDetection::minLineLength = gui2_minLineLength;
    LineDetection::maxLineGap = gui2_maxLineGap;

    // framerate-related spawn probability:
    if (CircleControls::circles_probability > gui_circleSpawnProbability.getMax())
        CircleControls::circles_probability = gui_circleSpawnProbability.getMax();
    else
        CircleControls::circles_probability = gui_circleSpawnProbability;

    // toggle spawn mode according to pixel brightness/lightness
    if (guiChangeSpawnMode)
    {
        CircleControls::spawn_index = (CircleControls::spawn_index + 1) % CircleControls::spawn_mode.size();
        guiChangeSpawnMode = false;
    }

    CircleControls::draw_circles = gui_main_drawCircles;
    LineDetection::drawLines = gui_main_drawLines;

    // ----------------------------------------------------------------
    // kill circles if fr < 30
    if (ofGetFrameRate() < 30)
    {
        if (CircleControls::circles.size() > 0)
        {
            int rnd = int(ofRandom(0, CircleControls::circles.size()));
            CircleControls::circle_list[CircleControls::circles.at(rnd)->x][CircleControls::circles.at(rnd)->y] = false;
            CircleControls::circles.erase(CircleControls::circles.begin() + rnd);
        }
    }
}

///////////////////////////////////////////////////////////////////////
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

    gui_circles.draw();
    gui_lines.draw();
    gui_main.draw();
}

///////////////////////////////////////////////////////////////////////
void consoleApp::keyPressed(int key)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::keyReleased(int key)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::mouseMoved(int x, int y)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::mouseDragged(int x, int y, int button)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::mousePressed(int x, int y, int button)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::mouseReleased(int x, int y, int button)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::mouseEntered(int x, int y)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::mouseExited(int x, int y)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::windowResized(int w, int h)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::gotMessage(ofMessage msg)
{
}

///////////////////////////////////////////////////////////////////////
void consoleApp::dragEvent(ofDragInfo dragInfo)
{
}