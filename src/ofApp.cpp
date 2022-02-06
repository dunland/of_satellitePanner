#include "ofApp.h"
#include <iostream>
#include "Globals.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    ofSetFrameRate(30);
    ofSetWindowTitle("ofApp");

    receiver.setup(PORT);

    ofSetCircleResolution(100);

    // audioSetup(2);

    // load videos from data folder
    ofDirectory dir(""); // read folder ./bin/data
    dir.allowExt("mp4");
    dir.allowExt("avi");
    dir.listDir();
    dir = dir.getSorted();
    for (int i = 0; i < dir.size(); i++)
        Globals::videoPaths.push_back(dir.getPath(i));

    Globals::video.load(Globals::videoPaths[Globals::videoPaths.size() - 1]);
    vidWidth = Globals::video.getWidth();
    vidHeight = Globals::video.getHeight();

    Globals::video.play();

    colorImg.allocate(vidWidth, vidHeight);
    grayImg.allocate(vidWidth, vidHeight);

    // create circles initially:
    // for (int i = CircleControls::radius * 2; i < vidWidth - CircleControls::radius * 2; i += CircleControls::radius * 2)
    // {
    //     for (int j = CircleControls::radius * 2; j < vidHeight - CircleControls::radius * 2; j += CircleControls::radius * 2)
    //     {
    //         if (ofRandom(0, 1) > CircleControls::spawnProbability)
    //         {
    //             CircleControls::circles.push_back(new Circle(i, j, CircleControls::radius));
    //             CircleControls::circle_list[i][j] = true;
    //         }
    //     }
    // }

    // for (int i = 0; i < sizeof(midiParams); i++)
    //     midiParams[i] = 0;
}

//--------------------------------------------------------------
void ofApp::update()
{
    // OSC -----------------------------------------------------
    static int channel;
    static int value;
    while (receiver.hasWaitingMessages())
    {
        ofxOscMessage message;
        receiver.getNextMessage(message);

        if (message.getAddress() == "/midi/channel/")
        {
            ofLogNotice(message.getAddress());
            ofLogNotice(ofToString(message.getArgAsInt(0)));
            channel = message.getArgAsInt(0);
        }
        else if (message.getAddress() == "/midi/value/")
        {
            ofLogNotice(message.getAddress());
            ofLogNotice(ofToString(message.getArgAsInt(0)));
            value = message.getArgAsInt(0);
        }
        midiParams[channel] = value;

        CircleControls::radius.set(max(midiParams[16], 1));
        CircleControls::spawnProbability.set(float(midiParams[17]) / 127);
        CircleControls::shrinkFactor.set(float(midiParams[18]) / 127 * 10);
        CircleControls::growFactor.set(float(midiParams[19]) / 127 * 10);
    }

    // video ---------------------------------------------------
    Globals::video.update();
    m_Grabber.update();

    colorImg.setFromPixels(Globals::video.getPixels());
    colorImg.convertToGrayscalePlanarImage(grayImg, 0);
    grayImg.threshold(160);

    Canny(grayImg, edge_img, LineDetection::edgeThreshold, LineDetection::edgeThreshold * 2);
    Sobel(edge_img, sobel_img);

    ofPixels &vidPixels = Globals::video.getPixels();

    int r = CircleControls::radius;

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
            circle->life_cycle -= CircleControls::shrinkFactor;
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

    mCapFbo.begin();
    { // write everything to buffer

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
    } // buffer end
    mCapFbo.end();

    // write buffer to file
    if (m_Recorder.isRecording())
    {
        // ofxFastFboReader can be used to speed this up :)
        mCapFbo.readToPixels(mPix);
        if (mPix.getWidth() > 0 && mPix.getHeight() > 0)
        {
            m_Recorder.addFrame(mPix);
        }
    }

    // draw buffer
    mCapFbo.draw(0, 0);
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

        int r = CircleControls::radius;

        // only create circles if frameRate > 30
        // if (ofGetFrameNum() > 1 && ofGetFrameRate() > 30)
        // {
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
        // }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    if (key == 'c')
    {
        CircleControls::radius = ofRandom(2, 10);
        CircleControls::resize_circles();
    }

    else if (key == 'f')
    {
        ofToggleFullscreen();
    }

    if (key == 'r')
    {

        if (m_Recorder.isRecording())
        {
            // stop
            m_Recorder.stop();
        }
        else
        {
#if defined(TARGET_OSX)
            m_Recorder.setOutputPath(ofToDataPath(ofGetTimestampString() + ".mp4", true));
#else
            m_Recorder.setOutputPath(ofToDataPath(ofGetTimestampString() + ".avi", true));
#endif
            m_Recorder.startCustomRecord();
        }
    }

    else if (key == '+')
    {
        CircleControls::radius += CircleControls::radius * 0.3;
        CircleControls::resize_circles();
    }
    else if (key == '-')
    {
        CircleControls::radius -= CircleControls::radius * 0.3;
        // if (CircleControls::radius < CircleControls::radius_standard)
        //     CircleControls::radius = CircleControls::radius_standard;
        CircleControls::resize_circles();
    }

    else if (key == OF_KEY_RETURN) // load next video
    {
        Globals::vidIdx = (Globals::vidIdx + 1) % Globals::videoPaths.size();
        Globals::video.load(Globals::videoPaths[Globals::vidIdx]);

        vidWidth = Globals::video.getWidth();
        vidHeight = Globals::video.getHeight();

        Globals::video.play();

        colorImg.allocate(vidWidth, vidHeight);
        grayImg.allocate(vidWidth, vidHeight);
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
    // cout << ofSoundStreamListDevices() << endl;
    // // ofSoundStream::setDevice(2);
    // ofSoundStreamSetup(2, 0, 48000, 256, 4);

    // auto devices = soundStream.getDeviceList();

    // ofSoundStreamSettings settings;
    // settings.setOutDevice(devices[deviceNum]);

    // settings.setOutListener(this);
    // settings.sampleRate = 48000;
    // settings.numOutputChannels = 2;
    // settings.numInputChannels = 0;
    // settings.bufferSize = 256;
    // soundStream.setup(settings);
}