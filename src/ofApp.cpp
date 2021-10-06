#include "ofApp.h"
#include <iostream>
#include "Globals.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // receiver.setup(PORT);

    cout << ofSoundStreamListDevices() << endl;
    // ofSoundStream::setDevice(2);
    ofSoundStreamSetup(2, 0, 48000, 256, 4);

    auto devices = soundStream.getDeviceList();

    ofSoundStreamSettings settings;
    settings.setOutDevice(devices[2]);

    ofSetCircleResolution(100);

    video.load("20210930_Flaggenwind.mp4");
    video.load("satellite_panner.mp4");
    video.play();

    settings.setOutListener(this);
    settings.sampleRate = 48000;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = 256;
    soundStream.setup(settings);

    // // create circles initially:
    // ofPixels &vidPixels = video.getPixels();

    // int vidWidth = vidPixels.getWidth();
    // int vidHeight = vidPixels.getHeight();

    // for (int i = CircleControls::circles_radius * 2; i < vidWidth - CircleControls::circles_radius * 2; i += CircleControls::circles_radius * 2)
    // {
    //     for (int j = CircleControls::circles_radius * 2; j < vidHeight - CircleControls::circles_radius * 2; j += CircleControls::circles_radius * 2)
    //     {
    //         if (ofRandom(0, 1) > CircleControls::circles_probability)
    //         {
    //             CircleControls::circles.push_back(new Circle(i, j, CircleControls::circles_radius));
    //             CircleControls::circle_list[i][j] = true;
    //         }
    //     }
    // }
}

//--------------------------------------------------------------
void ofApp::update()
{
    // OSC -----------------------------------------------------
    // ofxOscMessage message;
    // static ofxOscMessage previous_message = message;
    // receiver.getNextMessage(message);

    // if (message != previous_message)
    // {
    // cout << message.getArgAsInt(0) << endl;
    // previous_message = message;
    // }

    // video ---------------------------------------------------
    video.update();

    ofPixels &vidPixels = video.getPixels();

    int vidWidth = vidPixels.getWidth();
    int vidHeight = vidPixels.getHeight();

    int r = CircleControls::circles_radius;

    // update circle position/density:
    if (CircleControls::reset_circles)
    {
        CircleControls::circles.clear();
        for (int i = 0; i < 1920; i++)
            for (int j = 0; j < 1080; j++)
                CircleControls::circle_list[i][j] = false;

        for (int i = r; i < vidWidth - r * 2; i += r * 2)
        {
            for (int j = r; j < vidHeight - r * 2; j += r * 2)
            {
                if (ofRandom(0, 1) > CircleControls::circles_probability)
                {
                    CircleControls::circles.push_back(new Circle(i, j, r));
                    CircleControls::circle_list[i][j] = true;
                }
            }
        }

        CircleControls::reset_circles = false;
        // cout << r << endl;
    }

    // ------- resizing circles:
    for (auto &circle : CircleControls::circles)
        circle->radius = r;

    // TODO: do this only for neighboring circles of already existent ones

    // int n = int(r * 2);
    // for (int i = 0; i < vidWidth - r * 2; i += r * 2)
    // {
    //     for (int j = 0; j < vidHeight - r * 2; j += r * 2)
    //     {
    //         if (CircleControls::circle_list[i][j] == true)
    //         {
    //             float brightness_val = vidPixels.getColor(i, j).getBrightness();
    //             CircleControls::check_brightness(i, j, brightness_val);
    //             CircleControls::check_brightness(i+n, j, brightness_val);
    //             CircleControls::check_brightness(i+n, j+n, brightness_val);
    //             CircleControls::check_brightness(i, j+n, brightness_val);
    //             CircleControls::check_brightness(i-n, j+n, brightness_val);
    //             CircleControls::check_brightness(i-n, j, brightness_val);
    //             CircleControls::check_brightness(i-n, j-n, brightness_val);
    //             CircleControls::check_brightness(i, j-n, brightness_val);
    //             CircleControls::check_brightness(i+n, j-n, brightness_val);
    //             // i+n, j
    //             // i+n, j+n
    //             // i, j+n
    //             // i-n, j+n
    //             // i-n, j
    //             // i-n, j-n
    //             // i, j-n
    //             // i+n, j-n

    //         }
    //     }
    // }

    // --------------------------- pixel brightness: ----------------------------
    for (int i = 0; i < vidWidth - CircleControls::circles_radius * 2; i += CircleControls::circles_radius * 2)
    {
        for (int j = CircleControls::circles_radius; j < vidHeight - CircleControls::circles_radius * 2; j += CircleControls::circles_radius * 2)
        {
            float brightness_val = vidPixels.getColor(i, j).getBrightness();
            if (brightness_val > CircleControls::brightness_threshold)
            {
                // empty slot: create circle ------------------------------------
                if (CircleControls::circle_list[i][j] == false)
                {
                    if (ofRandom(0, 1) > CircleControls::circles_probability)
                    {
                        CircleControls::circles.push_back(new Circle(i, j, CircleControls::circles_radius));
                        CircleControls::circle_list[i][j] = true;
                    }
                }
                // occupied slot: get circle and do life_cycle++ ----------------
                else
                {
                    Circle *this_circle;
                    for (auto &circle : CircleControls::circles)
                    {
                        if (circle->x == i && circle->y == j)
                        {
                            this_circle = circle;
                            break;
                        }
                    }
                    this_circle->life_cycle += CircleControls::circles_grow_factor;
                }
            }
        }
    }

    // ------------------------- naturally decrease life_cycle: -----------------
    for (int i = 0; i < CircleControls::circles.size(); i++)
    {
        Circle *circle = CircleControls::circles.at(i);
        circle->life_cycle -= CircleControls::circles_shrink_factor;
        if (circle->life_cycle <= 0)
        {
            CircleControls::circles.erase(CircleControls::circles.begin() + i);
            CircleControls::circle_list[circle->x][circle->y] = false;
            cout << "circle removed. circles = " << CircleControls::circles.size() << endl;
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);

    if (show_video)
    {
        ofNoFill();
        ofSetColor(255, 255, 255); // reset color for video (else affected by circles-color)
        video.draw(0, 0);
    }
    ofPixels &vidPixels = video.getPixels();

    for (auto &circle : CircleControls::circles)
    {
        circle->color = vidPixels.getColor(circle->x, circle->y);
        if (CircleControls::draw_circles)
            circle->draw();
    }

    if (video.getIsMovieDone())
    {
        ofSetHexColor(0xFF0000);
        ofDrawBitmapString("end of movie", 20, 440);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if (key == 'b')
    {
        CircleControls::brightness_threshold += 1;
        cout << CircleControls::brightness_threshold << endl;
    }

    else if (key == 'n')
    {
        CircleControls::brightness_threshold -= 1;
        cout << CircleControls::brightness_threshold << endl;
    }

    else if (key == ' ' && !spacebar_lock)
    {
        CircleControls::previous_brightness_threshold = CircleControls::brightness_threshold;
        CircleControls::brightness_threshold -= CircleControls::brightness_threshold / 2;
        spacebar_lock = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if (key == 'r')
    {
        CircleControls::circles_radius = ofRandom(2, 10);
        CircleControls::reset_circles = true;
    }
    else if (key == '+')
    {
        CircleControls::circles_radius += CircleControls::circles_radius * 0.3;
        CircleControls::reset_circles = true; // TODO: CircleControls::reset_circles() as a function
    }
    else if (key == '-')
    {
        CircleControls::circles_radius -= CircleControls::circles_radius * 0.3;
        // if (CircleControls::circles_radius < CircleControls::circles_radius_standard)
        //     CircleControls::circles_radius = CircleControls::circles_radius_standard;
        CircleControls::reset_circles = true;
    }

    else if (key == 'f')
    {
        ofToggleFullscreen();
    }

    else if (key == 'v')
    {
        show_video = !show_video;
    }

    else if (key == 'c')
    {
        CircleControls::draw_circles = !CircleControls::draw_circles;
    }

    else if (key == ' ')
    {
        spacebar_lock = false;
        CircleControls::brightness_threshold = CircleControls::previous_brightness_threshold;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}