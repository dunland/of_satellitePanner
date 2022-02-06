#include "Globals.h"

//////////////////////////////// CIRCLES CLASS ////////////////////////

void Circle::draw()
{
    color.set(color.r, color.g, color.b, life_cycle);
    if (CircleControls::spawn_mode[CircleControls::spawn_index] == "brightness")
        radius = color.getBrightness() / 255 * CircleControls::radius;
    else if (CircleControls::spawn_mode[CircleControls::spawn_index] == "lightness")
        radius = color.getLightness() / 255 * CircleControls::radius;
    ofSetColor(color);
    ofFill();
    ofDrawCircle(x - radius, y - radius, radius);
}

////////////////////////////// CIRCLE CONTROLS ////////////////////////

std::vector<Circle *> CircleControls::circles;
bool CircleControls::circle_list[1920][1080]; // place holder to keep track where circles are

float CircleControls::radius_standard = 5; // this is the value the dot size will fall back to
ofParameter<int> CircleControls::radius = 7;          // actual (temporary) dot radius

ofParameter<float> CircleControls::growFactor = 1;
ofParameter<float> CircleControls::shrinkFactor = 0.1;

bool CircleControls::draw_circles = false;
ofParameter<float> CircleControls::spawnProbability = 0.3;

int CircleControls::spawn_threshold = 180;
int CircleControls::previous_spawn_threshold = CircleControls::spawn_threshold;

int CircleControls::spawn_index = 0;
vector<string> CircleControls::spawn_mode = {"brightness", "lightness"};

// --------------------- check pixel brightness -----------------------
void CircleControls::checkThreshold(int x, int y, float threshold_val)
{
    if (threshold_val > CircleControls::spawn_threshold)
    {
        // empty slot: create circle ----------------------------------
        if (circle_list[x][y] == false)
        {
            if (ofRandom(0, 1) < CircleControls::spawnProbability)
            {
                CircleControls::circles.push_back(new Circle(x, y, CircleControls::radius));
                circle_list[x][y] = true;
            }
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

void CircleControls::initialCircleCreation(int vidWidth, int vidHeight)
{
    // create circles initially :
    for (int i = CircleControls::radius * 2; i < vidWidth - CircleControls::radius * 2; i += CircleControls::radius * 2)
    {
        for (int j = CircleControls::radius * 2; j < vidHeight - CircleControls::radius * 2; j += CircleControls::radius * 2)
        {
            if (ofRandom(0, 1) > CircleControls::spawnProbability)
            {
                CircleControls::circles.push_back(new Circle(i, j, CircleControls::radius));
                CircleControls::circle_list[i][j] = true;
            }
        }
    }
}

// ------------------------- resize circles ---------------------------
void CircleControls::resize_circles()
{
    ofPixels &vidPixels = Globals::video.getPixels();

    int vidWidth = vidPixels.getWidth();
    int vidHeight = vidPixels.getHeight();

    int r = CircleControls::radius;

    // kill all existing circles:
    CircleControls::circles.clear();
    for (int i = 0; i < 1920; i++)
        for (int j = 0; j < 1080; j++)
            CircleControls::circle_list[i][j] = false;

    // create new circles:
    for (int i = r * 3; i < vidWidth; i += r * 2)
    {
        for (int j = r * 3; j < vidHeight; j += r * 2)
        {
            if (ofRandom(0, 1) < CircleControls::spawnProbability)
            {
                CircleControls::circles.push_back(new Circle(i, j, r));
                CircleControls::circle_list[i][j] = true;
            }
        }
    }
}
/////////////////////////////// GLOBALS ///////////////////////////////
vector<string> Globals::videoPaths;
ofVideoPlayer Globals::video;
int Globals::vidIdx = 0;
bool Globals::showVideo = false;

/////////////////////////// LINE DETECTION ////////////////////////////
bool LineDetection::drawLines = true;

int LineDetection::edgeThreshold; //    Canny Edge Detection
int LineDetection::lineThreshold; //    Hough Transform Lines
int LineDetection::minLineLength;
int LineDetection::maxLineGap;
