#include "TriggerFunctions.h"

///////////////////////// TRIGGER FUNCTIONS ///////////////////////////
bool TriggerFunctions::bUpdateCymbal = false;
bool TriggerFunctions::bUpdateHihat = false;
bool TriggerFunctions::bUpdateKick = false;
bool TriggerFunctions::bUpdateSnare = false;
string TriggerFunctions::song = "Theodolit";

// ----------------------------- Kick ---------------------------------
void TriggerFunctions::kickTrigger()
{
    // increase circle radius
    if (song != "Sattelstein")
    {

        float max_size = 30;
        CircleControls::radius = min(max_size, CircleControls::radius + 2);
    }
    else
    {
        LineDetection::minLineLength = max(27, LineDetection::minLineLength - 10);
    }
    bUpdateKick = true;
}

void TriggerFunctions::kickUpdate()
{
    if (bUpdateKick && song != "Sattelstein")
    {
        // decay circle radius until standard
        CircleControls::radius -= 1;
        if (CircleControls::radius <= CircleControls::radius_standard)
        {
            bUpdateKick = false;
        }
    }
    else
    {
        if (bUpdateKick)
        {
            LineDetection::minLineLength += 7;
            if (LineDetection::minLineLength >= 127)
            {
                bUpdateKick = false;
            }
        }
    }
}

// ---------------------------- Snare ---------------------------------
void TriggerFunctions::snareTrigger()
{
    /* creates 40 random circles */

    ofPixels &vidPixels = Globals::video.getPixels();
    int r = CircleControls::radius;

    for (int i = 0; i <= 40; i++)
    {
        CircleControls::checkPixelThreshold(ofRandom(Globals::video.getWidth()), ofRandom(Globals::video.getHeight()), ofRandom(255)); // checks pixel brightness threshold and creates/increases circles
    }

    if (song == "Sattelstein")
    {
        LineDetection::maxLineGap = max(27, LineDetection::minLineLength - 10);
        bUpdateSnare = true;
    }
}

void TriggerFunctions::snareUpdate()
{
    if (bUpdateSnare && song == "Sattelstein")
    {
        LineDetection::maxLineGap += 7;
        if (LineDetection::maxLineGap >= 150)
        {
            bUpdateSnare = false;
        }
    }
}

// ---------------------------- Cymbal --------------------------------
void TriggerFunctions::cymbalTrigger()
{
    // increase shrink factor:
    double maxShrinkFactor = 10;
    CircleControls::shrinkFactor = min(maxShrinkFactor, CircleControls::shrinkFactor + 0.3);
    bUpdateCymbal = true;
}

void TriggerFunctions::cymbalUpdate()
{
    static float shrinkStandard = 0.2;
    if (bUpdateCymbal)
    { // decay circle radius until standard
        CircleControls::shrinkFactor -= 0.1;
        if (CircleControls::shrinkFactor <= shrinkStandard)
        {
            bUpdateCymbal = false;
        }
    }
}

// ----------------------------- Bass ---------------------------------
void TriggerFunctions::hihatTrigger()
{
    // decrease minLineLength
    if (ofIsStringInString(song, "Improvisation"))
    {
        LineDetection::minLineLength = max(10, LineDetection::minLineLength - 10);
        bUpdateHihat = true;
        LineDetection::bDrawLines = true;
    }
}

void TriggerFunctions::hihatUpdate()
{
    // increase minLineLength until max
    if (bUpdateHihat && ofIsStringInString(song, "Improvisation"))
    {
        LineDetection::minLineLength += 3;
        if (LineDetection::minLineLength >= 100)
        {
            bUpdateHihat = false;
        }
    }
}

// -------------------------- other: ----------------------------------

void increaseCircleRadius()
{

    bool bUpdateThis = false;
    // increase circle radius
    int max_size = 50;
    CircleControls::radius = min(max_size, int(CircleControls::radius + 1));
    bUpdateThis = true;
}

void increaseCircleRadiusUpdate()
{

    bool bUpdateThis = true;
    if (bUpdateThis)
    {

        // decay circle radius until standard
        CircleControls::radius -= 0.5;
        if (CircleControls::radius <= CircleControls::radius_standard)
        {
            bUpdateThis = false;
        }
    }
}