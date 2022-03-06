#include "Globals.h"

//////////////////////////////// CIRCLES CLASS ////////////////////////

void Circle::draw()
{
    color.set(color.r, color.g, color.b, life_cycle);
    if (CircleControls::spawn_mode[CircleControls::spawn_index] == "brightness")
        radius = color.getBrightness() / 255 * CircleControls::radius + size_bonus;
    else if (CircleControls::spawn_mode[CircleControls::spawn_index] == "lightness")
        radius = color.getLightness() / 255 * CircleControls::radius + size_bonus;
    ofSetColor(color);
    ofFill();
    ofDrawCircle(x - radius, y - radius, radius);
}

////////////////////////////// CIRCLE CONTROLS ////////////////////////

std::vector<Circle *> CircleControls::circles;
bool CircleControls::circle_list[1920][1080]; // place holder to keep track where circles are

float CircleControls::radius_standard = 10;    // this is the value the dot size will fall back to
ofParameter<float> CircleControls::radius = 7; // actual (temporary) dot radius

ofParameter<float> CircleControls::growFactor = 1;
ofParameter<float> CircleControls::shrinkFactor = 0.1;

ofParameter<bool> CircleControls::bDrawCircles = true;
ofParameter<bool> CircleControls::bAutomaticCircleCreation = true;
ofParameter<float> CircleControls::spawnProbability = 0.03;

ofParameter<int> CircleControls::spawn_threshold = 180;
int CircleControls::previous_spawn_threshold = CircleControls::spawn_threshold;

int CircleControls::spawn_index = 0;
vector<string> CircleControls::spawn_mode = {"brightness", "lightness"};

// --------------------- check pixel brightness -----------------------
void CircleControls::checkPixelThreshold(int x, int y, float threshold_val)
{
    if (threshold_val > spawn_threshold)
    {
        // empty slot: create circle ----------------------------------
        if (circle_list[x][y] == false && bAutomaticCircleCreation)
        {
            if (ofRandom(0, 1) < spawnProbability)
            {
                circles.push_back(new Circle(x, y, radius));
                circle_list[x][y] = true;
            }
        }
        // occupied slot: get circle and do life_cycle++ --------------
        else
        {
            for (auto &circle : circles)
            {
                if (circle->x == x && circle->y == y)
                {
                    circle->life_cycle += CircleControls::growFactor;
                    break;
                }
            }
        }
    }
    else
    {
        for (auto &circle : CircleControls::circles)
        {
            if (circle->x == x && circle->y == y)
            {
                circle->life_cycle -= CircleControls::shrinkFactor;
                break;
            }
        }
    }
}

void CircleControls::initialCircleCreation(int vidWidth, int vidHeight)
{
    int r = CircleControls::radius;

    // create circles initially :
    for (int i = r * 2; i < vidWidth - r * 2; i += r * 2)
    {
        for (int j = r * 2; j < vidHeight - r * 2; j += r * 2)
        {
            if (ofRandom(0, 1) > CircleControls::spawnProbability)
            {
                CircleControls::circles.push_back(new Circle(i, j, r));
                CircleControls::circle_list[i][j] = true;
            }
        }
    }
}

// TODO: add ListenerFunction for radius
// void CircleControls::radiusChanged()
// {
//     circles.clear();
// }

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
ofParameter<bool> Globals::showVideo = false;

float Globals::vidWidth;
float Globals::vidHeight;

ofImage Globals::img, Globals::edgeImage, Globals::sobelImage;
// edge and line detection:
ofxCvGrayscaleImage Globals::grayImage;
ofxCvColorImage Globals::colorImage;

void Globals::loadNextVideo()
{
    CircleControls::circles.clear();

    vidIdx = (vidIdx + 1) % videoPaths.size();
    video.load(videoPaths[vidIdx]);

    vidWidth = video.getWidth();
    vidHeight = video.getHeight();

    video.play();

    colorImage.allocate(vidWidth, vidHeight);
    grayImage.allocate(vidWidth, vidHeight);
}

/////////////////////////// LINE DETECTION ////////////////////////////
ofParameter<bool> LineDetection::bDrawLines = true;

ofParameter<int> LineDetection::edgeThreshold; //    Canny Edge Detection
ofParameter<int> LineDetection::lineThreshold; //    Hough Transform Lines
ofParameter<int> LineDetection::minLineLength;
ofParameter<int> LineDetection::maxLineGap;

void LineDetection::lineDetection()
{
    /*
    https://stackoverflow.com/questions/40531468/explanation-of-rho-and-theta-parameters-in-houghlines
    The algorithm goes over every edge pixel (result of Canny, for example) and calculates ρ using the equation ρ = x * cosθ + y * sinθ, for many values of θ.

    The actual step of θ is defined by the function parameter, so if you use the usual math.pi / 180.0 value of theta, the algorithm will compute ρ 180 times in total for just one edge pixel in the image. If you would use a larger theta, there would be fewer calculations, fewer accumulator columns/buckets and therefore fewer lines found.

    The other parameter ρ defines how "fat" a row of the accumulator is. With a value of 1, you are saying that you want the number of accumulator rows to be equal to the biggest ρ possible, which is the diagonal of the image you're processing. So if for some two values of θ you get close values for ρ, they will still go into separate accumulator buckets because you are going for precision. For a larger value of the parameter rho, those two values might end up in the same bucket, which will ultimately give you more lines because more buckets will have a large vote count and therefore exceed the threshold.
    */

    Mat mat = toCv(Globals::edgeImage);

    vector<Vec4i> lines;
    HoughLinesP(mat, lines, 60, CV_PI / 270, lineThreshold, minLineLength, maxLineGap); // (E,Rres,Thetares,Threshold,minLineLength,maxLineGap)
    ofSetColor(255, 0, 0);

    for (int i = 0; i < lines.size(); i++)
    {

        ofColor col = ofColor(0);
        // ofSetColor(Globals::video.getPixels().getColor(lines[i][0], lines[i][1]));
        // col.setHsb(hue * 255, 250, 250);
        // ofSetColor(col);

        float x1 = lines[i][0];
        float y1 = lines[i][1];
        float x2 = lines[i][2];
        float y2 = lines[i][3];
        ofPolyline l;
        l.addVertex(x1, y1);
        l.addVertex(x2, y2);

        col.setHsb(l.getLengthAtIndex(l.getIndexAtPercent(1)), 255, 255);
        ofSetColor(col);
        l.draw();
    }
}