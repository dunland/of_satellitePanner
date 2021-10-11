#include "Globals.h"

//////////////////////////////// CIRCLES CLASS ////////////////////////

void Circle::draw()
{
    color.set(color.r, color.g, color.b, life_cycle);
    radius = color.getBrightness() / 255 * CircleControls::circles_radius;
    ofSetColor(color);
    ofFill();
    ofDrawCircle(x - radius, y - radius, radius);
}

////////////////////////////// CIRCLE CONTROLS ////////////////////////

std::vector<Circle *> CircleControls::circles;
bool CircleControls::circle_list[1920][1080]; // place holder to keep track where circles are

float CircleControls::circles_radius_standard = 5; // this is the value the dot size will fall back to
float CircleControls::circles_radius = 7;          // actual (temporary) dot radius

float CircleControls::circles_grow_factor = 1;
float CircleControls::circles_shrink_factor = 0.1;

bool CircleControls::draw_circles = true;
float CircleControls::circles_probability = 0.3;

int CircleControls::brightness_threshold = 180;
int CircleControls::previous_brightness_threshold = CircleControls::brightness_threshold;

// --------------------- check pixel brightness -----------------------
void CircleControls::check_brightness(int x, int y, float brightness_val)
{
    if (brightness_val > CircleControls::brightness_threshold)
    {
        // empty slot: create circle ----------------------------------
        if (circle_list[x][y] == false)
        {
            // if (ofRandom(0, 1) > CircleControls::circles_probability)
            // {
            CircleControls::circles.push_back(new Circle(x, y, CircleControls::circles_radius));
            circle_list[x][y] = true;
            // }
        }
        // occupied slot: get circle and do life_cycle++ --------------
        else
        {
            Circle *this_circle;
            for (auto &circle : circles)
            {
                if (circle->x == x && circle->y == y)
                {
                    this_circle = circle;
                    break;
                }
            }
            this_circle->life_cycle++;
        }
    }
}

// ------------------------- resize circles ---------------------------
void CircleControls::resize_circles()
{
    ofPixels &vidPixels = Globals::video.getPixels();

    int vidWidth = vidPixels.getWidth();
    int vidHeight = vidPixels.getHeight();

    int r = CircleControls::circles_radius;

    // kill all existing circles:
    CircleControls::circles.clear();
    for (int i = 0; i < 1920; i++)
        for (int j = 0; j < 1080; j++)
            CircleControls::circle_list[i][j] = false;

    // create new circles:
    for (int i = r * 2; i < vidWidth; i += r * 2)
    {
        for (int j = r * 2; j < vidHeight; j += r * 2)
        {
            if (ofRandom(0, 1) > CircleControls::circles_probability)
            {
                CircleControls::circles.push_back(new Circle(i, j, r));
                CircleControls::circle_list[i][j] = true;
            }
        }
    }
}
/////////////////////////////// GLOBALS ///////////////////////////////
ofVideoPlayer Globals::video;