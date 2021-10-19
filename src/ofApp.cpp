#include "ofApp.h"
#include <iostream>
#include "Globals.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    ofSetWindowTitle("ofApp");

    // receiver.setup(PORT);

    ofSetCircleResolution(100);

    // audioSetup(2);

    Globals::video.load("20210930_Flaggenwind.mp4");
    Globals::video.load("20211010_SF-Küste_00.mp4");
    Globals::video.load("satellite_panner.mp4");

    vidWidth = Globals::video.getWidth();
    vidHeight = Globals::video.getHeight();

    Globals::video.play();

    colorImg.allocate(vidWidth, vidHeight);
    grayImg.allocate(vidWidth, vidHeight);

    // create circles initially:
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
    Globals::video.update();

    colorImg.setFromPixels(Globals::video.getPixels());
    colorImg.convertToGrayscalePlanarImage(grayImg, 0);
    grayImg.threshold(160);

    Canny(grayImg, edge_img, LineDetection::edgeThreshold, LineDetection::edgeThreshold * 2);
    Sobel(edge_img, sobel_img);

    ofPixels &vidPixels = Globals::video.getPixels();

    int r = CircleControls::circles_radius;

    // TODO: do this only for neighboring circles of already existent ones

    // int n = int(r * 2);
    // for (int i = 0; i < vidWidth - r * 2; i += r * 2)
    // {
    //     for (int j = 0; j < vidHeight - r * 2; j += r * 2)
    //     {
    //         if (CircleControls::circle_list[i][j] == true)
    //         {
    //             float brightness_val = vidPixels.getColor(i, j).getBrightness();
    //             CircleControls::checkThreshold(i, j, brightness_val);
    //             CircleControls::checkThreshold(i+n, j, brightness_val);
    //             CircleControls::checkThreshold(i+n, j+n, brightness_val);
    //             CircleControls::checkThreshold(i, j+n, brightness_val);
    //             CircleControls::checkThreshold(i-n, j+n, brightness_val);
    //             CircleControls::checkThreshold(i-n, j, brightness_val);
    //             CircleControls::checkThreshold(i-n, j-n, brightness_val);
    //             CircleControls::checkThreshold(i, j-n, brightness_val);
    //             CircleControls::checkThreshold(i+n, j-n, brightness_val);
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

    // TODO: opencv → circle creation only along lines!

    if (CircleControls::draw_circles)
    {

        // naturally create one circle at a time:
        if (ofGetFrameNum() > 1 && ofGetFrameRate() > 30)
        {
            // --------------------------- pixel brightness: ----------------------------
            // int i = int(ofRandom(0, ofGetWindowWidth()));
            // int j = int(ofRandom(0, ofGetWindowHeight()));
            for (int i = r * 2; i < vidWidth; i += r * 2)
            {
                for (int j = r * 2; j < vidHeight; j += r * 2)
                {
                    float threshold_val;
                    if (CircleControls::spawn_mode[CircleControls::spawn_index] == "brightness")
                        threshold_val = vidPixels.getColor(i, j).getBrightness();
                    else
                        threshold_val = vidPixels.getColor(i, j).getLightness(); // TODO: make selectable
                    CircleControls::checkThreshold(i, j, threshold_val);
                }
            }
        }

        // ----------------- naturally decrease life_cycle: ------------
        for (int i = 0; i < CircleControls::circles.size(); i++)
        {
            Circle *circle = CircleControls::circles.at(i);
            circle->life_cycle -= CircleControls::circles_shrink_factor;
            if (circle->life_cycle <= 0)
            {
                CircleControls::circles.erase(CircleControls::circles.begin() + i);
                CircleControls::circle_list[circle->x][circle->y] = false;
            }
        }
    }
    else
        CircleControls::circles.clear();
}

// --------------------------------------------------------------------
void ofApp::draw()
{
    ofBackground(0);

    if (Globals::showVideo)
    {
        ofNoFill();
        ofSetColor(255, 255, 255); // reset color for video (else affected by circles-color)
        Globals::video.draw(0, 0);
    }

    // ---------------------------- LINE DETECTION --------------------
    if (LineDetection::drawLines)
    {
        Mat mat = toCv(edge_img);

        vector<Vec4i> lines;
        HoughLinesP(mat, lines, 120, CV_PI / 180, LineDetection::lineThreshold, LineDetection::minLineLength, LineDetection::maxLineGap); // (E,Rres,Thetares,Threshold,minLineLength,maxLineGap)
        ofSetColor(255, 0, 0);

        // static float count = 0;
        // static float hue = 0;
        // count += 0.01;
        // hue = sin(count);
        // cout << hue << endl;

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

    // ---------------------------- CIRCLES  --------------------------
    ofPixels &vidPixels = Globals::video.getPixels();

    for (auto &circle : CircleControls::circles)
    {
        circle->color = vidPixels.getColor(circle->x, circle->y);
        if (CircleControls::draw_circles)
            circle->draw();
    }

    if (Globals::video.getIsMovieDone())
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
        CircleControls::spawn_threshold += 1;
        cout << CircleControls::spawn_threshold << endl;
    }

    else if (key == 'n')
    {
        CircleControls::spawn_threshold -= 1;
        cout << CircleControls::spawn_threshold << endl;
    }

    else if (key == ' ')
    {
        ofPixels &vidPixels = Globals::video.getPixels();
        int vidWidth = vidPixels.getWidth();
        int vidHeight = vidPixels.getHeight();

        int r = CircleControls::circles_radius;

        // only create circles if frameRate > 30
        if (ofGetFrameNum() > 1 && ofGetFrameRate() > 30)
        {
            // --------------------------- pixel brightness: ----------------------------
            for (int i = r * 2; i < vidWidth; i += r * 2)
            {
                for (int j = r * 2; j < vidHeight; j += r * 2)
                {
                    float threshold_val;
                    if (CircleControls::spawn_mode[CircleControls::spawn_index] == "brightness")
                        threshold_val = vidPixels.getColor(i, j).getBrightness();
                    else
                        threshold_val = vidPixels.getColor(i, j).getLightness(); // TODO: make selectable
                    CircleControls::checkThreshold(i, j, threshold_val);
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if (key == 'r')
    {
        CircleControls::circles_radius = ofRandom(2, 10);
        CircleControls::resize_circles();
    }
    else if (key == '+')
    {
        CircleControls::circles_radius += CircleControls::circles_radius * 0.3;
        CircleControls::resize_circles();
    }
    else if (key == '-')
    {
        CircleControls::circles_radius -= CircleControls::circles_radius * 0.3;
        // if (CircleControls::circles_radius < CircleControls::circles_radius_standard)
        //     CircleControls::circles_radius = CircleControls::circles_radius_standard;
        CircleControls::resize_circles();
    }

    else if (key == 'f')
    {
        ofToggleFullscreen();
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

//--------------------------------------------------------------
void ofApp::audioSetup(int deviceNum)
{
    cout << ofSoundStreamListDevices() << endl;
    // ofSoundStream::setDevice(2);
    ofSoundStreamSetup(2, 0, 48000, 256, 4);

    auto devices = soundStream.getDeviceList();

    ofSoundStreamSettings settings;
    settings.setOutDevice(devices[deviceNum]);

    settings.setOutListener(this);
    settings.sampleRate = 48000;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = 256;
    soundStream.setup(settings);
}