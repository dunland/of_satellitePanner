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

bool CircleControls::reset_circles = false;
bool CircleControls::draw_circles = true;
float CircleControls::circles_probability = 0.3;

int CircleControls::brightness_threshold = 180;
int CircleControls::previous_brightness_threshold = CircleControls::brightness_threshold;

void CircleControls::check_brightness(int x, int y, float brightness_val)
{
    if (brightness_val > CircleControls::brightness_threshold)
    {
        // empty slot: create circle ------------------------------------
        if (circle_list[x][y] == false)
        {
            // if (ofRandom(0, 1) > CircleControls::circles_probability)
            // {
                CircleControls::circles.push_back(new Circle(x, y, CircleControls::circles_radius));
                circle_list[x][y] = true;
            // }
        }
        // occupied slot: get circle and do life_cycle++ ----------------
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