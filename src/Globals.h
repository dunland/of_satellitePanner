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
    static float circles_radius_standard; // this is the value the dot size will fall back to
    static float circles_radius;          // actual (temporary) dot radius

    // circle spawn:
    static float circles_probability;
    static int spawn_threshold;
    static int previous_spawn_threshold;
	static int spawn_index;
	static vector<string> spawn_mode;

    // circle change:
    static float circles_grow_factor;
    static float circles_shrink_factor;

    static void checkThreshold(int x, int y, float brightness_val);
    static void resize_circles();

    static std::vector<Circle *> circles;
    static bool circle_list[1920][1080]; // place holder to keep track where circles are
};

/////////////////////////////// GLOBALS ///////////////////////////////
class Globals
{
public:
    static ofVideoPlayer video;
};