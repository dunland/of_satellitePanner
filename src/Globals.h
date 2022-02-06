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
    static bool draw_circles;
    static float radius_standard; // this is the value the dot size will fall back to
    static ofParameter<int> radius;          // actual (temporary) dot radius

    // circle spawn:
    static ofParameter<float> spawnProbability;
    static int spawn_threshold;
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
    static ofVideoPlayer video;
	static bool showVideo; // toggles video on/off
};

class LineDetection
{
public:
    static bool drawLines;

    static int edgeThreshold; //    Canny Edge Detection
    static int lineThreshold; //    Hough Transform Lines
    static int minLineLength;
    static int maxLineGap;
};