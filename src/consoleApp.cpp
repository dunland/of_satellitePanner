#include "consoleApp.h"
#include <iostream>
#include "Globals.h"

//--------------------------------------------------------------
void consoleApp::setup()
{
}

//--------------------------------------------------------------
void consoleApp::update()
{
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

    // display circle radius:
    string circlesRadius = "circles_radius: " + ofToString(CircleControls::circles_radius, 2);
    ofDrawBitmapString(circlesRadius, 20, 80);

    // display circles creation probability:
    string circlesProbability = "circles_probability: " + ofToString(CircleControls::circles_probability, 2);
    ofDrawBitmapString(circlesProbability, 20, 100);

    // display circle brightness creation threshold:
    string brightnessThreshold = "brightness_threshold: " + ofToString(CircleControls::brightness_threshold, 2);
    ofDrawBitmapString(brightnessThreshold, 20, 120);

    // display circle grow and shrink factor:
    string circleGrowFactor = "circle grow factor: " + ofToString(CircleControls::circles_grow_factor, 2);
    ofDrawBitmapString(circleGrowFactor, 20, 140);
    string circleShrinkFactor = "circle shrink factor: " + ofToString(CircleControls::circles_shrink_factor, 2);
    ofDrawBitmapString(circleShrinkFactor, 20, 160);

    // display more stuff
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