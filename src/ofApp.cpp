#include "ofApp.h"
#include <iostream>
#include "Globals.h"
#include "TriggerFunctions.h"

//--------------------------------------------------------------
void ofApp::setup()
{

    // ofSetFrameRate(30);
    ofSetWindowTitle("ofApp");
    ofSetVerticalSync(true);

    // communication -------------------------------------------
    receiver.setup(PORT);
    // serial.setup("/dev/ttyACM0", 9600);
    // serial.startContinuousRead(); // TODO: FIX THIS! APPLICATION CRASHES AFTER TWO SECONDS!!!
    // ofAddListener(serial.NEW_MESSAGE, this, &ofApp::onNewMessage);

    // circles setup -------------------------------------------
    ofSetCircleResolution(100);
    // TODO:
    // CircleControls::radius.addListener(this, &CircleControls::radiusChanged);

    // audioSetup(2);

    // load videos from data folder ----------------------------
    ofDirectory dir(""); // read folder ./bin/data
    dir.allowExt("mp4");
    dir.allowExt("avi");
    dir.listDir();
    dir = dir.getSorted();
    for (int i = 0; i < dir.size(); i++)
        Globals::videoPaths.push_back(dir.getPath(i));

    Globals::video.load(Globals::videoPaths[0]);
    Globals::vidWidth = Globals::video.getWidth();
    Globals::vidHeight = Globals::video.getHeight();

    Globals::video.play();

    Globals::colorImage.allocate(Globals::vidWidth, Globals::vidHeight);
    Globals::grayImage.allocate(Globals::vidWidth, Globals::vidHeight);

    // create circles initially --------------------------------
    CircleControls::initialCircleCreation(Globals::vidWidth, Globals::vidHeight);

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

        // incoming instrument trigger (via serial from python)
        if (ofIsStringInString(ofToString(message), "Snare"))
        {
            TriggerFunctions::snareTrigger();
        }
        else if (ofIsStringInString(ofToString(message), "Crash1"))
        {
            TriggerFunctions::cymbalTrigger();
        }
        else if (ofIsStringInString(ofToString(message), "Kick"))
        {
            TriggerFunctions::kickTrigger();
        }
        else if (ofIsStringInString(ofToString(message), "Hihat"))
        {
            // cowbell input used as bass piezo mic
            TriggerFunctions::hihatTrigger();
        }
        // else if (ofIsStringInString(ofToString(message), "footswitch"))
        // {
        //     Globals::loadNextVideo();
        // }

        // react to different songs:
        if (ofIsStringInString(ofToString(message), "Theodolit"))
        {
            TriggerFunctions::song = "Theodolit";

            Globals::video.load("00-satellite_panner.mp4");
            Globals::vidWidth = Globals::video.getWidth();
            Globals::vidHeight = Globals::video.getHeight();

            Globals::video.play();

            Globals::colorImage.allocate(Globals::vidWidth, Globals::vidHeight);
            Globals::grayImage.allocate(Globals::vidWidth, Globals::vidHeight);
        }
        else if (ofIsStringInString(ofToString(message), "Improvisation"))
        {
            TriggerFunctions::song = "Improvisation";
            LineDetection::bDrawLines = true;

            Globals::video.load("01-boundarylayers_short_1080p.mp4");
            Globals::vidWidth = Globals::video.getWidth();
            Globals::vidHeight = Globals::video.getHeight();

            Globals::video.play();

            Globals::colorImage.allocate(Globals::vidWidth, Globals::vidHeight);
            Globals::grayImage.allocate(Globals::vidWidth, Globals::vidHeight);
        }
        else if (ofIsStringInString(ofToString(message), "Sattelstein"))
        {
            TriggerFunctions::song = "Sattelstein";
            LineDetection::minLineLength = 27;
            LineDetection::maxLineGap = 23;
            CircleControls::bDrawCircles = false;
            LineDetection::bDrawLines = true;

            Globals::video.load("00-satellite_panner.mp4");
            Globals::vidWidth = Globals::video.getWidth();
            Globals::vidHeight = Globals::video.getHeight();

            Globals::video.play();

            Globals::colorImage.allocate(Globals::vidWidth, Globals::vidHeight);
            Globals::grayImage.allocate(Globals::vidWidth, Globals::vidHeight);
        }
        else if (ofIsStringInString(ofToString(message), "KupferUndGold"))
        {
            TriggerFunctions::song = "KupferUndGold";
            LineDetection::bDrawLines = false;
            CircleControls::bDrawCircles = true;
            CircleControls::bAutomaticCircleCreation = true;

            Globals::video.load("02-arbol-shift-01_long.mp4");
            Globals::vidWidth = Globals::video.getWidth();
            Globals::vidHeight = Globals::video.getHeight();

            Globals::video.play();

            Globals::colorImage.allocate(Globals::vidWidth, Globals::vidHeight);
            Globals::grayImage.allocate(Globals::vidWidth, Globals::vidHeight);
        }

        // CircleControls::radius.set(max(midiParams[16], 1));
        // CircleControls::spawnProbability.set(float(midiParams[17]) / 127);
        // CircleControls::shrinkFactor.set(float(midiParams[18]) / 127 * 10);
        // CircleControls::growFactor.set(float(midiParams[19]) / 127 * 10);

        ofLogNotice(ofToString(message.getAddress()));
        ofLogNotice(ofToString(message));
    }

    // video ---------------------------------------------------
    Globals::video.update();
    m_Grabber.update();

    Globals::colorImage.setFromPixels(Globals::video.getPixels());
    Globals::colorImage.convertToGrayscalePlanarImage(Globals::grayImage, 0);
    Globals::grayImage.threshold(160);

    Canny(Globals::grayImage, Globals::edgeImage, LineDetection::edgeThreshold, LineDetection::edgeThreshold * 2);
    Sobel(Globals::edgeImage, Globals::sobelImage);

    // TODO: do this only for neighboring circles of already existent ones

    // int n = int(r * 2);
    // for (int i = 0; i < Globals::vidWidth - r * 2; i += r * 2)
    // {
    //     for (int j = 0; j < Globals::vidHeight - r * 2; j += r * 2)
    //     {
    //         if (CircleControls::circle_list[i][j] == true)
    //         {
    //             float brightness_val = vidPixels.getColor(i, j).getBrightness();
    //             CircleControls::checkPixelThreshold(i, j, brightness_val);
    //             CircleControls::checkPixelThreshold(i+n, j, brightness_val);
    //             CircleControls::checkPixelThreshold(i+n, j+n, brightness_val);
    //             CircleControls::checkPixelThreshold(i, j+n, brightness_val);
    //             CircleControls::checkPixelThreshold(i-n, j+n, brightness_val);
    //             CircleControls::checkPixelThreshold(i-n, j, brightness_val);
    //             CircleControls::checkPixelThreshold(i-n, j-n, brightness_val);
    //             CircleControls::checkPixelThreshold(i, j-n, brightness_val);
    //             CircleControls::checkPixelThreshold(i+n, j-n, brightness_val);
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

    //-------------------------- UPDATE CIRCLES -----------------------
    ofPixels &vidPixels = Globals::video.getPixels();
    int r = CircleControls::radius;

    if (CircleControls::bDrawCircles)
    {

        // naturally create one circle at a time:
        if (ofGetFrameNum() > 1 && ofGetFrameRate() > 15)
        {
            // -------------- pixel brightness: -----------------------
            // int i = int(ofRandom(0, ofGetWindowWidth()));
            // int j = int(ofRandom(0, ofGetWindowHeight()));
            for (int i = r * 2; i < Globals::vidWidth; i += r * 2)
            {
                for (int j = r * 2; j < Globals::vidHeight; j += r * 2)
                {
                    float threshold_val;
                    if (CircleControls::spawn_mode[CircleControls::spawn_index] == "brightness")
                        threshold_val = vidPixels.getColor(i, j).getBrightness();
                    else
                        threshold_val = vidPixels.getColor(i, j).getLightness(); // TODO: make selectable
                    CircleControls::checkPixelThreshold(i, j, threshold_val);    // checks pixel brightness threshold and creates/increases circles
                }
            }
        }

        // ----------------- naturally decrease life_cycle: -----------
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

    // ------------------------------ Trigger Updates -----------------
    TriggerFunctions::cymbalUpdate();
    TriggerFunctions::hihatUpdate();
    TriggerFunctions::kickUpdate();
    TriggerFunctions::snareUpdate();
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
        if (LineDetection::bDrawLines)
        {
            LineDetection::lineDetection();
        }

        // ---------------------------- CIRCLES  --------------------------
        ofPixels &vidPixels = Globals::video.getPixels();

        for (auto &circle : CircleControls::circles)
        {
            circle->color = vidPixels.getColor(circle->x, circle->y);
            if (CircleControls::bDrawCircles)
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

// Serial receive ----------------------------------------------
void ofApp::onNewMessage(string &message)
{
    cout << "onNewMessage, message: " << message << "\n";

    // if (message.rfind("hit", 0) == 0)
    {
        ofLogNotice(ofToString(message));

        // if (message.rfind("Snare") != message.npos)
        if (ofToString(message).compare("Snare"))
            TriggerFunctions::snareTrigger();
        if (message.rfind("Kick") != message.npos)
            TriggerFunctions::kickTrigger();
        if (message.rfind("Crash1") != message.npos)
            TriggerFunctions::cymbalTrigger();
        if (message.rfind("Cowbell") != message.npos) // cowball input used as bass piezo mic
            TriggerFunctions::hihatTrigger();
    }
    if (message.rfind("step") != message.npos) // cowball input used as bass piezo mic
        Globals::loadNextVideo();
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

    else if (key == 'b') // represents kick drum
    {
        TriggerFunctions::kickTrigger();
    }
    else if (key == 'n') // snare trigger
    {
        TriggerFunctions::snareTrigger();
    }

    else if (key == 'g')
    {
        TriggerFunctions::cymbalTrigger();
    }

    else if (key == 'h')
    {
        TriggerFunctions::hihatTrigger();
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
        CircleControls::resize_circles();
    }

    else if (key == OF_KEY_RETURN) // load next video
    {
        Globals::loadNextVideo();
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

//--------------------------------------------------------------
void ofApp::radiusChanged()
{
    CircleControls::circles.clear();
}
