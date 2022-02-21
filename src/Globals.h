#pragma once

#include "ofMain.h"

class Circle
{

public:
    int x;
    int y;
    int radius;
    float life_cycle;

    ofColor color;

    Circle(int _x, int _y, int radius_)
    {
        x = _x;
        y = _y;
        radius = radius_;
        life_cycle = 255;
        // CircleControls::num_of_circles++;
    }

    void draw();
};

class CircleControls
{
public:
    // general:
    static ofParameter<bool> bDrawCircles;
    static ofParameter<bool> bAutomaticCircleCreation;
    static float radius_standard; // this is the value the dot size will fall back to
    static ofParameter<int> radius;          // actual (temporary) dot radius

    // circle spawn:
    static ofParameter<float> spawnProbability;
    static ofParameter<int> spawn_threshold;
    static int previous_spawn_threshold;
    static int spawn_index;
    static vector<string> spawn_mode;

    // circle change:
    static ofParameter<float> growFactor;
    static ofParameter<float> shrinkFactor;

    static void checkThreshold(int x, int y, float brightness_val);
    static void resize_circles();
    static void initialCircleCreation(int vidWidth, int vidHeight);

    static std::vector<Circle *> circles;
    static bool circle_list[1920][1080]; // place holder to keep track where circles are
};

/////////////////////////////// GLOBALS ///////////////////////////////
class Globals
{
public:
    static vector<string> videoPaths;
    static int vidIdx;
    static ofVideoPlayer video;
	static ofParameter<bool> showVideo; // toggles video on/off
};

class LineDetection
{
public:
    static ofParameter<bool> drawLines;

    static ofParameter<int> edgeThreshold; //    Canny Edge Detection
    static ofParameter<int> lineThreshold; //    Hough Transform Lines
    static ofParameter<int> minLineLength;
    static ofParameter<int> maxLineGap;
};